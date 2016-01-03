#ifndef __BOND_PRICE_REMOTE_SERVICE_H__
#define __BOND_PRICE_REMOTE_SERVICE_H__

#include "ServiceBond.h"
#include "zmq.hpp"
#include <string>
#include <iostream>
#include <unistd.h>
#include <google/protobuf/text_format.h>

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
      std::cout << "Processing bond " << bond << std::endl;
      BondPrice bondPrice = service.reprice(bond);
      std::cout << "Bond price " << bondPrice << std::endl;

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

#endif
