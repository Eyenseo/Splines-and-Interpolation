#ifndef POLYGON_HPP
#define POLYGON_HPP

#include "Component.hpp"

/**
 * Polygon is a subclass of Component that implements all methods of Component
 * and is so able to draw itself as a basic polygon
 */
class Polygon : public Component {
public:
  Polygon();
  Polygon(Model* model);
  Polygon(const Component& copy);
  ~Polygon();

  virtual void draw() const;
  virtual void draw_selected(const Point* selected) const;
  virtual Point* on_object( const unsigned int& x,
                            const unsigned int& y,
                            const bool& selected) const;
protected:
  virtual void draw_internal() const;
  Point* on_point(const unsigned int& x,
                  const unsigned int& y,
                  std::vector<Point>& poly) const;
  Point* on_edge( const unsigned int& x,
                  const unsigned int& y,
                  std::vector<Point>& poly) const;
  bool is_in_rectangle( const unsigned int& x,
                        const unsigned int& y,
                        const Point& left_bottom,
                        const Point& left_top,
                        const Point& right_top,
                        const Point& right_bottom) const;
};
#endif

