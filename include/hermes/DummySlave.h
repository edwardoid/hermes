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

#ifndef HM_DUMMY_SLAVE_H
#define HM_DUMMY_SLAVE_H


#include <hermes/Slave.h>
#include <hermes/Message.h>

namespace hermes
{
    class IO;

    class DummySlave: public Slave
    {
    public:
        /**
         * Constructor
         * @param io IO to be used during communication
         * @param serial Serial number for the slave device
         * @param token Authentification tocken
        */
        DummySlave(IO* io, const byte_t* serial, const byte_t* token);

        /**
         * Perform handshake with master
         * @return Returns true if handshake succeeded.
         * @note This is an blocking method
        */
        bool handshake();
        void loop();

        /**
         * Process next incoming message from master.
        */
        bool processNextMessage();

    protected:
        bool dispatch(Message* message, Message* response);
        bool handleCommandRequest(Message* msg, Message* response);

    protected:
        IO* m_io;
        const serial_t m_serial;
        token_t m_token;
    };

} // namespace hermes

#endif // HM_DUMMY_SLAVE_H
