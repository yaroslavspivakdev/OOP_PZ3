#ifndef IPADDRESS_H
#define IPADDRESS_H

#include <array>
#include <string>

class IPAddress {
public:
    explicit IPAddress(const std::string& address);
    std::string toString() const;

private:
    std::array<unsigned int, 4> octets_{};
};

#endif