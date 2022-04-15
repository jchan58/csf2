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
  : m_fd(fd)
  , m_last_result(SUCCESS) {
  //call rio_readinitb to initialize the rio_t object
  rio_readinitb(&m_fdbuf, fd);
}

void Connection::connect(const std::string &hostname, int port) {
  //open_clientfd to connect to the server
  int ready_fd = open_clientfd(hostname, &((char) port));
  //call rio_readinitb to initialize the rio_t object
  rio_readinitb(&m_fdbuf, ready_fd);
}

Connection::~Connection() {
  //close the socket if it is open
  close(m_fd);
}

bool Connection::is_open() const {
  // TODO: return true if the connection is open
  return m_fd >= 0;
}

void Connection::close() {
  // TODO: close the connection if it is open
  if(m_fd >= 0) {
    //how is this different than closing the socket?
    close(m_fd);
  }
}

bool Connection::send(const Message &msg) {
  // TODO: send a message
  // return true if successful, false if not
  // make sure that m_last_result is set appropriately
  
  //send to server fd, message
  ssize_t result = rio_writen(m_fd, msg, strlen(*msg));
  m_last_result = result;
  if(result < 0){
    //success
    return false;
  } else {
    return true;
  }
  
}

bool Connection::receive(Message &msg) {
  // TODO: send a message, storing its tag and data in msg
  // return true if successful, false if not
  // make sure that m_last_result is set appropriately

  //read into message from the server fd
  ssize_t result = rio_readlineb(msg, msg->MAX_LEN, sizeof(buf));
  m_last_result = result;
  if(result < 0){
    //success
    return false;
  } else {
    return true;
  }
}
