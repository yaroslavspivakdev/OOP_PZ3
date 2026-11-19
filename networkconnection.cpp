#include "networkconnection.h"

NetworkConnection::NetworkConnection(const IPAddress& address,
    unsigned short port, std::ostream& output)
    : address_(address), port_(port), output_(output) {
}

const IPAddress& NetworkConnection::getIPAddress() const {
    return address_;
}

unsigned short NetworkConnection::getPort() const {
    return port_;
}

std::ostream& NetworkConnection::getOutputStream() const {
    return output_;
}