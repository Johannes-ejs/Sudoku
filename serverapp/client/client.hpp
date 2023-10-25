#ifndef BASIC_IMPORTS
#define BASIC_IMPORTS

#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ts/buffer.hpp>
#include <boost/asio/ts/internet.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <thread>

#endif

class Client{
    void listen();
    int send();
};