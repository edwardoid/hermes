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

#ifndef HM_COMMAND_PAYLOAD_H
#define HM_COMMAND_PAYLOAD_H

#include <hermes/CommandData.h>

namespace hermes
{
    enum class Command : byte_t
    {
        Set = 2,
        Get = 3,
        GetPropertiesCount = 4,
        GetPropertyName = 5
    };

    struct CommandPayload
    {
        Command command; // 1
        CommandData data;
    } __attribute__((packed));
} // namespace hermes

#endif // AB_PROTO_COMMAND_PAYLOAD_H