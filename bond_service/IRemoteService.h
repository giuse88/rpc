#ifndef __I_REMOTE_SERVICE_H_INCLUDED___
#define __I_REMOTE_SERVICE_H_INCLUDED___

#include "zmq.hpp"

class IRemoteService {
  public:
    virtual ~IRemoteService() {};
    virtual zmq::message_t processRequest(const zmq::message_t& request) = 0;
};

#endif
