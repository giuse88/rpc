#include <iostream>
#include "ServiceBond.h"

using namespace std;

int main() {
  cout << " >>> Welcome to the bond price service <<<" << endl;
  IBondPricerService * service = new BondPricerService();

  Bond bond("Alitalia", 50, 20, 0.06, 1000);
  cout << "The price of " << bond << " is " << service->reprice(bond) << endl;

  delete service;
  return 0;
}
