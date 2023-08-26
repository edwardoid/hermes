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


#include <hermes/InMemoryIO.h>

#ifdef HAS_THREAD_H
#include <thread>
#include <chrono>
#endif // HAS_THREAD_H

#include <memory.h>

using namespace hermes;

InMemoryIO::InMemoryIO( std::vector<byte_t>& buf,
                        #ifdef HAS_STDTHREAD_H
                        std::mutex& mx,
                        #endif // HAS_STDTHREAD_H
                        size_t maxSize)
    : m_max(maxSize)
    #ifdef HAS_STDTHREAD_H
    , m_mx(mx)
    #endif // HAS_STDTHREAD_H
    , m_buf(buf)
{
}

buffer_length_t InMemoryIO::wait(buffer_length_t length)
{
    while(available() < length)
    {
        #ifdef HAS_THREAD_H
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        #endif // HAS_THREAD_H
    }

    return available();
}

buffer_length_t InMemoryIO::available() const
{
    #ifdef HAS_THREAD_H
    std::lock_guard<std::mutex> lock(m_mx);
    #endif // HAS_THREAD_H
    return static_cast<buffer_length_t>(m_buf.size());
}

buffer_length_t InMemoryIO::write(const byte_t* buffer, buffer_length_t sz)
{
    if (good())
    {
        #ifdef HAS_THREAD_H
        std::lock_guard<std::mutex> lock(m_mx);
        #endif // HAS_THREAD_H
        sz = std::min(m_buf.size() + sz, m_max) - m_buf.size();
        m_buf.insert(m_buf.end(), buffer, buffer + sz);
        return sz;
    }
    return 0;
}

buffer_length_t InMemoryIO::read(byte_t* buffer, buffer_length_t sz)
{
    while(available() < sz)
    {
        wait(sz);
    }
    if (good())
    {
        #ifdef HAS_THREAD_H
        std::lock_guard<std::mutex> lock(m_mx);
        #endif // HAS_THREAD_H
        sz = std::min(static_cast<size_t>(sz), m_buf.size());
        memcpy(buffer, m_buf.data(), sz);
        m_buf.erase(m_buf.begin(), m_buf.begin() + sz);
        return sz;
    }
    return 0;
}

bool InMemoryIO::good() const
{
    return m_buf.size() <= m_max;
}

void InMemoryIO::flush()
{
    m_buf.clear();
}