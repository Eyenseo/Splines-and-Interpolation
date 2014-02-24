#ifndef MODEL_HPP
#define MODEL_HPP

#include <stdlib.h>
#include <vector>

#include "Point.hpp"
#include "Color.hpp"

class Component;

/**
 * The model has all needed and configurable objects of the program
 */
class Model {
  bool rendering_;
  bool redraw_;
  bool running_;
  unsigned int window_width_;
  unsigned int window_height_;
  unsigned int marker_readius_;
  unsigned int marker_sections_;
  Component* selected_;
  Point* selected_point_;
  Color* selection_color_;
  Color* selected_point_color_;
  Color* outline_color_;
  std::vector<Component*>* components_;

public:
  Model();
  Model(const unsigned int& width, const unsigned int& height);
  ~Model();

  void add_component(Component* component);
  void remove_component(Component* component);

  void clear();

  /**
   * The method changes the type of an component by invoking the copy
   * constructor
   *
   * Because of the circle dependency and the template this part needs to be
   * implemented here
   *
   * @param comp component to be changed
   */
  template <typename T> void change_type(Component* comp) {
    T* changed =  new T(*comp);
    if(selected_ == comp) {
      selected_ = changed;
      if(selected_point_ != NULL) {
        selected_point_ = changed->find_identical(*selected_point_);
      }
      redraw_ = true;
    }
    components_->at(find_in_vector(comp)) = changed;
    delete_component_pointer(comp);
  }

  bool is_redraw() const;
  void set_redraw(const bool& redraw);

  bool is_running() const;
  void set_running(const bool& running);

  bool is_rendering() const;
  void set_rendering(const bool& rendering);

  unsigned int get_window_width() const;
  void set_window_width(const unsigned int& window_width);

  unsigned int get_window_height() const;
  void set_window_height(const unsigned int& window_height);

  unsigned int get_marker_radius() const;
  void set_marker_radius(const unsigned int& marker_readius);

  unsigned int get_marker_sections() const;
  void set_marker_sections(const unsigned int& marker_sections);

  Component* get_selected() const;
  void set_selected(Component* selected);

  Point* get_selected_point() const;
  void set_selected_point(Point* selected_Point);

  Color get_selection_color() const;
  void set_selection_color(Color* color);

  Color get_selected_point_color() const;
  void set_selected_point_color(Color* color);

  Color get_outline_color() const;
  void set_outline_color(Color* color);

  Color get_selected_point_color_() const;
  void set_selected_point_color_(Color* color);

  std::vector<Component*>* get_components() const;
  void set_components(std::vector<Component*>* components);

private:
  void delete_component_pointer(Component* comp);
  long int find_in_vector(Component* comp) const;
};

#endif
