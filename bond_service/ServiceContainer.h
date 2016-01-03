#ifndef __SERVICE_CONTAINER_H_INCLUDED__
#define __SERVICE_CONTAINER_H_INCLUDED__

#include "IRemoteService.h"
#include "zmq.hpp"

class ServiceContainer {
  private:
    std::string uri;
    IRemoteService* service = nullptr;
    zmq::socket_t *socket = nullptr;

  public:
    ServiceContainer(const std::string uri);
    ~ServiceContainer();
    void start();
    bool registerService(IRemoteService* service);
};

#endif
