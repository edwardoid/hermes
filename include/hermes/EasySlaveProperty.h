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


#ifndef HM_EASY_SLAVE_PROPERTY_H
#define HM_EASY_SLAVE_PROPERTY_H

#include <hermes/ValueData.h>
#include <string.h>

namespace hermes
{
    struct SlaveProperty: public ValueData
    {
        virtual bool set(const ValueData& in) = 0;
        virtual bool get(ValueData& out) const = 0;
    };

    template<typename BasicT>
    struct BasicType
    {
        static const ValueType type;
    };

    template<>
    const ValueType BasicType<bool>::type = ValueType::Boolean;

    template<>
    const ValueType BasicType<int32_t>::type = ValueType::Integer;

    template<>
    const ValueType BasicType<char*>::type = ValueType::String;

    template<>
    const ValueType BasicType<float>::type = ValueType::Float;

    template<typename CachedT>
    struct CachedSlaveProperty : public SlaveProperty
    {
        CachedSlaveProperty(CachedT v = CachedT())
            : value(v)
        {
            type = BasicType<CachedT>::type;
        }
        CachedT value;

        virtual bool set(const ValueData& in)
        {
            if (in.type != type)
                return false;
        }
        virtual bool get(ValueData& out) const
        {
            out.type = type;

            return true;
        }
    };

    template<>
    bool CachedSlaveProperty<bool>::set(const ValueData& in)
    {
        if (in.type != type)
            return false;

        value = in.value.B != 0;
        return true;
    }

    template<>
    bool CachedSlaveProperty<bool>::get(ValueData& out) const
    {
        out.value.B = value ? 1 : 0;
        return true;
    }

    template<>
    bool CachedSlaveProperty<int32_t>::set(const ValueData& in)
    {
        if (in.type != type)
            return false;

        value = in.value.I;
        return true;
    }

    template<>
    bool CachedSlaveProperty<int32_t>::get(ValueData& out) const
    {
        out.value.I = value;
        return true;
    }

    template<>
    bool CachedSlaveProperty<char*>::set(const ValueData& in)
    {
        if (in.type != type)
            return false;

        strcpy(value, in.value.S);
        return 0;
    }

    template<>
    bool CachedSlaveProperty<char*>::get(ValueData& out) const
    {
        strcpy(out.value.S, value);
        return true;
    }

    template<>
    bool CachedSlaveProperty<float>::set(const ValueData& in)
    {
        if (in.type != type)
            return false;

        value = (float) in.value.F.V;
        value /= (float) in.value.F.Precision;

        return 0;
    }
}

#endif // HM_EASY_SLAVE_PROPERTY_H