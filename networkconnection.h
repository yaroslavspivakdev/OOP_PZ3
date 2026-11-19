#ifndef NETWORKCONNECTION_H
#define NETWORKCONNECTION_H

#include "ipaddress.h"

#include <ostream>

class NetworkConnection {
public:
    NetworkConnection(const IPAddress& address, unsigned short port,
        std::ostream& output);
    virtual ~NetworkConnection() = default;

    const IPAddress& getIPAddress() const;
    unsigned short getPort() const;

    // Кожен тип з'єднання реалізує власний спосіб передавання даних
    virtual void send() = 0;

protected:
    std::ostream& getOutputStream() const;

private:
    IPAddress address_;
    unsigned short port_;
    std::ostream& output_;
};

#endif