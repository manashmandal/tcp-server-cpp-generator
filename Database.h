//
// Created by manash on 5/12/23.
//

#ifndef TCPTASK_DATABASE_H
#define TCPTASK_DATABASE_H

#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <string>

class Database {
public:
  Database(std::string connection_string, std::string database_name)
      : database_name(database_name) {
    this->connection_string = static_cast<mongocxx::uri>(connection_string);
    mongocxx::client client{this->connection_string};
    this->client_ = std::move(client);
    db = this->client_[database_name];
  }

  mongocxx::collection get_collection(std::string collection_name) {
    return this->db[collection_name];
  }

  void ping() {
    const auto ping_cmd = bsoncxx::builder::basic::make_document(
        bsoncxx::builder::basic::kvp("ping", 1));
    try {
      this->db.run_command(ping_cmd.view());
      std::cout << "Database connection established successfully" << std::endl;
    } catch (const std::exception &e) {
      std::cout << "Exception: " << e.what() << std::endl;
    }
  }

private:
  mongocxx::uri connection_string;
  mongocxx::client client_;
  mongocxx::database db;
  std::string database_name;
};

#endif // TCPTASK_DATABASE_H
