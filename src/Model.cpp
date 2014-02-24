#include "Model.hpp"
#include "Component.hpp"

Model::Model() :
rendering_(false),
redraw_(false),
running_(false),
window_width_(0),
window_height_(0),
marker_readius_(10),
marker_sections_(10),
selected_(NULL),
selected_point_(NULL),
selection_color_(new Color(1, 0.6, 0)),
selected_point_color_(new Color(1, 0.4, 0.4)),
outline_color_(new Color(0.6, 0.6, 1)),
components_(new std::vector<Component*>()){}

Model::Model(const unsigned int& width, const unsigned int& height) :
rendering_(false),
redraw_(true),
running_(true),
window_width_(width),
window_height_(height),
marker_readius_(10),
marker_sections_(10),
selected_(NULL),
selected_point_(NULL),
selection_color_(new Color(1, 0.6, 0.0)),
selected_point_color_(new Color(1, 0.4, 0.4)),
outline_color_(new Color(0.6, 0.6, 1)),
components_(new std::vector<Component*>()) {}

Model::~Model() {
  for(std::vector<Component*>::iterator i = components_->begin();
      i != components_->end(); ++i) {
    delete *i;
  }
  delete components_;
  delete selection_color_;
  delete selected_point_color_;
  delete outline_color_;
};

/**
 * The method adds the component the component vector that will be rendered.
 * The model will delete the Components on destruction or when calling clear()
 *
 * @param component component that shall be added
 */
void Model::add_component(Component* component) {
  components_->push_back(component);
  redraw_ = true;
}

/**
 * The method removes a component from the component vector and deletes it while
 * doing it.
 *
 * @param component component to be deleted
 */
void Model::remove_component(Component* component) {
  for(std::vector<Component*>::iterator i = components_->begin();
      i != components_->end(); ++i) {
    if(component == *i) {
      delete *i;
      components_->erase(i);
      selected_ = NULL;
      selected_point_ = NULL;
      break;
    }
  }
  redraw_ = true;
}

/**
 * The method clears the component vector - all Components will be deleted
 */
void Model::clear() {
  for(std::vector<Component*>::iterator i = components_->begin();
      i != components_->end(); ++i) {
    delete *i;
  }
  components_->clear();

  selected_point_ = NULL;
  selected_ = NULL;

  redraw_ = true;
}

bool Model::is_redraw() const {
  return redraw_;
}

void Model::set_redraw(const bool& redraw) {
  redraw_ = redraw;
}

bool Model::is_running() const {
  return running_;
}

void Model::set_running(const bool& running) {
  running_ = running;
}

bool Model::is_rendering() const {
  return rendering_;
}

void Model::set_rendering(const bool& rendering) {
  rendering_ = rendering;
}

unsigned int Model::get_window_width() const {
  return window_width_;
}

void Model::set_window_width(const unsigned int& window_width){
  window_width_ = window_width;
}

unsigned int Model::get_window_height() const {
  return window_height_;
}

void Model::set_window_height(const unsigned int& window_height){
  window_height_ = window_height;
}

unsigned int Model::get_marker_radius() const {
  return marker_readius_;
}

void Model::set_marker_radius(const unsigned int& marker_readius) {
  marker_readius_ = marker_readius;
}

unsigned int Model::get_marker_sections() const {
  return marker_sections_;
}

void Model::set_marker_sections(const unsigned int& marker_sections) {
  marker_sections_ = marker_sections;
}

Component* Model::get_selected() const {
  return selected_;
}

void Model::set_selected(Component* selected){
  if(selected_ != selected) {
    selected_ = selected;
    redraw_ = true;
  }
}

Point* Model::get_selected_point() const {
  return selected_point_;
}

void Model::set_selected_point(Point* selected_Point) {
  // if(selected_point_ != selected_Point) {
    selected_point_ = selected_Point;
    redraw_ = true;
  // }
}

Color Model::get_selection_color() const {
  return *selection_color_;
}

void Model::set_selection_color(Color* color) {
  selection_color_ = color;
}

Color Model::get_selected_point_color() const {
  return *selected_point_color_;
}

void Model::set_selected_point_color(Color* color) {
  selected_point_color_ = color;
}

Color Model::get_outline_color() const {
  return *outline_color_;
}

void Model::set_outline_color(Color* color) {
  outline_color_ = color;
}

std::vector<Component*>* Model::get_components() const {
  return components_;
}

void Model::set_components(std::vector<Component*>* components){
  components_ = components;
}

void Model::delete_component_pointer(Component* comp) {
  delete comp;
}

/**
 * The method returns the number of the component in the vector
 *
 * @param  comp component to be found
 *
 * @return      index of the component in the component vector
 */
long int Model::find_in_vector(Component* comp) const {
  for(unsigned int i = 0, i_end = components_->size(); i < i_end; ++i) {
    if(comp == components_->at(i)) {
      return i;
    }
  }
  // std::cout << "-1" << std::endl;
  return -1;
}

