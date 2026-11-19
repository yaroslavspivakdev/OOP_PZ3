#include "httpconnection.h"

#include <stdexcept>

HTTPConnection::HTTPConnection(const IPAddress& address, unsigned short port,
    std::ostream& output, int statusCode)
    : NetworkConnection(address, port, output), statusCode_(statusCode) {
    if (statusCode < 100 || statusCode > 599) {
        throw std::invalid_argument("Invalid HTTP status code");
    }
}

void HTTPConnection::setStatusCode(int statusCode) {
    if (statusCode < 100 || statusCode > 599 || headersSent_) {
        throw std::logic_error("Cannot change HTTP status code");
    }
    statusCode_ = statusCode;
}

int HTTPConnection::getStatusCode() const {
    return statusCode_;
}

void HTTPConnection::setHeader(const std::string& name, const std::string& value) {
    if (headersSent_) {
        throw std::logic_error("HTTP headers have already been sent");
    }
    if (name.empty() || name.find_first_of(":\r\n") != std::string::npos ||
        value.find_first_of("\r\n") != std::string::npos) {
        throw std::invalid_argument("Invalid HTTP header");
    }
    headers_[name] = value;
}

const std::map<std::string, std::string>& HTTPConnection::getHeaders() const {
    return headers_;
}

bool HTTPConnection::headersSent() const {
    return headersSent_;
}

void HTTPConnection::setBody(const std::string& body) {
    if (headersSent_) {
        throw std::logic_error("HTTP message has already been sent");
    }
    body_ = body;
}

const std::string& HTTPConnection::getBody() const {
    return body_;
}

std::string HTTPConnection::getReasonPhrase(int statusCode) {
    switch (statusCode) {
    case 200: return "OK";
    case 201: return "Created";
    case 202: return "Accepted";
    case 204: return "No Content";
    case 400: return "Bad Request";
    case 401: return "Unauthorized";
    case 403: return "Forbidden";
    case 404: return "Not Found";
    case 500: return "Internal Server Error";
    default: return "Status";
    }
}

void HTTPConnection::sendHeaders(const std::string& startLine,
    std::size_t contentLength) {
    if (headersSent_) {
        throw std::logic_error("HTTP headers have already been sent");
    }

    // Content-Length завжди дорівнює фактичній кількості байтів тіла
    headers_["Content-Length"] = std::to_string(contentLength);
    std::ostream& output = getOutputStream();
    output << startLine << "\r\n";
    for (const auto& header : headers_) {
        output << header.first << ": " << header.second << "\r\n";
    }
    output << "\r\n";
    if (!output) {
        throw std::runtime_error("Unable to write HTTP headers");
    }
    headersSent_ = true;
}

void HTTPConnection::send() {
    // Для звичайного HTTP-з'єднання записуємо відповідь із текстовим тілом
    sendHeaders("HTTP/1.1 " + std::to_string(statusCode_) + " " +
        getReasonPhrase(statusCode_),
        body_.size());

    getOutputStream().write(body_.data(),
        static_cast<std::streamsize>(body_.size()));
    if (!getOutputStream()) {
        throw std::runtime_error("Unable to write HTTP body");
    }
}
