#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include <math.h>
#include <vector>
#include <GL/glfw.h>
#include <GL/gl.h>
// #include <iostream>

#include "Point.hpp"
#include "Color.hpp"
#include "Model.hpp"

/**
 * component is an abstract base class for Objects that will be able to be drawn
 * by the renderer
 */

class Component {
protected:
  Model* model_;
  std::vector<Point>* object_;
  Color* color_;

public:
  Component();
  Component(Model* model);
  Component(const Component& component);
  virtual ~Component();

  virtual Point* on_object( const unsigned int& x,
                            const unsigned int& y,
                            const bool& selected) const = 0;
  virtual void draw() const = 0;
  virtual void draw_selected(const Point* selected) const = 0;
  virtual void add_Point(const unsigned int& x, const unsigned int& y);
  virtual void remove_Point(const Point& point);
  virtual void set_color(const Color& c);
  virtual Color get_color() const;
  virtual void smooth();
  Point* find_identical(Point pos);

protected:
  Point* next_point(const Point& home) const;
  Point* previous_point(const Point& home) const;
  Point* nearest_neighbour(const Point& home) const;
  virtual void draw_internal() const = 0;
  void marker(const Point& pos) const;
  void marker_selected(const Point& pos) const;
  void circle(const Point& pos,
              const unsigned int& sections,
              const unsigned int& radius) const;
  Point* nearest_point(const unsigned int& x, const unsigned int& y) const;
  double magnitude(const Point& vector) const;
  double scalar(const Point& vector_a, const Point& vector_b) const;
};
#endif
