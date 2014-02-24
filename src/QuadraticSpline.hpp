#ifndef QUADRATIC_SPLINE_HPP
#define QUADRATIC_SPLINE_HPP

#include "Polygon.hpp"

/**
 * Subclass of Polygon that draws a quadratic spline
 */
class QuadraticSpline : public Polygon{
  const float HIGH_SMOOTHNESS = 10;
  const float LOW_SMOOTHNESS = 30;

public:
  QuadraticSpline();
  QuadraticSpline(Model* model);
  QuadraticSpline(const Component& copy);
  ~QuadraticSpline();

  virtual void draw_selected(const Point* pos) const;
  virtual Point* on_object( const unsigned int& x,
                            const unsigned int& y,
                            const bool& selected) const;
protected:
  virtual void draw_internal() const;
  bool is_smooth(const std::vector<Point>& part, const float& smoothness) const;
  void quadratic_spline(const std::vector<Point> &c_points,
                        const float& smoothness,
                        std::vector<Point>* curve) const;
};
#endif

