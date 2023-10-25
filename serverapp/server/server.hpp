#ifndef BASIC_IMPORTS
#define BASIC_IMPORTS

#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ts/buffer.hpp>
#include <boost/asio/ts/internet.hpp>
#include <boost/asio/error.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <thread>

namespace asio = boost::asio;
namespace ip = asio::ip;
namespace error_code = asio::error;
using tcp = ip::tcp;

#endif

class Server{
    void listen();
    int send();
};