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


#ifndef HM_ERROR_PAYLOAD_H
#define HM_ERROR_PAYLOAD_H

#include <hermes/Types.h>

namespace hermes
{
    enum class ErrorType : uint8_t
    {
        OK = 0,
        Unsupported = 1,
        AccessDenied = 2,
        BadType = 3
    };

    struct ErrorPayload
    {
        ErrorType e;
        char msg[100];
    } __attribute__((packed));
} // namespace hermes

#endif // HM_ERROR_PAYLOAD_H