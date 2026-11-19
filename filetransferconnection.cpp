#include "filetransferconnection.h"

#include <fstream>
#include <stdexcept>

FileTransferConnection::FileTransferConnection(
    const IPAddress& address, unsigned short port, std::ostream& output,
    const std::filesystem::path& filePath)
    : NetworkConnection(address, port, output),
    filePath_(std::filesystem::absolute(filePath)) {
}

const std::filesystem::path& FileTransferConnection::getFilePath() const {
    return filePath_;
}

void FileTransferConnection::writeFileContent() {
    std::ifstream input(filePath_, std::ios::binary);
    if (!input) {
        throw std::runtime_error("Unable to open source file");
    }

    char buffer[4096];
    while (input.read(buffer, sizeof(buffer)) || input.gcount() > 0) {
        getOutputStream().write(buffer, input.gcount());
        if (!getOutputStream()) {
            throw std::runtime_error("Unable to write destination file");
        }
    }

    if (input.bad()) {
        throw std::runtime_error("Unable to read source file");
    }
}

void FileTransferConnection::send() {
    writeFileContent();
}