//
// Created by manash on 4/23/23.
//

#ifndef TASK_GENERATOR_H
#define TASK_GENERATOR_H

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <coroutine>
#include <vector>


using boost::asio::ip::tcp;

template <typename T> struct Generator {
  struct promise_type;

  using handle_type = std::coroutine_handle<promise_type>;

  Generator(handle_type h) : coro(h) {}

  handle_type coro;

  T getValue() { return coro.promise().current_value; }

  bool next() {
    coro.resume();
    return not coro.done();
  }

  struct promise_type {
    promise_type() = default;
    ~promise_type() = default;

    auto initial_suspend() { return std::suspend_always{}; }

    auto final_suspend() noexcept { return std::suspend_always{}; }

    auto get_return_object() {
      return Generator{handle_type::from_promise(*this)};
    }

    auto return_void() { return std::suspend_never{}; }

    auto yield_value(const T value) {
      current_value = value;
      return std::suspend_always{};
    }

    void unhandled_exception() { std::exit(1); }

    T current_value;
  };
};

Generator<std::vector<std::string>>iterateNextBatch(boost::asio::io_context &context, tcp::acceptor &acceptor);

#endif // TASK_GENERATOR_H
