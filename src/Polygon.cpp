#include "Polygon.hpp"

Polygon::Polygon() :
Component(){}

Polygon::Polygon(Model* model) :
Component(model) {}

Polygon::Polygon(const Component& copy) :
Component(copy) {}

Polygon::~Polygon() {}
/**
 * draw internal is used only by the component objects them self.
 * The method draws the curve
 */
void Polygon::draw_internal() const {
  Point* pos;

  glBegin(GL_LINE_STRIP);

  for(unsigned int i = 0; i < object_->size(); i++) {
    pos = &(object_->at(i));
    glVertex2f(pos->x, pos->y);
  }

  glEnd();
}

/**
 * Draws the Polygon in its color by calling the draw internal method
 */
void Polygon::draw() const  {
  glColor3f(color_->get_red(), color_->get_green(), color_->get_blue());
  draw_internal();
}

/**
 * The method draws the selected polygon with markers
 *
 * @param selected selected point that will be drawn differently
 */
void Polygon::draw_selected(const Point* pos) const  {
  glColor3f(color_->get_red(), color_->get_green(), color_->get_blue());

  draw_internal();

  //draw markers
  for(std::vector<Point>::iterator i = object_->begin();
      i != object_->end(); ++i) {
    if(pos != NULL && pos == &*i){
      marker_selected(*i);
    } else {
      marker(*i);
    }
  }
}

/**
 * Method to check if the point specified, is on the object / on a point of the
 * polygon
 *
 * @param  x        x position of the point to be checked
 * @param  y        y position of the point to be checked
 * @param  selected true if the object is selected
 *
 * @return          point 'under' x and y or NULL
 */
Point* Polygon::on_object(const unsigned int& x,
                          const unsigned int& y,
                          const bool& selected) const {
  Point* ret = on_point(x, y, *object_);

  if(!selected && ret == NULL) {
    ret = on_edge(x, y, *object_);
  }
  return ret;
}

/**
 * Method checks if the point specified by x and y is on a point
 *
 * @param  x        x position of the point to be checked
 * @param  y        y position of the point to be checked
 * @param  selected true if the object is selected
 *
 * @return          point 'under' x and y or NULL
 */
Point* Polygon::on_point( const unsigned int& x,
                          const unsigned int& y,
                          std::vector<Point>& poly) const {
  unsigned int x_diff, y_diff, vol, radius;
  radius = model_->get_marker_radius();
  vol = radius * radius;

  for(std::vector<Point>::iterator i = poly.begin();
      i != poly.end(); ++i) {
    x_diff = x - i->x;
    y_diff = y - i->y;

    // checks if x and y is in the 'circle'
    if((x_diff * x_diff + y_diff * y_diff) <= vol) {
      return (&*i);
    }
  }
  return NULL;
}

/**
 * Method checks if the point specified by x and y is on an edge
 *
 * @param  x        x position of the point to be checked
 * @param  y        y position of the point to be checked
 * @param  selected true if the object is selected
 *
 * @return          point nearest to x&y or NULL
 */
Point* Polygon::on_edge(const unsigned int& x,
                        const unsigned int& y,
                        std::vector<Point>& poly) const {
  unsigned int radius = model_->get_marker_radius();

  for(std::vector<Point>::iterator i = poly.begin() + 1;
      i != poly.end(); ++i) {
    Point n = *i - *(i - 1);
    double buffer =  n.x;
    n.x = - 1 * n.y;
    n.y = buffer;
    n = n * (1 / magnitude(n)); // calculate nominal normal vector

    //calculate box around curve segments
    Point lt = *(i - 1) + n * radius;
    Point lb = *(i - 1) - n * radius;
    Point rt = *i + n * radius;
    Point rb = *i - n * radius;

    if(is_in_rectangle(x, y, lb, lt, rt, rb)) {
      return nearest_point(x, y);
    }
  }
  return NULL;
}
/**
 * the method checks if the point specified by x and y is in the rectangle
 * specified by left_bottom, left_top, right_top, right_bottom
 *
 * mathematics taken from http://stackoverflow.com/questions/2752725/finding-whether-a-point-lies-inside-a-rectangle-or-not
 *
 * @param  x            x position of the point to be checked
 * @param  y            y position of the point to be checked
 * @param  left_bottom  left bottom point of the rectangle
 * @param  left_top     left top point of the rectangle
 * @param  right_top    right top point of the rectangle
 * @param  right_bottom right bottom point of the rectangle
 *
 * @return              returns true if the point is in the rectangle
 */
bool Polygon::is_in_rectangle(const unsigned int& x,
                              const unsigned int& y,
                              const Point& left_bottom,
                              const Point& left_top,
                              const Point& right_top,
                              const Point& right_bottom) const {
  auto is_right = [&x, &y](const Point& left, const Point& right) -> bool {
    auto A = -(right.y - left.y);
    auto B =  right.x - left.x;
    auto C = -(A * left.x + B * left.y);

    return  (A * x + B * y + C) < 0;
  };

  return is_right(left_bottom, left_top)
      && is_right(left_top, right_top)
      && is_right(right_top, right_bottom)
      && is_right(right_bottom, left_bottom);
}
