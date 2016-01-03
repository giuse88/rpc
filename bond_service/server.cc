#include <iostream>
#include "ServiceContainer.h"
#include "BondPriceRemoteService.h"

using namespace std;

int main() {
  cout << " >>> Welcome to the bond price server <<<" << endl;

  ServiceContainer container("tcp://*:5555");
  container.registerService(new BondPriceRemoteService());
  container.start();

  return 0;
}
