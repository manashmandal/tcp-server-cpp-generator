#include <boost/asio.hpp>
#include <iostream>
#include <numeric>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>

#include "Database.h"
#include "Generator.h"

using boost::asio::ip::tcp;

void compute_mean_odd_only(std::vector<std::string> inputs,
                           Database &database) {
  std::cout << "Begin calculation " << std::endl;
  try {
    auto results = inputs | std::views::transform([](std::string value) {
                     return std::stoi(value);
                   }) |
                   std::views::filter([](int n) { return n % 2 != 0; });

    int sum = std::accumulate(results.begin(), results.end(), 0);
    int size = std::distance(results.begin(), results.end());

    double mean = static_cast<double>(sum) / size;
    database.insert_document(database.prepare_document("mean", mean).view(),
                             "computed_mean");
    std::cout << "Mean: " << mean << std::endl;

  } catch (std::exception &e) {
    std::cout << "Exception: " << e.what() << std::endl;
  }
  std::cout << "End calculation " << std::endl;
}

int main() {
  std::string connection_uri{"mongodb://localhost:27020"};
  std::string database_name{"data_engineering_in_cpp_with_manash"};

  Database database(connection_uri, database_name);
  database.ping();

  try {
    boost::asio::io_context io_context;
    tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 5000));
    auto gen = iterateNextBatch(io_context, acceptor);

    for (;;) {
      gen.next();
      compute_mean_odd_only(gen.getValue(), database);
    }
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}