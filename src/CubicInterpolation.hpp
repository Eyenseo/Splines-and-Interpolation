#ifndef QUADRATIC_INTERPOLATION_HPP
#define QUADRATIC_INTERPOLATION_HPP

#include "Polygon.hpp"

/**
 * Subclass of Polygon that draws a quadratic interpolation
 */
class CubicInterpolation : public Polygon{
  const float HIGH_SMOOTHNESS = 10;
  const float LOW_SMOOTHNESS = 30;

public:
  CubicInterpolation();
  CubicInterpolation(Model* model);
  CubicInterpolation(const Component& copy);
  ~CubicInterpolation();

  virtual void draw_selected(const Point* pos) const;
  virtual Point* on_object( const unsigned int& x,
                            const unsigned int& y,
                            const bool& selected) const;

protected:
  virtual void draw_internal() const;
  bool is_smooth(const std::vector<Point>& part, const float& smoothness) const;
  void quadratic_interpolation( const std::vector<Point> &c_points,
                                const float& smoothness,
                                std::vector<Point>* curve) const;
};
#endif

