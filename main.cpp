#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <vector>
#include <chrono>

const int numThreads = 10000;

void simulateUser() {
    const char* server_ip = "193.228.196.49";
    const int server_port = 80;

    int clientSocket = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    if (clientSocket == -1) {
        std::cerr << "Error creating socket." << std::endl;
        return;
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(server_port);
    serverAddr.sin_addr.s_addr = inet_addr(server_ip);

    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Error connecting to server." << std::endl;
        close(clientSocket);
        return;
    }

    // Simulate user behavior by keeping the connection open for a while
    std::this_thread::sleep_for(std::chrono::seconds(1));

    close(clientSocket);
}

int main() {
    std::cout << "Simulating " << numThreads << " users...\n";

    std::vector<std::thread> threads;
    for (int i = 0; i < numThreads; ++i) {
        threads.push_back(std::thread(simulateUser));
    }

    for (auto& thread : threads) {
        thread.join();
    }

    // Measure and print performance metrics here

    return 0;
}
