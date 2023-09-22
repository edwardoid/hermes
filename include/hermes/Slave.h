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

#ifndef HM_SLAVE_H
#define HM_SLAVE_H

#include <hermes/Types.h>
#include <hermes/ValueData.h>

namespace hermes
{
    class Slave
    {
    public:
        /**
         * Constructor
         * @param io IO to be used during communication
         * @param serial Serial number for the slave device
         * @param token Authentification tocken
        */
        Slave() = default;

        /**
         * @return Properties count associated with this slave.
        */
        virtual uint8_t propertiesCount() const = 0;

        /**
         * @param index Index of property.
         * @return Name of property number index.
         * @see propertiesCount()
         * @see propertyIndex()
        */
        virtual const char* propertyName(uint8_t index) const = 0;

        /**
         * @param name Name of the property.
         * @return Index of the property.
         * @see propertiesCount()
         * @see propertyName()
        */
        virtual int8_t propertyIndex(const char* name) const = 0;

        /**
         * Get value type for the property.
         * @param index Property index.
        */
        virtual ValueType propertyType(uint8_t index) const = 0;

        /**
         * Assign new value to a property
         * @param property Index of the property
         * @param value New value.
         * @return false if writing new value to the property failed.
        */
        virtual bool set(uint8_t property, const ValueData& value) = 0;

        /**
         * Fetch currrent value of a property
         * @param property Index of the property
         * @param value Storage for value.
         * @param false if Fetching failed for some reason.
        */
        virtual bool get(uint8_t property, ValueData& value) = 0;
    };

} // namespace hermes

#endif // HM_SLAVE_H
