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


#ifndef HM_IN_MEMORY_IO_H
#define HM_IN_MEMORY_IO_H

#include <hermes/IO.h>
#include <stddef.h>

#ifdef HAS_STDTHREAD_H
#include <mutex>
#endif // HAS_STDTHREAD_H

#include <vector>

namespace hermes
{
    class InMemoryIO: public IO
    {
    public:
        InMemoryIO( std::vector<byte_t>& bufOut,
                    std::vector<byte_t>& bufIn,
                    #ifdef HAS_STDTHREAD_H
                    std::mutex& mx,
                    #endif // HAS_STDTHREAD_H
                    size_t maxSize = 1024);
        virtual ~InMemoryIO() {};
        virtual buffer_length_t wait(buffer_length_t length) override;
        virtual buffer_length_t available() const override;
        virtual buffer_length_t write(const byte_t* buffer, buffer_length_t sz) override;
        virtual buffer_length_t read(byte_t* buffer, buffer_length_t sz) override;
        virtual bool good() const override;
        virtual void flush() override;
        virtual bool close() override { return true; }
    private:
        size_t m_max;
        #ifdef HAS_STDTHREAD_H
        std::mutex& m_mx;
        #endif // HAS_STDTHREAD_H
        std::vector<byte_t>& m_bufOut;
        std::vector<byte_t>& m_bufIn;
    };
}

#endif // HM_IN_MEMORY_IO_H

