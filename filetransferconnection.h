#ifndef FILETRANSFERCONNECTION_H
#define FILETRANSFERCONNECTION_H

#include "networkconnection.h"

#include <filesystem>

class FileTransferConnection : public virtual NetworkConnection {
public:
    FileTransferConnection(const IPAddress& address, unsigned short port,
        std::ostream& output,
        const std::filesystem::path& filePath);

    const std::filesystem::path& getFilePath() const;
    void send() override;

protected:
    // Копіюємо вміст файлу без зміни жодного байта
    void writeFileContent();

private:
    std::filesystem::path filePath_;
};

#endif