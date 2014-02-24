#include "CubicSpline.hpp"

CubicSpline::CubicSpline() :
Polygon() {};

CubicSpline::CubicSpline(Model* model) :
Polygon(model) {};

CubicSpline::CubicSpline(const Component& copy) :
Polygon(copy) {};

CubicSpline::~CubicSpline() {};

/**
 * draw internal is used only by the component objects them self.
 * The method draws the curve
 */
void CubicSpline::draw_internal() const {
  std::vector<Point>* curve = new std::vector<Point>();
  cubic_spline(*object_, HIGH_SMOOTHNESS, curve);

  // std::cout << "Points: " <<  curve->size() << std::endl;

  glBegin(GL_LINE_STRIP);

  for(std::vector<Point>::iterator i = curve->begin(); i != curve->end(); ++i) {
    glVertex2f(i->x, i->y);
  }

  glEnd();

  delete curve;
}

/**
 * The method draws the selected curve with the polygon as an outline with
 * markers at the control points
 *
 * @param selected selected point that will be drawn differently
 */
void CubicSpline::draw_selected(const Point* selected) const  {
  Color outline = model_->get_outline_color();

  glColor3f(outline.get_red(), outline.get_green(), outline.get_blue());
  Polygon::draw_internal(); // draw outline

  glColor3f(color_->get_red(), color_->get_green(), color_->get_blue());
  draw_internal(); // draw curve

  //draw markers
  for(std::vector<Point>::iterator i = object_->begin();
      i != object_->end(); ++i) {
    if(selected != NULL && selected == &*i){
      marker_selected(*i);
    } else {
      marker(*i);
    }
  }
}

/**
 * Method to check if the point specified, is on the object / on a point of the
 * curve
 *
 * @param  x        x position of the point to be checked
 * @param  y        y position of the point to be checked
 * @param  selected true if the object is selected
 * @return          point 'under' x and y or NULL
 */
Point* CubicSpline::on_object(const unsigned int& x,
                              const unsigned int& y,
                              const bool& selected) const {
  if(selected) { // if selected only check if on control polygon
    return Polygon::on_object(x, y, selected);
  } else {
    std::vector<Point>* curve = new std::vector<Point>();
    cubic_spline(*object_, LOW_SMOOTHNESS, curve);  // calculate the curve with
                                                    // low polygon count
    Point* ret  =  on_edge(x, y, *curve);

    delete curve;
    return ret;
  }
  return NULL;
}

/**
 * Method checks if the curve is smooth enough
 *
 * @param  part       part to check
 * @param  smoothness value that defines the smoothness /
 *                    distance of middle point
 * @return            true if smooth enough
 */
bool CubicSpline::is_smooth(const std::vector<Point>& part,
                                const float& smoothness) const {
  if(part.size() < 3) { // a line is smooth - all right?
    return true;
  }

  for(unsigned int i = 1, i_end = part.size() - 1; i < i_end; ++i) {
    const Point& first = part.at(i - 1);

    Point n = *(part.end() - 1) - first;
    double buffer = n.x;
    n.x = -n.y;
    n.y = buffer;
    n *= 1 / magnitude(n);

    // calculates the distance of the point in the middle to the line created
    // by the point before and after
    if(magnitude((part.at(i) - first) * n) > smoothness) {
      return false;
    }
  }
  return true;
}


/**
 * The function creates the quadratic spline curve
 *
 * @param c_points   control points
 * @param smoothness smoothness to achieve

 * @param curve      holds the smooth curve
 */
void CubicSpline::cubic_spline( const std::vector<Point>& c_points,
                                const float& smoothness,
                                std::vector<Point>* curve) const {
  if(is_smooth(c_points, smoothness)) {
    for(unsigned int i = 0, i_end = c_points.size(); i < i_end; ++i) {
      curve->push_back(c_points.at(i));
    }
  } else {
    std::vector<Point>* sub_points = new std::vector<Point>();

    sub_points->push_back(*c_points.begin());

    for(int i = 0, i_end = c_points.size() -1; i < i_end; ++i) {
      sub_points->push_back(c_points.at(i) * 1/2 + c_points.at(i+1) * 1/2);
      if(i < i_end - 1) {
        sub_points->push_back(c_points.at(i) * 1/8
                              + c_points.at(i+1) * 6/8
                              + c_points.at(i+2) * 1/8);
      }
    }

    sub_points->push_back(*(c_points.end()-1));

    cubic_spline(*sub_points, smoothness, curve);

    delete sub_points;
  }
}
