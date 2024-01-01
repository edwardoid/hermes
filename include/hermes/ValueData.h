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


#ifndef HM_VALUE_DATA_H
#define HM_VALUE_DATA_H

#include <hermes/Types.h>

namespace hermes
{
    enum ValueType : uint8_t
    {
        Boolean = 0,
        Integer = 1,
        UnsignedInteger = 2,
        String = 3,
        Float = 4
    };

    struct FloatValue
    {
        int32_t V;
        uint16_t Precision;
    } __attribute__((packed));

    struct ValueData
    {
        char name[HERMES_PROPERTY_NAME_MAX_LENGTH];
        ValueType type; // 1
        union {
            uint8_t B;  // 1
            int32_t I;  // 4
            uint32_t U; // 4
            char S[HERMES_STRING_LENGTH];
            FloatValue F;
        } value;
    } __attribute__((packed));
} // namespace hermes

#endif // HM_VALUE_DATA_H