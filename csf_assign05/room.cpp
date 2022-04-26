#include "guard.h"
#include "message.h"
#include "message_queue.h"
#include "user.h"
#include "room.h"
#include <iterator>
#include <set>
#include <iostream>

using std::iterator;
using std::set; 

Room::Room(const std::string &room_name)
  : room_name(room_name) {
  //initialize the mutex
  pthread_mutex_init(&lock, NULL);
}

Room::~Room() {
   //destroy mutex
   pthread_mutex_destroy(lock);
}

void Room::add_member(User *user) {
  // TODO: add User to the room
  members.insert(user); 
}

void Room::remove_member(User *user) {
  // remove user from collection
  members.erase(user); 
}

void Room::broadcast_message(const std::string &sender_username, const std::string &message_text) {
  // TODO: send a message to every (receiver) User in the room
  std::set<User *>::iterator it;
  for(it = members.begin(); it != members.end(); ++it){
    if(*(it)->sender == false && strcmp(*(it)->username, sender_username) != 0){
      *(it)->mqueue.enqueue(message_text); 
    }
  }
}

Message& Room::take_message() {
  // take the message out of every reciever's message queue in the room
  std::set<User *>::iterator it;
  for(it = members.begin(); it != members.end(); ++it){
    if(*(it)->sender == false){
      *(it)->mqueue.deque();
    }
  }
}
