//
// Created by pawkrol on 12/6/15.
//

#ifndef TCPCLIENT_PACKET_H
#define TCPCLIENT_PACKET_H

#include <stdint.h>
#include <iostream>
#include <bits/stringfwd.h>

class Packet {
private:
    const static int _dataSizeOffset = 10;
    const static int _bytesLeft = 245;
    const static int _headerSize = 11;
    const static int _s = 256;
    uint8_t buff[_s];

    std::string data;
    uint8_t size = _headerSize; //packet size, 11 = header size

    static uint16_t id;

    uint16_t packetId;

public:
    Packet();
    Packet(uint8_t *recv);

    void putData(std::string data);
    void putData(uint8_t *data, size_t length);

    uint16_t getId();
    uint64_t getTime();
    std::string getData();
    uint8_t getDataSize();
    int getSize();
};


#endif //TCPCLIENT_PACKET_H
