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


#include <hermes/DummySlave.h>
#include <hermes/MessageBuilder.h>
#include <hermes/IO.h>
#include <string.h>

using namespace hermes;

DummySlave::DummySlave(IO* io, const byte_t* serial, const byte_t* token)
    : m_io(io)
    , m_serial(serial)
    , m_token(token)
{
}

bool DummySlave::handshake()
{
    Message msg = MessageBuilder::handshake(m_serial.data, 1, 0, 0, m_token.data);
    if (!m_io->write(msg)) {
        return false;
    }

    Message response;
    if (!m_io->read(response)) {
        return false;
    }

    if (response.type == MessageType::Handshake) {
        m_token = response.token;
        return true;
    }

    return false;
}

void DummySlave::loop()
{
    bool getResult = false;
    do {
        getResult = processNextMessage();
    } while (getResult);
}

bool DummySlave::processNextMessage()
{
    bool getResult = false;
    Message rcv;
    getResult = m_io->read(rcv);
    Message rpl;
    if (dispatch(&rcv, &rpl)) {
        m_io->write(rpl);
    }
    return getResult;
}

bool DummySlave::dispatch(Message* message, Message* response)
{
    switch (message->type) {
    case MessageType::Command: {
        return handleCommandRequest(message, response);
    }
    }

    return false;
}

bool DummySlave::handleCommandRequest(Message* msg, Message* response)
{
    bool handled = true;
    MessageBuilder::setSerial(*response, m_serial.data);
    MessageBuilder::setToken(*response, m_token.data);

    switch (msg->payload.command.command) {
    case Command::GetPropertiesCount: {
        response->type = MessageType::Command;
        response->payload.command.command = Command::GetPropertiesCount;
        response->payload.command.data.count = propertiesCount();
        break;
    }

    case Command::GetPropertyName: {
        response->type = MessageType::Command;
        response->payload.command.command = Command::GetPropertyName;

        const char* pname = propertyName(msg->payload.command.data.index);
        if (pname != NULL) {
            size_t len = strlen(pname);
            memcpy(response->payload.command.data.string, pname, len);
            response->payload.command.data.string[len] = '\0';
        }
        else {
            HM_ERR("Requested property name with bad index %d", (int)msg->payload.command.data.index);
            MessageBuilder::setError(*response, ErrorType::Unsupported, "Bad property index");
        }
        break;
    }

    case Command::Get: {
        response->type = MessageType::Command;
        response->payload.command.command = Command::Get;

        const int8_t idx = propertyIndex(msg->payload.command.data.get.name);
        bool ok = idx < propertiesCount() && idx >= 0;
        if (ok) {
            memcpy(response->payload.command.data.value.name,
                   msg->payload.command.data.get.name,
                   strlen(msg->payload.command.data.get.name));
            response->payload.command.data.value.name[strlen(msg->payload.command.data.get.name)] = '\0';
            response->payload.command.data.value.type = propertyType(idx);
            ok = get(idx, response->payload.command.data.get);
        }
        else if (!ok) {
            MessageBuilder::setError(*response, ErrorType::Unsupported, "Property does not exists");
        }
        break;
    }

    case Command::Set: {
        const int8_t idx = propertyIndex(msg->payload.command.data.get.name);
        bool ok = idx < propertiesCount() && idx >= 0;
        if (ok) {
            memcpy(response->payload.command.data.value.name,
                   msg->payload.command.data.get.name,
                   strlen(msg->payload.command.data.get.name));
            response->payload.command.data.value.name[strlen(msg->payload.command.data.get.name)] = '\0';
            response->payload.command.data.value.type = propertyType(idx);
            ok = set(idx, msg->payload.command.data.set) && get(idx, response->payload.command.data.set);
        }
        else if (!ok) {
            MessageBuilder::setError(*response, ErrorType::Unsupported, "Property does not exists");
        }
        break;
    }

    default: {
        return false;
    }
    }

    return true;
}