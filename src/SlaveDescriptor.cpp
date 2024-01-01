/**
 * Hermes - A RPC for IOT
 * Copyright (C) 2023  Eduard Sargsyan and Andrey Ovodov
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include <hermes/SlaveDescriptor.h>
#include <hermes/MessageBuilder.h>
#include <hermes/Message.h>
#include <hermes/Config.h>

using namespace hermes;

SlaveDescriptor::SlaveDescriptor(IO* io, serial_t serial)
    : m_io(io)
    , m_serial(serial)
{}

uint8_t SlaveDescriptor::propertiesCount()
{
    Message req;
    MessageBuilder::setSerial(req, m_serial.data);
    MessageBuilder::setToken(req, m_token.data);
    req.type = MessageType::Command;
    req.payload.command.command = Command::GetPropertiesCount;
    req.payloadLength = sizeof(CommandData);
    Message resp = makeRequest(req);
    return (resp.type == MessageType::Command && resp.payload.command.command == Command::GetPropertiesCount)
            ? resp.payload.command.data.count
            : 0;
}

bool SlaveDescriptor::propertyName(uint8_t index, char* name)
{
    Message req;
    MessageBuilder::setSerial(req, m_serial.data);
    MessageBuilder::setToken(req, m_token.data);
    req.type = MessageType::Command;
    req.payload.command.command = Command::GetPropertyName;
    req.payload.command.data.index = index;
    req.payloadLength = sizeof(CommandData);
    Message resp = makeRequest(req);
    if (resp.type == MessageType::Command && resp.payload.command.command == Command::GetPropertyName) {
        strcpy(name, resp.payload.command.data.string);
        return true;
    }
    return false;
}

std::string SlaveDescriptor::propertyName(uint8_t idx)
{
    char name[HERMES_PROPERTY_NAME_MAX_LENGTH];
    std::string res;
    if (propertyName(idx, name))
        res = name;
    return res;
}

int8_t SlaveDescriptor::propertyIndex(const char* name)
{
    static char tmpName[HERMES_PROPERTY_NAME_MAX_LENGTH];
    for(int8_t i = 0; i < propertiesCount(); ++i)
    {
        if(!propertyName(i, tmpName))
            continue;

        if(0 == strcmp(name, tmpName))
        {
            return i;
        }
    }
    return -1;
}

ValueType SlaveDescriptor::propertyType(uint8_t index)
{
    ValueData vt;
    if (get(index, vt))
    {
        return vt.type;
    }
    return ValueType::Boolean;
}

bool SlaveDescriptor::set(uint8_t property, const ValueData& value)
{
    Message req;
    MessageBuilder::setSerial(req, m_serial.data);
    MessageBuilder::setToken(req, m_token.data);
    req.type = MessageType::Command;
    req.payload.command.command = Command::Set;
    if(!propertyName(property, req.payload.command.data.get.name))
        return false;

    req.payload.command.data.value = value;
    req.payloadLength = sizeof(CommandData);
    Message resp = makeRequest(req);
    return resp.type == MessageType::Command && resp.payload.command.command == Command::Set;
}

bool SlaveDescriptor::get(uint8_t property, ValueData& value)
{
    Message req;
    MessageBuilder::setSerial(req, m_serial.data);
    MessageBuilder::setToken(req, m_token.data);
    req.type = MessageType::Command;
    req.payload.command.command = Command::Get;
    if(!propertyName(property, req.payload.command.data.get.name))
        return false;
    
    req.payloadLength = sizeof(CommandData);
    Message resp = makeRequest(req);

    if(resp.type == MessageType::Command && resp.payload.command.command == Command::Get)
    {
        value = resp.payload.command.data.value;
        return true;
    }
    return false;
}

Message SlaveDescriptor::makeRequest(const Message& msg)
{
    m_rsp.type = MessageType::Error;
    m_rsp.payload.error.e = ErrorType::Fail;
    static uint8_t errLen = strlen("Request failed\0");
    memcpy(m_rsp.payload.error.msg, "Request failed\0", errLen);
    if(m_io->write(msg))
    {
        memset(&m_rsp, 0, sizeof(m_rsp));
        if(!m_io->read(m_rsp)) {
            HM_ERR("Failed to get response for request %s", mt2str(msg.type));
        }
    } else{
        HM_ERR("Sending request %s to client failed", mt2str(msg.type));
    }
    return m_rsp;
}

void SlaveDescriptor::close()
{
    Message req;
    MessageBuilder::setSerial(req, m_serial.data);
    MessageBuilder::setToken(req, m_token.data);
    req.type = MessageType::Command;
    req.payload.command.command = Command::Disconnect;
    req.payloadLength = sizeof(CommandData);

    m_io->write(req);
    m_io->close();
}
