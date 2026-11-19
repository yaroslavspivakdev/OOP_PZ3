#ifndef HTTPCONNECTION_H
#define HTTPCONNECTION_H

#include "networkconnection.h"

#include <cstddef>
#include <map>
#include <string>

class HTTPConnection : public virtual NetworkConnection {
public:
    HTTPConnection(const IPAddress& address, unsigned short port,
        std::ostream& output, int statusCode = 200);

    void setStatusCode(int statusCode);
    int getStatusCode() const;

    void setHeader(const std::string& name, const std::string& value);
    const std::map<std::string, std::string>& getHeaders() const;
    bool headersSent() const;

    void setBody(const std::string& body);
    const std::string& getBody() const;
    void send() override;

protected:
    // Формуємо HTTP-заголовки з довжиною тіла у байтах
    void sendHeaders(const std::string& startLine, std::size_t contentLength);

private:
    static std::string getReasonPhrase(int statusCode);

    int statusCode_;
    std::map<std::string, std::string> headers_;
    bool headersSent_ = false;
    std::string body_;
};

#endif
