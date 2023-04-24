#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include "Generator.h"

using boost::asio::ip::tcp;


int main() {
  try {
    boost::asio::io_context io_context;
    tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 5000));
    auto gen = iterateNextBatch(io_context, acceptor);

    for (;;) {
      gen.next();
      for (auto v : gen.getValue()) {
        std::cout << v << std::endl;
      }
    }
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}