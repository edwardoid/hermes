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


#ifndef HM_MASTER_H
#define HM_MASTER_H

#include <hermes/IO.h>
#include <hermes/Message.h>
#include <hermes/SlaveDescriptor.h>
#include <list>

namespace hermes
{
    /**
     * Callback for handling connections from new clients
     * 
    */
    typedef void (*on_new_slave_fn_t)(SlaveDescriptor* slave);

    /**
     * Callback for autentification
     * 
    */
    typedef bool (*authenticate_fn_t)(const serial_t& serial, token_t& token);

    class Master
    {
    public:
        Master(IO* io);
        inline void setOnNewSlaveCallback(on_new_slave_fn_t cb) { m_new_client = cb; }
        
        inline void setAuthenticator(authenticate_fn_t authenticator)
        { m_authenticator = authenticator; }

        bool accept(IO* io);

        void close(SlaveDescriptor& slave);
    private:
        IO* m_io;
        on_new_slave_fn_t m_new_client = nullptr;
        authenticate_fn_t m_authenticator = nullptr;
        std::list<SlaveDescriptor> m_slaves;
    };
}

#endif // HM_MASTER_H
