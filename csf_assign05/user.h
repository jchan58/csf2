#ifndef USER_H
#define USER_H

#include <string>
#include "message_queue.h"
#include "room.h"

struct User {
  std::string username;
  Room *room; 

  // queue of pending messages awaiting delivery
  MessageQueue mqueue;
  bool sender = false; 

  User(const std::string &username) : username(username) { }
};

#endif // USER_H
