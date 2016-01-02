#include <cmath>

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
    BondPrice reprice(Bond bond) {
      std::cout << "Computing price for bond" << std::endl;
      return BondPrice();
    }
};
