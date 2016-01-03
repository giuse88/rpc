#ifndef __BOND_PRICE_SERVICE_H__
#define __BOND_PRICE_SERVICE_H__

#include <cmath>
#include "ServiceBondInput.pb.h"
#include "ServiceBondOutput.pb.h"
#include "zmq.hpp"
#include <string>
#include <iostream>

class Bond {
  public:
    std::string name;
    float coupon;
    int payments;
    float interestRate;
    float parValue;

  public:
    Bond():coupon(0), payments(0), interestRate(0), parValue(0){};
    Bond(std::string name, float coupon, int payments, float interestRate, float parValue):
      name(name), coupon(coupon), payments(payments), interestRate(interestRate), parValue(parValue){};

  friend std::ostream& operator<< (std::ostream &stream, const Bond &obj);

};

std::ostream& operator<< (std::ostream &stream, const Bond &obj) {
  return stream << obj.name << " (" << obj.interestRate <<  ", " << obj.parValue << ")";
}

class BondPrice {
  public:
    float price;

  public:
    BondPrice():price(0) {};
    BondPrice(float price):price(price) {};
    friend std::ostream& operator<< (std::ostream &stream, const Bond &obj);
};

std::ostream& operator<< (std::ostream &stream, const BondPrice &obj) {
  return stream << obj.price << "$";
}

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
      return BondPrice(computeBondPrice(bond.coupon, bond.payments, bond.interestRate, bond.parValue));
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

      ServiceBondInput in;
      in.set_name(bond.name);
      in.set_coupon(bond.coupon);
      in.set_payments(bond.payments);
      in.set_interestrate(bond.interestRate);
      in.set_parvalue(bond.parValue);

      std::string str;
      in.SerializeToString(&str);
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
      ServiceBondOutput out = ServiceBondOutput();
      out.ParseFromArray(reply.data(), reply.size());
      BondPrice bondPrice = BondPrice(out.price());
      //std::cout << bondPrice << std::endl;
      return bondPrice;
    }
};

#endif
