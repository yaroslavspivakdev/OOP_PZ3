#include "filetransferconnection.h"
#include "httpconnection.h"
#include "httptunneledfiletransferconnection.h"

#include <fstream>
#include <iostream>
#include <stdexcept>

int main() {
    try {
        const IPAddress address("192.168.1.100");
        const unsigned short port = 8080;

        // Створюємо двійковий тестовий файл для демонстрації передавання
        {
            std::ofstream source("sample.bin", std::ios::binary);
            if (!source) {
                throw std::runtime_error("Cannot create sample.bin");
            }
            const char data[] = { 'T', 'e', 's', 't', 0, 1, 2, '\n', 'E', 'N', 'D' };
            source.write(data, sizeof(data));
        }

        // Звичайне HTTP-з'єднання записує заголовки та текст повідомлення
        {
            std::ofstream output("http_message.txt", std::ios::binary);
            if (!output) {
                throw std::runtime_error("Cannot create http_message.txt");
            }

            HTTPConnection connection(address, port, output, 200);
            connection.setHeader("Content-Type", "text/plain; charset=utf-8");
            connection.setBody("Hello from HTTPConnection!\nThis is a test message.\n");
            connection.send();

            std::cout << "HTTP message saved to http_message.txt\n";
        }

        // Передавання файлу створює його точну двійкову копію
        {
            std::ofstream output("file_copy.bin", std::ios::binary);
            if (!output) {
                throw std::runtime_error("Cannot create file_copy.bin");
            }

            FileTransferConnection connection(address, port, output, "sample.bin");
            connection.send();

            std::cout << "Binary file copied to file_copy.bin\n";
        }

        // HTTP-тунель передає той самий файл у форматі multipart/form-data
        {
            std::ofstream output("http_upload.txt", std::ios::binary);
            if (!output) {
                throw std::runtime_error("Cannot create http_upload.txt");
            }

            HTTPTunneledFileTransferConnection connection(
                address, port, output, "sample.bin");
            connection.send();

            std::cout << "HTTP file upload saved to http_upload.txt\n";
        }

        std::cout << "Remote endpoint: " << address.toString() << ':' << port
            << "\nAll tests completed successfully.\n";
        return 0;
    }
    catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }
}