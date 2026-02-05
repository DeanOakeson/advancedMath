#include <cmath>
#include <matplot/matplot.h>
#include <stdlib.h>
#include <vector>

#define PRIME_GROUP_MAG 101

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

// std::vector<double> additionOnElipticCurve(std::vector<double> p1,
//                                            std::vector<double> p2) {
//   double m = std::fmod(((p2[1] - p1[1]) / (p2[0] - p2[1])), PRIME_GROUP_MAG);
//
//   double x3 = std::fmod(((pow(m, 2)) - p1[0] - p2[0]), PRIME_GROUP_MAG);
//   double y3 = std::fmod(((m * (p1[0] - x3)) - p1[1]), PRIME_GROUP_MAG);
//
//   std::vector<double> p3 = {x3, y3};
//   return p3;
// }

bool isQuadraticResidue(int x) {
  for (int i = 0; i <= PRIME_GROUP_MAG; i++) {
    if ((i * i) % PRIME_GROUP_MAG == x)
      return true;
  }
  return false;
}

int elipticCurve(int x) {

  int A = 2;
  int B = 2;

  int y = x * x * x + A * x + B;

  return y;
}

std::vector<int> findRoots(int y) {
  std::vector<int> roots;
  for (int i = 0; i <= PRIME_GROUP_MAG; i++) {
    if ((i * i) % PRIME_GROUP_MAG == y) {
      roots.push_back(i);
    }
  }
  return roots;
}

std::vector<std::vector<int>> elipticCurveGroupGen() {

  std::vector<std::vector<int>> elipticCurveGroup;
  // walks through each x and checks for y; y determines whether the point is on
  // the group.
  for (int x = 0; x <= PRIME_GROUP_MAG; x++) {
    int y = elipticCurve(x);
    int r = y % PRIME_GROUP_MAG;
    if (r == 0) {
      elipticCurveGroup.push_back({x, 0});
    }
    if (isQuadraticResidue(r)) {
      std::vector<int> roots = findRoots(r);
      printf("%d\n", (int)roots.size());
      if (roots.size() != 0) {
        int y1 = roots[0];
        int y2 = roots[1];
        printf("y1 = %d\n", y1);
        printf("y2 = %d\n", y2);

        elipticCurveGroup.push_back({x, y1});
        elipticCurveGroup.push_back({x, y2});
      }
    }
  }
  return elipticCurveGroup;
}

int main() {
  using namespace matplot;

  // find p1 and p2 on the eliptic curve
  std::vector<double> xArr;
  std::vector<double> yArr;

  std::vector<std::vector<int>> elipticCurveGroup = elipticCurveGroupGen();

  if (elipticCurveGroup.size() == 0) {
    printf("group is empty\n");
    return 0;
  }

  for (int i = 0; i < elipticCurveGroup.size(); i++) {
    printf("SPLITTING DIMENSIONS\n");
    xArr.push_back(elipticCurveGroup[i][0]);
    yArr.push_back(elipticCurveGroup[i][1]);
  }

  auto l = matplot::scatter(xArr, yArr);
  l->marker_face(true);
  l->marker_color({1.f, 0.f, 0.f});

  show();
  return 0;
}
