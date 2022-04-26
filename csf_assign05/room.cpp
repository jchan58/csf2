#include "guard.h"
#include "message.h"
#include "message_queue.h"
#include "user.h"
#include "room.h"

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
}

void Room::remove_member(User *user) {
  // remove user from collection 
}

void Room::broadcast_message(const std::string &sender_username, const std::string &message_text) {
  // TODO: send a message to every (receiver) User in the room
}
