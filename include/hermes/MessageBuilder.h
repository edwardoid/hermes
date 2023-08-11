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


#ifndef HM_MESSAGE_BUILDER_H
#define HM_MESSAGE_BUILDER_H

#include <hermes/Message.h>
#include <hermes/Config.h>
#include <string.h>

namespace hermes
{
    struct MessageBuilder
    {
        static inline void setSerial(Message& msg, const uint8_t* serial) { memcpy(&msg.serial, serial, HERMES_SERIAL_LENGTH); }

        static inline void setToken(Message& msg, uint8_t* token) { memcpy(&msg.token, token, HERMES_TOKEN_LENGTH); }

        static inline void setError(Message& msg, ErrorType error, const char* cause)
        {
            msg.type = MessageType::Error;
            msg.payload.error.e = error;
            memcpy(msg.payload.error.msg, cause, strlen(cause));
            msg.payload.error.msg[strlen(cause)] = '\0';
            msg.payloadLength = sizeof(msg.payload.error);
        }
        static Message handshake(const uint8_t* serial, byte_t api_release, byte_t api_major, byte_t api_minor, uint8_t* token)
        {
            Message msg;
            msg.type = MessageType::Handshake;

            setSerial(msg, serial);
            setToken(msg, token);

            msg.payload.handshake.maximumVersion.Release = api_release;
            msg.payload.handshake.maximumVersion.Major = api_major;
            msg.payload.handshake.maximumVersion.Minor = api_minor;

            msg.payload.handshake.minimumVersion.Release = api_release;
            msg.payload.handshake.minimumVersion.Major = api_major;
            msg.payload.handshake.minimumVersion.Minor = api_minor;

            msg.payload.handshake.desiredVersion.Release = api_release;
            msg.payload.handshake.desiredVersion.Major = api_major;
            msg.payload.handshake.desiredVersion.Minor = api_minor;

            msg.payloadLength = sizeof(msg.payload.command);

            return msg;
        }
    };
} // namespace hermes

#endif // HM_MESSAGE_BUILDER_H