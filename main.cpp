#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include "Generator.h"
#include "Database.h"

using boost::asio::ip::tcp;

void compute_windowed_mean(std::vector<std::string> inputs, Database &database){

}


int main() {
  std::string connection_uri {"mongodb://localhost:27020"};
  std::string database_name {"data_engineering_in_cpp_with_manash"};

  Database database(connection_uri, database_name);
  database.ping();


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