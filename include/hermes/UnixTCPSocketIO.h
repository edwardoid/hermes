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

#ifndef HM_UNIX_TCP_SOCKET_IO_H
#define HM_UNIX_TCP_SOCKET_IO_H

#ifdef HAS_LINUX_HEADERS

#include <hermes/IO.h>

#include <vector>
#include <mutex>

namespace hermes
{
    class UnixTCPSocketIO: public IO
    {
    public:
        UnixTCPSocketIO(int sc, int timeout = 10);
        virtual ~UnixTCPSocketIO();
        virtual buffer_length_t wait(buffer_length_t length) override;
        virtual buffer_length_t available() const override;
        virtual buffer_length_t write(const byte_t* buffer, buffer_length_t sz) override;
        virtual buffer_length_t read(byte_t* buffer, buffer_length_t sz) override;
        virtual bool good() const override;
        virtual void flush() override;
        virtual bool close() override;

    private:
        int m_sfd;
        std::mutex m_ioMx;
        mutable std::vector<byte_t> m_buffer;
        mutable bool m_good;
    };
}

#endif // HAS_LINUX_HEADERS

#endif // HM_UNIX_TCP_SOCKET_IO_H