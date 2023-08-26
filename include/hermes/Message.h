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

#ifndef HM_MESSAGE_H
#define HM_MESSAGE_H

#include <hermes/Config.h>
#include <hermes/CommandPayload.h>
#include <hermes/ErrorPayload.h>
#include <hermes/HandshakePayload.h>

#ifdef HAS_STDINT_H
#include <stdint.h>
#endif // HAS_STDINT_H

namespace hermes
{
    enum class MessageType : uint8_t
    {
        Error = 0,
        InternalError = 1,
        Command = 2,
        Handshake = 3
    };

    struct Message // 256 bytes
    {
        byte_t serial[HERMES_SERIAL_LENGTH];  // 8 by default
        byte_t token[HERMES_TOKEN_LENGTH];    // 8 by default
        uint16_t payloadLength;               // 2
        MessageType type;                     // 1
        union Payload {
            CommandPayload command;
            ErrorPayload error;
            HandshakePayload handshake;
        } payload;
    } __attribute__((packed));
}

#endif // HM_MESSAGE_H