#include "ipaddress.h"

#include <cctype>
#include <sstream>
#include <stdexcept>

IPAddress::IPAddress(const std::string& address) {
    if (address.empty() || address.back() == '.') {
        throw std::invalid_argument("Invalid IPv4 address");
    }

    std::istringstream input(address);
    std::string part;

    // Перевіряємо чотири складові IPv4-адреси
    for (unsigned int& octet : octets_) {
        if (!std::getline(input, part, '.') || part.empty() || part.size() > 3) {
            throw std::invalid_argument("Invalid IPv4 address");
        }

        for (char symbol : part) {
            if (!std::isdigit(static_cast<unsigned char>(symbol))) {
                throw std::invalid_argument("Invalid IPv4 address");
            }
        }

        const unsigned long value = std::stoul(part);
        if (value > 255) {
            throw std::invalid_argument("Invalid IPv4 address");
        }
        octet = static_cast<unsigned int>(value);
    }

    // Додаткові частини адреси заборонені
    if (std::getline(input, part, '.')) {
        throw std::invalid_argument("Invalid IPv4 address");
    }
}

std::string IPAddress::toString() const {
    std::ostringstream output;
    output << octets_[0] << '.' << octets_[1] << '.'
        << octets_[2] << '.' << octets_[3];
    return output.str();
}