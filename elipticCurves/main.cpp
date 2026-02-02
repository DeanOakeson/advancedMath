#include <cmath>
#include <iostream>
#include <matplot/matplot.h>
#include <random>
#include <stdlib.h>
#include <vector>

#define PRIME_GROUP_MAG 1000

// y^2 = x^3 +Ax + B mod p = eliptic curve group

// miller-rabin function
// double millerRabin() { return prime; }
//
// // generates the primes and the coprime needed to begin the algorithm
// std::vector<int> genModulus() {
//
//   // genModulus function here
//   //
//   return (N, p, q);
// }
//
// // Eulers Totient
// double ΦN(int N) { return ΦN; }
//
// double eGen(int ΦN) { return e; }

std::vector<double> additionOnElipticCurve(std::vector<double> p1,
                                           std::vector<double> p2) {
  double m = std::fmod(((p2[1] - p1[1]) / (p2[0] - p2[1])), PRIME_GROUP_MAG);

  double x3 = std::fmod(((pow(m, 2)) - p1[0] - p2[0]), PRIME_GROUP_MAG);
  double y3 = std::fmod(((m * (p1[0] - x3)) - p1[1]), PRIME_GROUP_MAG);

  std::vector<double> p3 = {x3, y3};
  return p3;
}

std::vector<std::vector<double>> elipticCurveGroupGen(std::vector<double> p1,
                                                      std::vector<double> p2) {
  std::vector<std::vector<double>> elipticCurveGroup;
  std::vector<double> p3 = additionOnElipticCurve(p1, p2);
  elipticCurveGroup.push_back(p3);
  for (int i = 0; i <= PRIME_GROUP_MAG; i++) {
    p3 = additionOnElipticCurve(p1, p3);
    elipticCurveGroup.push_back(p3);
  }
  return elipticCurveGroup;
}

int main() {
  using namespace matplot;

  // find p1 and p2 on the eliptic curve

  std::vector<double> p1 = {-1.32472, 0};
  std::vector<double> p2 = {0, 1};

  std::vector<double> xArr;
  std::vector<double> yArr;

  std::vector<std::vector<double>> elipticCurveGroup =
      elipticCurveGroupGen(p1, p2);

  for (int i = 0; i <= PRIME_GROUP_MAG; i++) {
    xArr.push_back(elipticCurveGroup[i][0]);
    yArr.push_back(elipticCurveGroup[i][1]);
  }

  auto color = linspace(1.0, 0.0, 0.0);

  auto l = plot(xArr, yArr, color);
  show();
  return 0;
}
