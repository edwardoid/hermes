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


#ifndef HM_EASY_SLAVE_H
#define HM_EASY_SLAVE_H

#include <hermes/DummySlave.h>
#include <hermes/EasySlaveProperty.h>

namespace hermes
{
    /**
     * @class EasySlave basic implementation for Slaves
    */
    template<uint8_t PropertiesCount>
    class EasySlave : public DummySlave
    {
    public: 
        using SlavePropertyPtr = SlaveProperty*;

        /**
         * @param props List of available properties
         * @param io Communication channel
         * @param serial Serial number of the slave
         * @param token Token received from master from last time if available
        */
        EasySlave(SlavePropertyPtr* props, IO* io, const byte_t* serial, const byte_t* token)
            : DummySlave(io, serial, token)
            , m_vars(props)
        {}


        /**
         * @return Properties count associated with the slave
        */
        inline uint8_t propertiesCount() const { return PropertiesCount; }

        /**
         * Get property name by index
         * @param index Property intex
         * @return Name of property
         * @note Here is no check if index is correct, be careful calling this function!
        */
        inline const char* propertyName(uint8_t index) const { return m_vars[index]->name; }

        /**
         * Get index of property by name.
         * @param name ASCII name of the property
         * @return Index of the property, or -1 if there is no such property
         * @note Since in many embedded systems there is no implementation of hash maps, and
         *       usually properties count is quite a small number, lookup is not optimized
         *       and has O(n) complexity.
        */
        int8_t propertyIndex(const char* name) const
        {
            for (int i = 0; i < PropertiesCount; ++i) {
                if (strcmp(m_vars[i]->name, name) == 0) {
                    return i;
                }
            }

            return -1;
        }

        /**
         * Get property value type by index.
         * @param index Property index.
         * @note Here is no check if index is correct, be careful calling this function!
        */
        ValueType propertyType(uint8_t index) const { return m_vars[index]->type; }

        /**
         * Set new value to the property
         * @param property Index of the property
         * @param value New value.
         * @return true if property type is correct and value has been updated.
         * @note Here is no check if index is correct, be careful calling this function!
        */
        bool set(uint8_t property, const ValueData& value) { return m_vars[property]->set(value); }

        /**
         * Get current value of the property
         * @param property Index of the property
         * @param value value of the property
         * @return true if property value has been fetched successeful
         * @note Here is no check if index is correct, be careful calling this function!
        */
        bool get(uint8_t property, ValueData& value) { return m_vars[property]->get(value); }

    protected:
        void setProps(SlavePropertyPtr* props) { m_vars = props; }

    protected:
        SlavePropertyPtr* m_vars;
    };
}

#endif // HM_EASY_SLAVE_H