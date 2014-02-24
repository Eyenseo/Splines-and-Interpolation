#ifndef CUBIC_SPLINE_HPP
#define CUBIC_SPLINE_HPP

#include "Polygon.hpp"

/**
 * Subclass of Polygon that draws a cubic spline
 */
class CubicSpline : public Polygon {
  const float HIGH_SMOOTHNESS = 15;
  const float LOW_SMOOTHNESS = 40;

public:
  CubicSpline();
  CubicSpline(Model* model);
  CubicSpline(const Component& copy);
  ~CubicSpline();

  virtual void draw_selected(const Point* pos) const;
  virtual Point* on_object( const unsigned int& x,
                            const unsigned int& y,
                            const bool& selected) const;
protected:
  virtual void draw_internal() const;
  bool is_smooth(const std::vector<Point>& part, const float& smoothness) const;
  void cubic_spline(const std::vector<Point> &c_points,
                    const float& smoothness,
                    std::vector<Point>* curve) const;
};
#endif

