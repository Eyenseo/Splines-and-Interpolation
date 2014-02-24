#include "Component.hpp"

Component::Component() :
model_(NULL),
object_(new std::vector<Point>()),
color_(new Color()) {}

Component::Component(Model* model) :
model_(model),
object_(new std::vector<Point>()) {
  auto rand_float =  []() -> float{return (rand() % 100 + 1) / 100.0;};

  color_ = new Color(rand_float(), rand_float(), rand_float());
}

Component::Component(const Component& copy) :
model_(copy.model_),
object_(new std::vector<Point>(*copy.object_)),
color_(new Color(*copy.color_)) {}

Component::~Component() {
  delete object_;
  delete color_;
}

/**
 * The method returns an identical (might also return the same one with the same
 * memory address) point
 *
 * @param  pos Point to compare to
 *
 * @return     identical point - might be the same eg. same memory address.
 */
Point* Component::find_identical(Point pos) {
  for(std::vector<Point>::iterator i = object_->begin();
      i != object_->end(); ++i) {
    if(*i == pos) {
      return &*i;
    }
  }
  return NULL;
}

/**
 * The method draws a marker. A circle in a specific color at the Position of
 * the Point
 *
 * @param pos Position the marker will be drawn
 */
void Component::marker(const Point& pos) const {
  Color c = model_->get_selection_color();
  glColor3f(c.get_red(), c.get_green(), c.get_blue());
  circle(pos, model_->get_marker_sections(), model_->get_marker_radius());
}

/**
 * The method draws a marker. A circle in the "selected" color at the Position
 * of the Point
 *
 * @param pos Position the marker will be drawn
 */
void Component::marker_selected(const Point& pos) const {
  Color c = model_->get_selected_point_color();
  glColor3f(c.get_red(), c.get_green(), c.get_blue());
  circle(pos, model_->get_marker_sections(), model_->get_marker_radius());
}

/**
 * The method will draw a circle with the amount of sections and radius
 *
 * @param pos      Position the circle will be drawn at
 * @param sections sections of the circle
 * @param radius   radius of the circle
 */
void Component::circle( const Point& pos,
                        const unsigned int& sections,
                        const unsigned int& radius) const {
  glBegin(GL_LINE_STRIP);
  for(unsigned char i = 0; i <= sections; ++i) {
    float t = 2 * M_PI * (float)i / sections;
    glVertex2f(pos.x + sin(t) * radius, pos.y + cos(t) * radius);
  }
  glEnd();
}

void Component::smooth() {
  if(object_->size() >= 3) {
    Point prev_selected = *model_->get_selected_point();
    std::vector<Point>* ret = new std::vector<Point>();

    for(long int i = 0, i_end = object_->size(); i < i_end; ++i) {
      Point p;

      // See Savitzky–Golay filter
      //
      // Switched to different algorithm since -2/64, 4/64, 58/64, 4/64, -2/64
      // kept the points moving
      if(i > 1 && i < i_end - 2) {
        p = (   object_->at(i - 2) * -3
              + object_->at(i - 1) * 12
              + object_->at(i)     * 17
              + object_->at(i + 1) * 12
              + object_->at(i + 2) * -3)
            * 1 / 35;
      } else if(i > 0 && i < i_end - 1) {
        p = (   object_->at(i - 1) * -3
              + object_->at(i - 1) * 12
              + object_->at(i)     * 17
              + object_->at(i + 1) * 12
              + object_->at(i + 1) * -3)
            * 1 / 35;
      } else {
        p = object_->at(i);
      }

      ret->push_back(p);
    }

    // set "new" selected point
    for(unsigned int i = 0; i < object_->size(); ++i) {
      if(prev_selected == object_->at(i)) {
        model_->set_selected_point(&ret->at(i));
        i = object_->size();
      }
    }

    delete object_;
    object_ = ret;

    model_->set_redraw(true);
  }
}

void Component::set_color(const Color& c) {
  delete color_;
  color_ = new Color(c);
  model_->set_redraw(true);
}

Color Component::get_color() const {
  return *color_;
}

/**
 * The method finds the nearest point to the point specified by x and y
 *
 * @param  x x position of the point to be checked
 * @param  y y position of the point to be checked
 *
 * @return   nearest point
 */
Point* Component::nearest_point(const unsigned int& x,
                                const unsigned int& y) const {
  unsigned int dist, dist_prev = 0;
  Point* ret = NULL;
  Point p = Point(x, y);

  for(std::vector<Point>::iterator i = object_->begin();
      i != object_->end(); ++i) {
    dist = magnitude(p - *i);

    if(dist < dist_prev || ret == NULL) {
      dist_prev = dist;
      ret = &*i;
    }
  }
  return ret;
}


