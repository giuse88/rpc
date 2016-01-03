#include <iostream>
#include "ServiceBond.h"
#include <string>
#include <vector>
#include <iostream>
#include <unistd.h>

using namespace std;

int main() {
  cout << " >>> Welcome to the bond price service <<<" << endl;
  IBondPricerService * service = new BondPricerServiceProxy("tcp://localhost:5555");

  vector<Bond> bonds = vector<Bond>();
  bonds.push_back(Bond("Alitalia", 50, 20, 0.06, 1000));
  bonds.push_back(Bond("Telecom", 30, 30, 0.07, 1000));
  bonds.push_back(Bond("Google", 10, 50, 0.07, 1000));

  for(auto bond: bonds)
    cout << "The price of " << bond << " is " << service->reprice(bond) << endl;

  delete service;
  return 0;
}
