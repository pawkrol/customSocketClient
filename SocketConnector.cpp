//
// Created by pawkrol on 11/28/15.
//

#include <string.h>
#include <unistd.h>

#include "SocketConnector.h"

SocketConnector::SocketConnector(std::string address, std::string port) {
    this->address = address;
    this->port = port;

    setStructs();
    setSocket();
}

void SocketConnector::setStructs(){ //Ustawianie struktur
    addrinfo hostData;
    addrinfo *hostDataList;

    memset(&hostData, 0, sizeof(hostData)); //Zerowanie pamięci

    hostData.ai_family = AF_UNSPEC; //IPv4 lub IPv6
    hostData.ai_socktype = SOCK_STREAM; //TCP

    this->status = getaddrinfo(address.c_str(), port.c_str(), &hostData, &hostDataList);
    if (this->status != 0){
        std::cerr << "getaddrinfo ERROR: " << gai_strerror(status) << std::endl;
    }

    this->hostDataList = hostDataList;
}

void SocketConnector::setSocket(){ //Ustawianie socketu, używanie pierwszego dostępnego adresu z listy
    this->socketfd = socket(this->hostDataList->ai_family,
                            this->hostDataList->ai_socktype,
                            this->hostDataList->ai_protocol);
    if (this->socketfd == -1){
        std::cerr << "Socket error" << std::endl;
        exit(-1);
    }
}

void SocketConnector::makeConnect(){ //Łączenie
    int status = connect(this->socketfd,
                         this->hostDataList->ai_addr, this->hostDataList->ai_addrlen);
    if (status == -1){
        std::cerr << "Connection error" << std::endl;
        exit(-1);
    }

    freeHostsList();
}

int SocketConnector::getSocket() {
    return this->socketfd;
}

void SocketConnector::freeHostsList() {
    freeaddrinfo(hostDataList);
}

void SocketConnector::closeSocket() {
    close(socketfd);
}

ssize_t SocketConnector::sendMessage(void *buff, size_t size) { //Wysyłanie wiadomości
    ssize_t bytesSent;

    bytesSent = send(socketfd, buff, size, 0);

    return bytesSent;
}

ssize_t SocketConnector::sendMessage(Packet *packet) {
    ssize_t bytesSent;

    bytesSent = send(socketfd, packet, (size_t)packet->getSize(), 0);

    return bytesSent;
}

ssize_t SocketConnector::getResponse(void *buff, size_t size) { //Odbieranie wiadomości
    ssize_t received = recv(socketfd, buff, size, 0);
    if (received == -1) {
        std::cerr << "Receiving error" << std::endl;
    } else if (received == 0) {
        std::cerr << "Server closed connection" << std::endl;
    }
    return received;
}

ssize_t SocketConnector::getResponse(Packet *&packet) { //Odbieranie wiadomości
    uint8_t tmp[256];
    ssize_t receivedH = recv(socketfd, tmp, 11, 0);
    if (receivedH == -1) {
        std::cerr << "Receiving header error" << std::endl;
        return receivedH;
    } else if (receivedH == 0) {
        std::cerr << "Server closed connection" << std::endl;
        return receivedH;
    }

    packet = new Packet(tmp);
    ssize_t receivedD = recv(socketfd, tmp, packet->getDataSize(), 0);
    packet->putData(tmp, packet->getDataSize());
    if (receivedD == -1) {
        std::cerr << "Receiving data error" << std::endl;
    } else if (receivedD == 0) {
        std::cerr << "Server closed connection" << std::endl;
    }

    return receivedH + receivedD;
}