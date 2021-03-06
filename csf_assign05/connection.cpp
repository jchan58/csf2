#include <sstream>
#include <cctype>
#include <cassert>
#include "csapp.h"
#include "message.h"
#include "connection.h"

#include <unistd.h>
#include <sstream>
#include <iostream>


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
  //convert port to a string
  std::stringstream ss;
  ss << port;
  std::string str_port = ss.str();

  //open_clientfd to connect to the server
  m_fd = open_clientfd(hostname.c_str(), str_port.c_str());

  //call rio_readinitb to initialize the rio_t object
  rio_readinitb(&m_fdbuf, m_fd);
}

Connection::~Connection() {
  //close the socket if it is open
  Connection::close();
}

bool Connection::is_open() const {
  //return true if the connection is open
  return m_fd >= 0;
}

void Connection::close() {
  //close the connection if it is open
  if(m_fd >= 0) {
    Close(m_fd);
    m_fd = -1;
  }
}

bool Connection::send(const Message &msg) {
  // return true if successful, false if not
  // make sure that m_last_result is set appropriately
    
  //convert the message to a string
  std::string message = msg.tag + ":" + msg.data + "\n";
  ssize_t length = message.length(); 

  //send the message to the server
  ssize_t result = rio_writen(m_fd, message.c_str(), length);
  
  if(result != length){
    //EOF or other error
    m_last_result = EOF_OR_ERROR;
    return false;
  } else {
    m_last_result = SUCCESS;
    return true;
  }
 
}

bool Connection::receive(Message &msg) {
  // return true if successful, false if not
  // make sure that m_last_result is set appropriately

  char buf[msg.MAX_LEN];

  //receive a message from the server and write it into the buffer
  ssize_t result = rio_readlineb(&m_fdbuf, buf, msg.MAX_LEN);

  if(result < 0){
    m_last_result = EOF_OR_ERROR;
    return false;
  } else {
    m_last_result = SUCCESS;
  }
  
  //convert the contents in the buffer into a stringstream
  std::string buf_cpp = buf;
  std::stringstream ss;
  ss << buf_cpp;

  //store the tag and payload properly into the message
  std::string word;
  std::getline(ss, word, ':');

  //invalid message
  if(strcmp(word.c_str(), ss.str().c_str()) == 0){
    m_last_result = EOF_OR_ERROR;
    return false;
  }
  
  msg.tag = word;

  std::getline(ss, word);
  msg.data = word;

  return true;
}
