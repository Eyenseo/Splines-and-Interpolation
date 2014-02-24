#include "Bezier.hpp"

Bezier::Bezier() :
Polygon() {};

Bezier::Bezier(Model* model) :
Polygon(model) {};

Bezier::Bezier(const Component& copy) :
Polygon(copy) {};

Bezier::~Bezier() {};

/**
 * draw internal is used only by the component objects them self.
 * The method draws the curve
 */
void Bezier::draw_internal() const {
  std::vector<Point>* curve = new std::vector<Point>();
  bezier(*object_, HIGH_SMOOTHNESS, curve);

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
void Bezier::draw_selected(const Point* selected) const  {
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
Point* Bezier::on_object(const unsigned int& x,
                                  const unsigned int& y,
                                  const bool& selected) const {
  if(selected) { // if selected only check if on control polygon
    return Polygon::on_object(x, y, selected);
  } else {
    std::vector<Point>* curve = new std::vector<Point>();
    bezier(*object_, LOW_SMOOTHNESS, curve);  // calculate the curve with low
                                              // polygon count
    Point* ret = on_edge(x, y, *curve);

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
bool Bezier::is_smooth( const std::vector<Point>& part,
                        const float& smoothness) const {
  if(part.size() < 3) { // a line is smooth - all right?
    return true;
  }

  double total_difference = 0;
  const Point& first = *part.begin();

  Point n = *(part.end() - 1) - first;
  double buffer = n.x;
  n.x = -n.y;
  n.y = buffer;
  n *= 1 / magnitude(n);

  for(unsigned int i = 1, i_end = part.size() - 1; i < i_end; ++i) {
    // calculates the distance of the point in the middle to the line created
    // by the first point and last point
    total_difference += magnitude((part.at(i) - first) * n);
  }

  return total_difference / part.size() < smoothness;
}

/**
 * The function creates the bezier curve
 *
 * @param c_points   control points
 * @param smoothness smoothness to achieve
 *
 * @param curve      holds the smooth curve
 *
 * @param first      specifies if it is the first iteration
 *                   default value is true
 */
void Bezier::bezier(const std::vector<Point>& c_points,
                    const float& smoothness,
                    std::vector<Point>* curve,
                    const bool& first) const {
 if(first) {
    bezier(c_points, smoothness, curve, false);
    curve->push_back(*(c_points.end() - 1)); // push back last point
  } else {
    if(is_smooth(c_points, smoothness)) {
      curve->push_back(*c_points.begin()); // push back first point
    } else {
      vector_2_d* sub_points = new vector_2_d(2);

      build_table(c_points, sub_points);

      bezier(sub_points->at(0), smoothness, curve, false); // left
      bezier(sub_points->at(1), smoothness, curve, false); // right

      delete sub_points;
    }
  }
}

/**
 * The method builds the table of de Casteljau's
 *
 * @param c_points control points
 *
 * @param table    pointer to a vector_2_d with two rows
 */
void Bezier::build_table( const std::vector<Point>& c_points,
                          vector_2_d* table) const {
  vector_2_d *p = new vector_2_d();

  // initializes the first row of the table
  p->push_back(std::vector<Point>(c_points));

  build_table(p);

  // grabs top
  for(unsigned int i = 0, i_end = c_points.size(); i < i_end; ++i) {
    table->at(0).push_back(p->at(i).at(0));
  }

  // grabs diagonal
  for(unsigned int i = c_points.size() - 1; i != 0; --i) {
    table->at(1).push_back(*(p->at(i).end() - 1));
  }
  table->at(1).push_back(*(p->at(0).end() - 1)); // manually add since unsigned

  delete p;
}

/**
 * The method calculates the de Casteljau's table
 *
 * @param table pointer to a table with the first row as initial values
 */
void Bezier::build_table(vector_2_d* table) const {
  unsigned int end_i = table->at(table->size() - 1).size() - 1;
  unsigned int column = table->size();
  unsigned int prev_column = column - 1;

  table->push_back(std::vector<Point>());
  for(unsigned int i = 0; i < end_i; ++i) {
    Point& p_current = table->at(prev_column).at(i);
    Point& p_next = table->at(prev_column).at(i + 1);

    Point p_new = (p_current + p_next) / 2;

    table->at(column).push_back(p_new);
  }

  if(column != table->at(0).size()) {
    build_table(table);
  }
}
