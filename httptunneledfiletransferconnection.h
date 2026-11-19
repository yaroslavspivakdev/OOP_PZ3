#ifndef HTTPTUNNELEDFILETRANSFERCONNECTION_H
#define HTTPTUNNELEDFILETRANSFERCONNECTION_H

#include "filetransferconnection.h"
#include "httpconnection.h"

#include <string>

class HTTPTunneledFileTransferConnection : public FileTransferConnection,
    public HTTPConnection {
public:
    HTTPTunneledFileTransferConnection(
        const IPAddress& address, unsigned short port, std::ostream& output,
        const std::filesystem::path& filePath,
        const std::string& boundary = "Asrf456BGe4h");

    void send() override;

private:
    std::string getMultipartPrefix() const;
    std::string getMultipartSuffix() const;

    std::string boundary_;
};

#endif