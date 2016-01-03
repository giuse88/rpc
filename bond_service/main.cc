#include <iostream>
#include "ServiceBond.h"
#include "zmq.hpp"
#include <string>
#include <iostream>
#include <unistd.h>
#include <google/protobuf/text_format.h>

using namespace std;

class IRemoteService {
  public:
    virtual ~IRemoteService() {};
    virtual zmq::message_t processRequest(const zmq::message_t& request) = 0;
};

class ServiceContainer {
  private:
    string uri;
    IRemoteService* service = nullptr;
    zmq::socket_t *socket = nullptr;

  public:
    ServiceContainer(const string uri);
    ~ServiceContainer();
    void start();
    bool registerService(IRemoteService* service);
};

ServiceContainer::ServiceContainer(const string uri) {
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
  cout << "Server waiting for connection on " << uri << "..." << endl;

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

class BondPriceRemoteService: public IRemoteService {
  private:
    BondPricerService service;

  public:
    zmq::message_t processRequest(const zmq::message_t& req) {
      // deserialization
      ServiceBondInput in = ServiceBondInput();
      in.ParseFromArray(req.data(), req.size());
      google::protobuf::TextFormat::Printer p;
  //    std::string s;
  //    p.PrintToString(in, &s);
  //    cout << "Received " << s << endl;
      Bond bond;
      bond.name = in.name();
      bond.coupon = in.coupon();
      bond.payments = in.payments();
      bond.interestRate = in.interestrate();
      bond.parValue = in.parvalue();

      // call real serivce
      cout << "Processing bond " << bond << endl;
      BondPrice bondPrice = service.reprice(bond);
      cout << "Bond price " << bondPrice << endl;

      // serializion of the response
      ServiceBondOutput out;
      out.set_price(bondPrice.price);
      std::string str;
      out.SerializeToString(&str);
      int sz = str.length();
      zmq::message_t response(sz);
      memcpy(response.data(), str.c_str(), sz);
      return response;
    }
};

int main() {
  cout << " >>> Welcome to the bond price server<<<" << endl;
  IBondPricerService * service = new BondPricerService();

  //Bond bond("Alitalia", 50, 20, 0.06, 1000);
  //  cout << "The price of " << bond << " is " << service->reprice(bond) << endl;

  ServiceContainer container("tcp://*:5555");
  container.registerService(new BondPriceRemoteService());
  container.start();

  delete service;
  return 0;
}
