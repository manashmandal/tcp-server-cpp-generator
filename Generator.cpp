#include "Generator.h"
#include <iostream>

std::string make_daytime_string() {
  using namespace std; // For time_t, time and ctime;
  time_t now = time(0);
  return ctime(&now);
}

Generator<std::vector<std::string>>
iterateNextBatch(boost::asio::io_context &context, tcp::acceptor &acceptor) {

  std::vector<std::string> buffer_vector;

  while (true) {
    tcp::socket socket(context);
    acceptor.accept(socket);

    // Enable TCP Keep-Alive
    boost::asio::socket_base::keep_alive option(true);
    socket.set_option(option);

    boost::array<char, 128> local_buffer;
    size_t length = socket.read_some(boost::asio::buffer(local_buffer));

    // This makes sure that the string is null terminated, otherwise
    // converting it to a number might add additional digits
    local_buffer[length] = '\0';

    boost::system::error_code ignored_error;
    std::string message = make_daytime_string();
    boost::asio::write(socket, boost::asio::buffer(message), ignored_error);

    buffer_vector.push_back(std::string(local_buffer.c_array()));

    if (buffer_vector.size() > 5) {
      co_yield buffer_vector;
      buffer_vector.clear();
    }
  }
}