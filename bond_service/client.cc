#include <iostream>
#include "ServiceBond.h"
#include <string>
#include <vector>
#include <iostream>
#include <unistd.h>
#include <sstream>

using namespace std;


Bond bondBuilder(const string& name, int coupon, int years, float interest, float parValue) {
  Bond bond;
  bond.set_name(name);
  bond.set_coupon(coupon);
  bond.set_payments(years);
  bond.set_interestrate(interest);
  bond.set_parvalue(parValue);
  return bond;
}

string toString(Bond bond) {
  std::ostringstream stringStream;
  stringStream << bond.name() << " (" << bond.coupon() << ", " << bond.interestrate() << ")";
  return stringStream.str(
}

int main() {
  cout << " >>> Welcome to the bond price service <<<" << endl;
  IBondPricerService * service = new BondPricerServiceProxy("tcp://localhost:5555");

  vector<Bond> bonds = vector<Bond>();
  bonds.push_back(bondBuilder("Alitalia", 50, 20, 0.06, 1000));
  bonds.push_back(bondBuilder("Telecom", 30, 30, 0.07, 1000));
  bonds.push_back(bondBuilder("Google", 10, 50, 0.07, 1000));

  for(auto bond: bonds)
    cout << "The price of " << toString(bond) << " is " << service->reprice(bond).price() << endl;

  delete service;
  return 0;
}
