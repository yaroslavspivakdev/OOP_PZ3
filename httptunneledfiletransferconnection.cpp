#include "httptunneledfiletransferconnection.h"

#include <cctype>
#include <filesystem>
#include <stdexcept>

HTTPTunneledFileTransferConnection::HTTPTunneledFileTransferConnection(
    const IPAddress& address, unsigned short port, std::ostream& output,
    const std::filesystem::path& filePath, const std::string& boundary)
    : NetworkConnection(address, port, output),
    FileTransferConnection(address, port, output, filePath),
    HTTPConnection(address, port, output),
    boundary_(boundary) {
    // Роздільник multipart не повинен містити пробілів або керівних символів
    if (boundary_.empty() || boundary_.size() > 70) {
        throw std::invalid_argument("Invalid multipart boundary");
    }
    for (char symbol : boundary_) {
        if (!std::isalnum(static_cast<unsigned char>(symbol)) && symbol != '-' &&
            symbol != '_') {
            throw std::invalid_argument("Invalid multipart boundary");
        }
    }
}

std::string HTTPTunneledFileTransferConnection::getMultipartPrefix() const {
    // За заголовками частини йде порожній рядок, а потім двійкові дані
    return "--" + boundary_ + "\r\n"
        "Content-Disposition: form-data; name=\"file1\"; filename=\"" +
        getFilePath().filename().string() + "\"\r\n"
        "Content-Type: application/octet-stream\r\n"
        "\r\n";
}

std::string HTTPTunneledFileTransferConnection::getMultipartSuffix() const {
    // Подвійний дефіс наприкінці позначає завершення multipart-повідомлення
    return "\r\n--" + boundary_ + "--\r\n";
}

void HTTPTunneledFileTransferConnection::send() {
    const std::string prefix = getMultipartPrefix();
    const std::string suffix = getMultipartSuffix();
    const std::uintmax_t fileSize = std::filesystem::file_size(getFilePath());

    // Обчислюємо розмір усього multipart-тіла, включно з його заголовками
    const std::uintmax_t totalLength =
        prefix.size() + fileSize + suffix.size();

    setHeader("Host", getIPAddress().toString() + ":" +
        std::to_string(getPort()));
    setHeader("Content-Type", "multipart/form-data; boundary=" + boundary_);
    sendHeaders("POST /upload HTTP/1.1", static_cast<std::size_t>(totalLength));

    std::ostream& output = getOutputStream();
    output.write(prefix.data(), static_cast<std::streamsize>(prefix.size()));
    writeFileContent();
    output.write(suffix.data(), static_cast<std::streamsize>(suffix.size()));
    if (!output) {
        throw std::runtime_error("Unable to write multipart message");
    }
}
