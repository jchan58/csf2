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
  Close(m_fd);
}

bool Connection::is_open() const {
  // TODO: return true if the connection is open
  return m_fd >= 0;
}

void Connection::close() {
  // TODO: close the connection if it is open
  if(m_fd >= 0) {
    //how is this different than closing the socket?
    Close(m_fd);
  }
}

bool Connection::send(const Message &msg) {
  // TODO: send a message
  // return true if successful, false if not
  // make sure that m_last_result is set appropriately
    
  //send to server fd, message
  //make sure that m_last_result is correct
  std::string message = msg.tag + ":" + msg.data + "\n";
  ssize_t length = message.length(); 

  ssize_t result = rio_writen(m_fd, (message).c_str(), length);
  if(result != length){
    //EOF or other err
    m_last_result = EOF_OR_ERROR;
    return false;
  } else {
        m_last_result = SUCCESS;
    return true;
  }

  //TODO: make sure that m_last_result is correct
  
}

bool Connection::receive(Message &msg) {
  // TODO: send a message, storing its tag and data in msg
  // return true if successful, false if not
  // make sure that m_last_result is set appropriately

  //read into message from the server fd
  //make sure that m_last_result is correct
  char buf[msg.MAX_LEN];


  ssize_t result = rio_readlineb(&m_fdbuf, buf, msg.MAX_LEN);
  if(result < 0){
    m_last_result = EOF_OR_ERROR;
    return false;
  } else {
    m_last_result = SUCCESS;
  }
  
  //msg.tag = "delivery";
  
  //put buf in msg
  std::string buf_cpp = buf;
  


  std::stringstream ss;
  ss << buf_cpp;

  std::string word;
  std::getline(ss, word, ':');

  msg.tag = word;

  std::getline(ss, word);

  msg.data = word;


  return true;
}
