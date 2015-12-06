//
// Created by pawkrol on 11/28/15.
//

#ifndef TCPCLIENT_SOCKETCONNECTOR_H
#define TCPCLIENT_SOCKETCONNECTOR_H

#include <netdb.h>
#include <iostream>
#include "Packet.h"

class SocketConnector {
private:
    addrinfo *hostDataList;
    int socketfd;

    std::string address;
    std::string port;

    void setStructs();
    void setSocket();
    void freeHostsList();

public:
    int status;

    SocketConnector(std::string address, std::string port);

    void makeConnect();
    void closeSocket();

    ssize_t sendMessage(void* buff, size_t size);
    ssize_t sendMessage(Packet *packet);
    ssize_t getResponse(void* buff, size_t size);
    ssize_t getResponse(Packet *&packet);

    int getSocket();
};


#endif //TCPCLIENT_SOCKETCONNECTOR_H
