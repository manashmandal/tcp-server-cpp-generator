//
// Created by manash on 5/12/23.
//

#ifndef TCPTASK_DATABASE_H
#define TCPTASK_DATABASE_H

#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <string>

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

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

  bsoncxx::document::value prepare_document(std::string key, double value) {
    return make_document(kvp(key, value));
  }

  std::optional<mongocxx::result::insert_one>
  insert_document(const bsoncxx::document::view &document,
                  std::string collection) {
    return this->get_collection(collection).insert_one(document);
  }

  void ping() {
    const auto ping_cmd = make_document(kvp("ping", 1));
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
