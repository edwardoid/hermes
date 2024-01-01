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

#include <hermes/UnixTCPSocketIO.h>

#ifdef HAS_LINUX_HEADERS

#include <chrono>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>

using namespace hermes;

UnixTCPSocketIO::UnixTCPSocketIO(int sc, int timeoutSeconds)
{
    m_sfd = sc;

    int flags = fcntl(m_sfd, F_GETFL, 0);
    if (flags != -1)
    {
        if(!(fcntl(m_sfd, F_SETFL, flags & ~O_NONBLOCK) == 0))
        {
            m_good = false;
            HM_WARN("Can't change socket's flags: %d", (int) errno);
            return;
        }
    } else
    {
        m_good = false;
        HM_WARN("Can't get socket's flags: %d", (int) errno);
        return;
    }

    struct timeval tv;
    tv.tv_sec = 30;
    tv.tv_usec = 0;
    m_good = (setsockopt(m_sfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof(tv)) == 0);
    if(!m_good)
    {
        HM_WARN("Can't set read timeout for socket: %d", (int) errno);
        m_good = false;
    };
}

void UnixTCPSocketIO::flush()
{ }

bool UnixTCPSocketIO::close()
{
    m_good = false;
    ::close(m_sfd);
    return true;
}

bool UnixTCPSocketIO::good() const
{
    return m_good;
}

buffer_length_t UnixTCPSocketIO::wait(buffer_length_t length)
{
    if (!m_good) {
        return 0;
    }
    if (m_buffer.size() >= length) {
        return length;
    }
    buffer_length_t remain = length - m_buffer.size();
    buffer_length_t avail = available();
    if (avail > remain) {
        remain = avail;
    }
    m_buffer.resize(m_buffer.size() + remain);

    int count = ::read(m_sfd, m_buffer.data() + (m_buffer.size() - remain), remain);

    if (count < 1)
    {
        HM_DBG("Read failed with: %s", strerror(errno));
    }

    if (count <= 0) {
        m_good = false;
    }

    return m_buffer.size();
}

buffer_length_t UnixTCPSocketIO::available() const
{
    int count = 0;
    if (ioctl(m_sfd, FIONREAD, &count) < 0) {
        m_good = false;
        HM_WARN("ioctl failed for with %d", (int) errno);
        count = 0;
    }

    return static_cast<buffer_length_t>(count + m_buffer.size());
}

buffer_length_t UnixTCPSocketIO::write(const byte_t* buffer, buffer_length_t sz)
{
    int count = ::write(m_sfd, buffer, sz);
    return count > 0 ? count : 0;
}

buffer_length_t UnixTCPSocketIO::read(byte_t* buffer, buffer_length_t sz)
{
    if (m_buffer.size() < sz) {
        buffer_length_t remain = sz - m_buffer.size();
        HM_DBG("Waiting for %d bytes", (int) remain);
        wait(remain);
    }

    buffer_length_t len = sz > m_buffer.size() ? m_buffer.size() : sz;
    if (len > 0) {
        memcpy(buffer, m_buffer.data(), len);
        m_buffer.erase(m_buffer.begin(), m_buffer.begin() + len);
    } else {
        HM_WARN("Read failed with %d. Got %d bytes", (int) errno, (int)len);
    }
    return len;
}

#endif // HAS_LINUX_HEADERS