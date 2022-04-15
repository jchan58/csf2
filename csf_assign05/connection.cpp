#include <sstream>
#include <cctype>
#include <cassert>
#include "csapp.h"
#include "message.h"
#include "connection.h"

Connection::Connection()
  : m_fd(-1)
  , m_last_result(SUCCESS) {
}

Connection::Connection(int fd)
  rio_t rio;
  : m_fd(fd)
  , m_last_result(SUCCESS) {
  //call rio_readinitb to initialize the rio_t object
  rio_readinitb(&rio, fd);
}

void Connection::connect(const std::string &hostname, int port) {
  rio_t rio;
  //open_clientfd to connect to the server
  int ready_fd = open_clientfd(hostname, (char *) &port);
  //call rio_readinitb to initialize the rio_t object
  rio_readinitb(&rio, ready_fd);
}

Connection::~Connection() {
  // TODO: close the socket if it is open
  //how to get socket fd?
}

bool Connection::is_open() const {
  // TODO: return true if the connection is open
}

void Connection::close() {
  // TODO: close the connection if it is open
}

bool Connection::send(const Message &msg) {
  // TODO: send a message
  // return true if successful, false if not
  // make sure that m_last_result is set appropriately
}

bool Connection::receive(Message &msg) {
  // TODO: send a message, storing its tag and data in msg
  // return true if successful, false if not
  // make sure that m_last_result is set appropriately
}
