#ifndef Point_HPP
#define Point_HPP

#include <ostream>


/**
 * Basic class for an Point or Vector - should have used set ...
 */
class Point {
 public:
   Point();
   Point(unsigned int x, unsigned int y);
   ~Point();

  const Point operator+=(const Point &rhs);
  friend Point operator+(Point lhs, const Point &rhs) {
    lhs += rhs;
    return lhs;
  }

  const Point operator+=(const double &rhs);
  friend Point operator+(Point lhs, const double& rhs) {
    lhs += rhs;
    return lhs;
  }

  const Point operator-=(const Point &rhs);
  friend Point operator-(Point lhs, const Point &rhs) {
    lhs -= rhs;
    return lhs;
  }

  const Point operator-=(const double &rhs);
  friend Point operator-(Point lhs, const double& rhs) {
    lhs -= rhs;
    return lhs;
  }

  const Point operator*=(const Point &rhs);
  friend Point operator*(Point lhs, const Point &rhs) {
    lhs *= rhs;
    return lhs;
  }

  const Point operator*=(const double &rhs);
  friend Point operator*(Point lhs, const double& rhs) {
    lhs *= rhs;
    return lhs;
  }

  const Point operator/=(const Point &rhs);
  friend Point operator/(Point lhs, const Point &rhs) {
    lhs /= rhs;
    return lhs;
  }

  const Point operator/=(const double &rhs);
  friend Point operator/(Point lhs, const double& rhs) {
    lhs /= rhs;
    return lhs;
  }

  friend std::ostream& operator<<(std::ostream& out, const Point& p) {
    out << "X: " << p.x << '\n'
    << "Y: " << p.y << '\n';

    return out;
  }

  bool operator==(const Point &rhs) const;
  bool operator!=(const Point &rhs) const;

  double x;
  double y;
};
#endif
