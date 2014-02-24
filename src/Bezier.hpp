#ifndef BEZIER_HPP
#define BEZIER_HPP

#include "Polygon.hpp"

/**
 * Subclass of Polygon that draws a bezier curve
 */
class Bezier : public Polygon {
  typedef std::vector<std::vector<Point> > vector_2_d;
  const float HIGH_SMOOTHNESS = 0.5;
  const float LOW_SMOOTHNESS = 3;

public:
  Bezier();
  Bezier(Model* model);
  Bezier(const Component& copy);
  virtual ~Bezier();

  virtual void draw_selected(const Point* pos) const;
  virtual Point* on_object( const unsigned int& x,
                            const unsigned int& y,
                            const bool& selected) const;
protected:
  virtual void draw_internal() const;
  bool is_smooth(const std::vector<Point>& part, const float& smoothness) const;
  void bezier(const std::vector<Point>& c_points,
              const float& smoothness,
              std::vector<Point>* curve,
              const bool& first = true) const;
  void build_table(const std::vector<Point>& c_points, vector_2_d* table) const;
  void build_table(vector_2_d* table) const;
};
#endif
