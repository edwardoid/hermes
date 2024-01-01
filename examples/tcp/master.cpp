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
#include <unistd.h>
#include <thread>
#include <functional>
#include <vector>
#include <sstream>

#include <hermes/Master.h>
#include <hermes/UnixTCPSocketIO.h>

int socket_fd = 0;

hermes::Master master(nullptr);

std::vector<std::thread*> slaves;

// void slave_routine(hermes::IO* io)
// {
//     while(io->good())
//     {
//         hermes::Message msg;
//         while(io->available() < sizeof(hermes::Message) && io->good())
//             io->wait(sizeof(hermes::Message));
//         if (!io->read(msg))
//             break;
//         master.newMesssage(io, msg);
//         break;
//     }
//     delete io;
// }

bool accept_all(const hermes::serial_t& serial, hermes::token_t& token)
{
    for(int i = 0; i < HERMES_TOKEN_LENGTH; ++i) token.data[i] = rand();
    return true;
}

void new_thread_for_client(hermes::SlaveDescriptor* slave)
{
    static int num = 0;
    std::thread t([slave]()
    {
        std::cout << "Requesting properties count..." << std::endl;
        int c = slave->propertiesCount();
        {
            std::cout << "Properties count " << c << std::endl;
        }
        for(int i = 0; i < c; ++i)
        {
            std::string name = slave->propertyName(i);
            std::cout << "Property " << i << " name is '" << name << "'" << std::endl;
            hermes::ValueData vd;
            if (slave->get(i, vd))
                std::cout << "\t\t\t" << name << " = " << hermes::vd2str(vd) << std::endl;
            else
                std::cout << "\t\t\t*** CANT'T GET VALUE FOR PROPERTY " << i << "!!!***";
        }
        std::cout << "Disconnecting...";
        //master.close(*slave);
        while (1)
        {
            sleep(1);
            slave->propertiesCount();
        }   
    });
    std::stringstream ss;
    ss << "Client_" << ++num;
    pthread_setname_np(t.native_handle(), ss.str().c_str());
    t.detach();
}

int main(int argc, char** argv)
{
    master.setAuthenticator(accept_all);
    master.setOnNewSlaveCallback(new_thread_for_client);
    int sockfd, connfd; 
    struct sockaddr_in servaddr;

    unsigned int port = 1311;
    if (argc < 2 || sscanf(argv[1], "%u", &port) != 1)
    {
        std::cerr << "Usage: " << argv[0] << " <port num>" << std::endl;
        return -1;
    }

    // socket create and verification 
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1)
    {
        std::cerr << "Error: " << strerror(errno) << std::endl;
        return -1;
    }

    const int reuse = 1;

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) < 0)
    {
        std::cerr << "Error: " << strerror(errno) << std::endl;
        return -1;
    }

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &reuse, sizeof(int)) < 0)
    {
        std::cerr << "Error: " << strerror(errno) << std::endl;
        return -1;
    }

    // struct timeval tv;
    // bzero(&tv, sizeof(timeval));
    // tv.tv_sec = 500;

    // if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0)
    // {
    //     std::cerr << "Error: " << strerror(errno) << std::endl;
    //     return -1;
    // }

    bzero(&servaddr, sizeof(servaddr)); 
   
    // assign IP, PORT 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(port); 
   
    // Binding newly created socket to given IP and verification 
    if ((bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))) != 0)
    {
        std::cerr << "Error: " << strerror(errno) << std::endl;
        return -1;
    }
   
    // Now server is ready to listen and verification 
    if ((listen(sockfd, 5)) != 0) { 
        printf("Listen failed...\n"); 
    }

    std::cout << "Listening on " << port << "..." << std::endl;
    
    struct sockaddr_in client;
    socklen_t clientLen = sizeof(&client);

    int clientfd = 0;

    while (clientfd = accept(sockfd, (struct sockaddr*)&client, &clientLen))
    {
        std::cout << "Client connected" << std::endl;
        hermes::UnixTCPSocketIO *io =  new hermes::UnixTCPSocketIO(clientfd);
        if (!master.accept(io))
            delete io;
    }
   
    // After chatting close the socket 
    ::close(sockfd);
    return 0;
}