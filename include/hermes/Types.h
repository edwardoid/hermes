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


#ifndef HM_TYPES_H
#define HM_TYPES_H

#include <hermes/Config.h>

#ifdef HAS_STDINT_H
#include <stdint.h>
#endif // HAS_STDINT_H

#include <memory.h>

namespace hermes
{
    using byte_t = uint8_t;
    using buffer_length_t = uint16_t;

    template<int Length>
    struct Buffer
    {
        inline Buffer(byte_t val = 0) { memset(data, val, Length); }
        inline Buffer(const byte_t* val) { *this = val; }
        inline Buffer(Buffer& src) { *this = src; }
        inline const Buffer& operator = (const Buffer& src) { *this = src.data; return *this;}
        inline const Buffer& operator = (const byte_t* src) { memcpy(data, src, Length); return *this; }
        inline bool operator == (const Buffer& src) const { return *this == src.data; }
        inline bool operator!= (const Buffer& src) const { return *this != src.data; }
        inline bool operator == (const byte_t* src) const { return memcmp(data, src, Length) == 0; }
        inline bool operator != (const byte_t* src) const { return !(*this == src); }

        byte_t data[Length];
    };

    using token_t = Buffer<HERMES_TOKEN_LENGTH>;
    using serial_t = Buffer<HERMES_SERIAL_LENGTH>;
} // namespace hermes

#endif // HM_TYPES+H