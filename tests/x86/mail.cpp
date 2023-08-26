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

#include <hermes/Message.h>
#include <hermes/InMemoryIO.h>
#include <iostream>
#include <thread>
#include <mutex>

std::vector<hermes::byte_t> buf;
const size_t BUF_MAX_SIZE = 16 * sizeof(hermes::Message);
std::mutex sync, mx;

hermes::InMemoryIO io1(buf, sync, BUF_MAX_SIZE);
hermes::InMemoryIO io2(buf, sync, BUF_MAX_SIZE);

int main(int argc, char** argv)
{
    std::thread first([]()
    {
        while(1)
        {
            hermes::Message msg;
            hermes::buffer_length_t req = sizeof(msg);
            hermes::buffer_length_t got = io1.read(reinterpret_cast<hermes::byte_t*>(&msg), sizeof(msg));

            {
                std::lock_guard<std::mutex> lock(mx);
                std::cout << "Reader: Requested " << req << " bytes, got " << got << " bytes. Sleeping" << std::endl;
                std::cout << "Received msg " << (int) msg.type
                    << ".As commat it is command "<< (int) msg.payload.command.command
                    << " with string data '" << msg.payload.command.data.string << "'"
                    << std::endl;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    });

    std::thread second([]()
    {
        int i = 0;
        while(1)
        {
            hermes::Message msg;
            msg.type = hermes::MessageType::Command;
            msg.payload.command.command = hermes::Command::GetPropertyName;
            sprintf(msg.payload.command.data.string, "Prop%d", (i++) % 32);
            hermes::buffer_length_t req = sizeof(msg);
            hermes::buffer_length_t got = io2.write(reinterpret_cast<hermes::byte_t*>(&msg), sizeof(msg));

            {
                std::lock_guard<std::mutex> lock(mx);
                std::cout << "Writer: Requested " << req << " bytes, got " << got << " bytes. Sleeping" << std::endl;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    });
    first.join();
    second.join();
    return 0;
}