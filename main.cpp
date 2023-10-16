#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <cstdlib>
#include <iostream>
#include <string>
#include <pthread.h>

namespace http = boost::beast::http;    // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

#define NUM_THREADS 10

void *send_request(void *threadid) {
    auto const host = "88.198.48.45";
    auto const port = "80";
    auto const target = "/";
    int version = 11;

    net::io_context ioc;

    tcp::resolver resolver(ioc);
    auto const results = resolver.resolve(host, port);

    tcp::socket socket(ioc);
    net::connect(socket, results.begin(), results.end());

    http::request<http::empty_body> req{http::verb::head, target, version};
    req.set(http::field::host, host);
    // req.set(http::field::user_agent, "My Program v1.0");

    while(true) {
        http::write(socket, req);

        boost::beast::flat_buffer buffer;

        http::response<http::dynamic_body> res;
        http::read(socket, buffer, res);

        std::cout << res << std::endl;
    }

    socket.shutdown(tcp::socket::shutdown_both);

    pthread_exit(NULL);
}

int main(void)
{
  pthread_t threads[NUM_THREADS];
  int rc;
  long t;

  for(t=0; t<NUM_THREADS; t++){
     rc = pthread_create(&threads[t], NULL, send_request, (void *)t);
     if (rc){
       printf("ERROR; return code from pthread_create() is %d\n", rc);
       exit(-1);
     }
  }

  /* Wait for all threads to complete */
  for(t=0; t<NUM_THREADS; t++){
     pthread_join(threads[t], NULL);
  }

  return EXIT_SUCCESS;
}
