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

#include <hermes/Master.h>
#include <hermes/Message.h>

using namespace hermes;

Master::Master(IO* io)
    : m_io(io)
{}

bool Master::accept(IO* io)
{
    Message msg;
    while(io->available() < sizeof(hermes::Message) && io->good())
        io->wait(sizeof(hermes::Message));
    if (!io->read(msg))
        return false;
    
    HM_DBG("New message receive: %s", mt2str(msg.type));
    
    
    switch(msg.type)
    {
        case MessageType::Handshake:
        {
            bool accept = true;
            if (m_authenticator == nullptr)
            {
                HM_ERR("Authentificator is not set!");
            }
            else
            {
                token_t token = msg.token;
                accept = m_authenticator(msg.serial, token);
                if (accept)
                {
                    memcpy(msg.token, token.data, HERMES_TOKEN_LENGTH);
                }
                else
                {
                    m_slaves.remove_if([msg](const SlaveDescriptor& slave) { return slave.serial() == msg.serial; });
                    HM_WARN("Client rejected");
                }
            }

            io->write(msg);

            if (!accept)
            {
                return false;
            }

            break;
        }
        case MessageType::Command:
        {
            HM_DBG("Got command %s response", cmd2str(msg.payload.command.command));
            break;
        }
        default:
        {

        }
    }
    SlaveDescriptor* descriptor = nullptr;

    for(auto& s : m_slaves)
    {
        if(s.serial() == msg.serial)
        {
            descriptor = &s;
            break;
        }
    }

    if (descriptor == nullptr)
    {
        m_slaves.emplace_back(io, msg.serial);
        descriptor = & *(m_slaves.rbegin());
        if (m_new_client)
        {
            (*m_new_client)(descriptor);
        }
    }

    return true;
}

void Master::close(SlaveDescriptor& target)
{
    m_slaves.remove_if([&target](const SlaveDescriptor& slave) { return target.serial() == slave.serial(); });
    target.close();
}