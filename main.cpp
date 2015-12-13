#include <iostream>

#include "SocketConnector.h"

using namespace std;

int main() {

    SocketConnector *sc = new SocketConnector("127.0.0.1", "8989");
    sc->makeConnect();

    Packet sendPacket;
    sendPacket.putData("Hello");
    sc->sendMessage(&sendPacket);

    Packet *getPacket;
    sc->getResponse(getPacket);
    cout << getPacket->getId();
    delete getPacket;

    Packet sendPacket2;
    sendPacket2.putData("Bye");
    sc->sendMessage(&sendPacket2);

    Packet *getPacket2;
    sc->getResponse(getPacket2);
    cout << getPacket2->getId();
    delete getPacket2;

    sc->closeSocket();
    delete sc;

    return 0;
}