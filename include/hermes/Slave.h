#ifndef HM_SLAVE_H
#define HM_SLAVE_H

#include <hermes/IO.h>
#include <hermes/Message.h>

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
        Slave(IO* io, const byte_t* serial, const byte_t* token);

        /**
         * Perform handshake with master
         * @return Reeturns true if handshake succeeded.
         * @note This is an blocking method
        */
        bool handshake();
        void loop();

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

        /**
         * Process next incoming message from master.
        */
        bool processNextMessage();

    protected:
        bool dispatch(Message* message, Message* response);
        bool handleCommandRequest(Message* msg, Message* response);

    protected:
        IO* m_io;
        const byte_t m_serial[HERMES_SERIAL_LENGTH];
        byte_t m_token[HERMES_TOKEN_LENGTH];
    };

} // namespace hermes

#endif // HM_SLAVE_H