/**
 * magnitude / length of the "vector"
 *
 * @param  vector the length shall be calculated from
 *
 * @return        magnitude / length of the vector
 */
double Component::magnitude(const Point& vector) const {
  return sqrt((vector.x * vector.x) + (vector.y * vector.y));
}

/**
 * The methods calculates the scalar of the two "vectors"
 *
 * @param  vector_a
 * @param  vector_b
 *
 * @return          scalar of the two vectors
 */
double Component::scalar(const Point& vector_a, const Point& vector_b) const {
  return vector_a.x * vector_b.x + vector_a.y * vector_b.y;
}


/**
 * The method finds and returns the nearest neighbour
 *
 * @param  home point that wants to find it's neighbour
 *
 * @return      nearest neighbour point
 */
Point* Component::nearest_neighbour(const Point& home) const {
  std::vector<Point>::iterator i_start = object_->begin();
  std::vector<Point>::iterator i_end = object_->end();
  Point* ret = NULL;

  for(std::vector<Point>::iterator i = i_start; i != i_end; ++i) {
    if(home == *i) {
      if(i == i_start) {
        ret = &*(i + 1);
      } else if(i == i_end - 1) {
        ret = &*(i - 1);
      } else if(magnitude(home - *(i - 1)) < magnitude(home - *(i + 1))) {
        ret = &*(i - 1);
      } else {
        ret = &*(i + 1);  // The address is taken from the Vector not from the
                          // Point - when deleting the object at i, i + 1 will
                          // be i + 2 !!
      }
      i = i_end - 1;
    }
  }
  return ret;
}

/**
 * The method finds and returns the next point
 *
 * @param  home point that wants to find it's follow-up 
 *
 * @return      next point
 */
Point* Component::next_point(const Point& home) const {
  std::vector<Point>::iterator i_start = object_->begin();
  std::vector<Point>::iterator i_end = object_->end();
  Point* ret = NULL;

  for(std::vector<Point>::iterator i = i_start; i != i_end; ++i) {
    if(home == *i) {
      if(i != i_end - 1) {
        ret = &*(i + 1);
      }
      i = i_end - 1;
    }
  }
  return ret;
}

/**
 * The method finds and returns the previous point
 *
 * @param  home point that wants to find it's forerunner
 *
 * @return      previous point
 */
Point* Component::previous_point(const Point& home) const {
  std::vector<Point>::iterator i_start = object_->begin();
  std::vector<Point>::iterator i_end = object_->end();
  Point* ret = NULL;

  for(std::vector<Point>::iterator i = i_start; i != i_end; ++i) {
    if(home == *i) {
      if(i != i_start) {
        ret = &*(i - 1);
      }
      i = i_end - 1 ;
    }
  }
  return ret;
}

/**
 * The method adds a point to the Component
 *
 * @param x x position of the point to be added
 * @param y y position of the point to be added
 */
void Component::add_Point(const unsigned int& x, const unsigned int& y) {
  std::vector<Point>::iterator i_start = object_->begin();
  std::vector<Point>::iterator i_end = object_->end();
  Point* selected_Point = model_->get_selected_point();
  Point p;
  p.x = x;
  p.y = y;

  // if start or end or new Component add to the end of the vector
  if(    selected_Point == NULL
      || selected_Point == &*i_start
      || selected_Point == &*(i_end - 1)) {
    object_->push_back(p);
  } else {
    // else add the point to the vector at the nearest position to the selected
    // point
    for(std::vector<Point>::iterator i = i_start; i != i_end; ++i) {
      if(&*i == selected_Point) {
        if(magnitude(p - *(i - 1)) < magnitude(p - *(i + 1))) {
          object_->insert(i, p);
          break;
        } else {
          object_->insert(i + 1, p);
          break;
        }
      }
    }
  }
  model_->set_redraw(true);
};

/**
 * The method removes a Point from the Component
 *
 * @param pos
 */
void Component::remove_Point(const Point& pos) {
  Point* p = NULL;

  if(next_point(pos) != (p = nearest_neighbour(pos))) {
    model_->set_selected_point(p);
  }

  for(std::vector<Point>::iterator i = object_->begin();
      i != object_->end(); ++i) {
    if(*i == pos) {
      object_->erase(i);
      break;
    }
  }
  model_->set_redraw(true);
}

