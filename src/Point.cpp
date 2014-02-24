#include "Point.hpp"

Point::Point() :
x(0),
y(0) {};

Point::Point(unsigned int x, unsigned int y) :
x(x),
y(y) {};

Point::~Point() {}

const Point Point::operator+=(const Point &rhs) {
  x += rhs.x;
  y += rhs.y;

  return *this;
}

const Point Point::operator+=(const double &rhs) {
  x += rhs;
  y += rhs;
  return *this;
}

const Point Point::operator-=(const Point &rhs) {
  x -= rhs.x;
  y -= rhs.y;

  return *this;
}

const Point Point::operator-=(const double &rhs) {
  x -= rhs;
  y -= rhs;
  return *this;
}

const Point Point::operator*=(const Point &rhs) {
  x *= rhs.x;
  y *= rhs.y;

  return *this;
}

const Point Point::operator*=(const double &rhs) {
  x *= rhs;
  y *= rhs;
  return *this;
}

const Point Point::operator/=(const Point &rhs) {
  x /= rhs.x;
  y /= rhs.y;

  return *this;
}

const Point Point::operator/=(const double &rhs) {
  x /= rhs;
  y /= rhs;
  return *this;
}

bool Point::operator==(const Point &rhs) const {
  return (int)x == (int)rhs.x && (int)y == (int)rhs.y;
}

bool Point::operator!=(const Point &rhs) const {
  return !(*this == rhs);
}
