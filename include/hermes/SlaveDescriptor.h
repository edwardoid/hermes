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


#ifndef HM_SLAVE_DESCRIPTOR_H
#define HM_SLAVE_DESCRIPTOR_H

#include <hermes/IO.h>
#include <hermes/Event.h>
#include <hermes/Message.h>
#include <hermes/Slave.h>
#include <vector>
#include <list>
#include <string>


namespace hermes
{
    /**
     * Callback to check is device with serial and token can be accepted.
     * @param serial Serial number of the slave device
     * @param token Current token of the slave. Is token is empty, consider
     *              slave asking for a new token. Token also can contain some data
     *              in cases when slave was accepted some time ago, and wants
     *              to use the same token. However, it SlaveDescriptor device accepts token,
     *              it can update it with a new value, and for next requests and
     *              responses slave **HAS TO** use new token.
     * @return true if authentificaton succeeded
    */
    typedef bool (*authenticate_fn_t)(const serial_t& serial, token_t& token);

    /**
     * Callback for new events
     * 
    */
    typedef void (*on_event_fn_t)(event_t& event);

    class SlaveDescriptor: public Slave
    {
    public:
        /**
         * @param io Communication channel
         * @param auth Callback function to check if client can be accepted
        */
        SlaveDescriptor(IO* io, serial_t serial);

        /**
         * Makes request to obtain available properties count
         * @return Properties count associated with this slave.
         * @see Slave::propertiesCount
        */
        virtual uint8_t propertiesCount() override;

        /**
         * Makes request to obtain property name by index
         * @param index Index of the property
         * @param name Name of the property
         * @return True if slave responded with name
         * @see Slave::propertyName
        */
        virtual bool propertyName(uint8_t index, char* name) override;

        /**
         * Helper function to get name of a property
         * @see SlaveDescriptor::propertyName(uint8_t, char*)
        */
        std::string propertyName(uint8_t index);

        /**
         * Makes request to obtain property's index by name
         * @return Index of the property or -1.
         * @see Slave::propertyIndex
        */
        virtual int8_t propertyIndex(const char* name) override;


        /**
         * Get value type for the property.
         * @param index Property index.
        */
        virtual ValueType propertyType(uint8_t index) override;

        /**
         * Assign new value to a property
         * @param property Index of the property
         * @param value New value.
         * @return false if writing new value to the property failed.
        */
        virtual bool set(uint8_t property, const ValueData& value) override;

        /**
         * Fetch currrent value of a property
         * @param property Index of the property
         * @param value Storage for value.
         * @param false if Fetching failed for some reason.
        */
        virtual bool get(uint8_t property, ValueData& value) override;

        /**
         * @return Serial id
        */
        inline const serial_t& serial() const { return m_serial; }

        /**
         * Set callback to handle new events from clients
         * @param callback Callback
        */
        inline void setEventsHandlerCallback(on_event_fn_t callback) { m_on_event = callback; }

        void close();
    protected:
        friend class Master;
        void add(const Message& msg);
        bool handle(Message& msg);
        Message makeRequest(const Message& msg);
    private:
        IO* m_io;
        std::list<Message> m_msgs;
        serial_t m_serial;
        token_t m_token;
        Message m_rsp;
        on_event_fn_t m_on_event = nullptr;
    };
}

#endif // HM_SLAVE_DESCRIPTOR_H