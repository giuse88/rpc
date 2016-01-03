#include "ServiceContainer.h"
#include <iostream>
#include <string>

ServiceContainer::ServiceContainer(const std::string uri) {
  this->uri = uri;
}

ServiceContainer::~ServiceContainer() {
  delete this->service;
  delete this->socket;
}

void ServiceContainer::start() {
  zmq::context_t context (1);
  socket = new zmq::socket_t(context, ZMQ_REP);
  socket->bind(this->uri);
  std::cout << "Server waiting for connection on " << uri << "..." << std::endl;

  while (true) {
    zmq::message_t request;
    socket->recv(&request);
    std::cout << "Received request " << request.size() << std::endl;
    // cout << std::string(static_cast<char*>(request.data()), request.size()) << endl;
    zmq::message_t response = service->processRequest(request);
    // std::cout << std::string(static_cast<char*>(response.data()), response.size()) << std::endl;
    this->socket->send(response);
  }

}

bool ServiceContainer::registerService(IRemoteService* service) {
  this->service = service;
  return true;
}
