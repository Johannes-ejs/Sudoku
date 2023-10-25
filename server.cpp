#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ts/buffer.hpp>
#include <boost/asio/ts/internet.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <thread>

class Server{
    void listen();
    int send();

};