#include <boost/asio.hpp>
#include <vector>
#include <thread>

// Function that sends data to the server
void sendData(const std::string& server_ip, unsigned short server_port) {
    boost::asio::io_service io_service;

    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(server_ip), server_port);

    boost::asio::ip::tcp::socket socket(io_service);
    socket.connect(endpoint);

    std::vector<char> data(1024 * 1024, 0);  // 1MB of data

    while (true) {
        boost::system::error_code error;
        socket.write_some(boost::asio::buffer(data), error);

        if (error) {
            std::cerr << "Error while sending data: " << error.message() << std::endl;
            break;
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <server_ip> <server_port> <num_threads>\n";
        return 1;
    }

    const std::string server_ip = argv[1];
    const unsigned short server_port = std::stoi(argv[2]);
    const unsigned int numThreads = std::stoi(argv[3]);

    // Create a vector of threads
    std::vector<std::thread> threads(numThreads);

    // Launch the threads
    for (auto& t : threads) {
        t = std::thread(sendData, server_ip, server_port);
    }

    // Wait for all threads to finish
    for (auto& t : threads) {
        t.join();
    }

    return 0;
}
