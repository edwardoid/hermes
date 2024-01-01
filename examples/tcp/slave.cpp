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

#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <hermes/UnixTCPSocketIO.h>
#include <hermes/EasySlave.h>
#include <hermes/EasySlaveProperty.h>

int socket_fd = 0;

hermes::CachedSlaveProperty<char*> model("Model", const_cast<char*>("AquaboxBase Dosator"));
hermes::CachedSlaveProperty<float> remain1("Channel_1_remain", 3.145);
hermes::CachedSlaveProperty<float> remain2("Channel_2_remain", 2.74);
hermes::byte_t token[HERMES_TOKEN_LENGTH];
hermes::byte_t serial[HERMES_SERIAL_LENGTH];


int main(int argc, char** argv)
{
    struct sockaddr_in addr;

    if(argc < 2)
    {
        printf("\n Usage: %s <ip>:<port> \n",argv[0]);
        return 1;
    }

    if((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    }

    std::string host = argv[1];
    host = host.substr(0, host.find_last_of(':'));
    int port = std::atoi(argv[1] + host.size() + 1);

    if (host == "")
        host = "localhost";

    struct hostent *he = gethostbyname (host.c_str());
    if (he == NULL)
    {
        std::cerr << "Error: Can't resolve address " << host << std::endl;
        return 1;
    }

    host = inet_ntoa(*(struct in_addr*)he->h_addr_list[0]);

    std::cout << "Trying to connect to " << host << std::endl;

    memset(&addr, '0', sizeof(addr)); 

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port); 

    if(inet_pton(AF_INET, host.c_str(), &addr.sin_addr)<=0)
    {
        std::cerr << "Error: " << strerror(errno) << std::endl;
        return 1;
    } 

    if(connect(socket_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
       std::cerr << "Error: " << strerror(errno) << std::endl;
       return 1;
    } 

/*    struct timeval tv;
    bzero(&tv, sizeof(timeval));
    tv.tv_sec = 60 * 60 * 10000;

    if (setsockopt(socket_fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0)
    {
        std::cerr << "Error: " << strerror(errno) << std::endl;
        return -1;
    }
*/
    hermes::SlaveProperty* props[] = { &model, &remain1, &remain2 };
    hermes::UnixTCPSocketIO io(socket_fd);


    srand(time(NULL));

    *(reinterpret_cast<uint32_t*>(serial)) = rand();

    hermes::EasySlave<3> slave( props, &io, serial, token);


    if(!slave.handshake())
    {
        std::cerr << "Handshake failed!" << std::endl;
    } else {
        std::cout << "Handshake succeeded" << std::endl;
    }

    while (io.good())
    {
        slave.loop();
    }

    if (errno == 0)
        io.close();
    else
        std::cerr << "Error: " << strerror(errno) << std::endl;
    return errno;
}