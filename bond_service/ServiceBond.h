#ifndef __BOND_PRICE_SERVICE_H__
#define __BOND_PRICE_SERVICE_H__

#include <cmath>
#include "BondPrice.pb.h"
#include "Bond.pb.h"
#include "zmq.hpp"
#include <string>
#include <iostream>

class IBondPricerService {
  public:
    virtual BondPrice reprice(Bond) = 0;
    virtual ~IBondPricerService() {}
};

class BondPricerService final: public IBondPricerService {

  private:
    inline float computeBondPrice(float coupon, int payments, float interestRate, float parValue) {
      float interesteRateAmount = 0;
      int i=1;
      for (; i <=payments; i++)
        interesteRateAmount += 1 /pow(1+interestRate,i);
      return coupon*interesteRateAmount + parValue/pow(1+interestRate, i);
    }

  public:
    BondPrice reprice(Bond bond) {
      float price = computeBondPrice(bond.coupon(), bond.payments(), bond.interestrate(), bond.parvalue());
      BondPrice bondPrice = BondPrice();
      bondPrice.set_price(price);
      return bondPrice;
    }
};

class BondPricerServiceProxy final: public IBondPricerService {
  private:
    std::string uri;
    zmq::socket_t* socket;
  public:

    BondPricerServiceProxy(std::string uri): uri(uri) {
    }

    BondPrice reprice(Bond bond) {
      zmq::context_t context (1);
      this->socket = new zmq::socket_t(context, ZMQ_REQ);
      //std::cout << "Connecting to bond price server…" << std::endl;
      this->socket->connect(uri);
      //std::cout << "Connected" << std::endl;


      std::string str;
      bond.SerializeToString(&str);
      int sz = str.length();
      //std::cout << str << sz << std::endl;
      zmq::message_t msg(sz);
      memcpy(msg.data(), str.c_str(), sz);
      //std::cout << std::string(static_cast<char*>(msg.data()), msg.size()) << std::endl;
      this->socket->send(msg);

      zmq::message_t reply;
      this->socket->recv(&reply);
      //std::cout << std::string(static_cast<char*>(reply.data()), reply.size()) << std::endl;
      this->socket->close();

      // deserialization
      BondPrice bondPrice = BondPrice();
      bondPrice.ParseFromArray(reply.data(), reply.size());
      return bondPrice;
    }
};

#endif
