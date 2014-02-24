#include <iostream>
#include <string>

#include "Model.hpp"
#include "Renderer.hpp"
#include "Polygon.hpp"
#include "CubicInterpolation.hpp"
#include "QuadraticSpline.hpp"
#include "CubicSpline.hpp"
#include "Bezier.hpp"

/**
 * Main is the one and only running thread that starts and ends the program. It
 * acts also as a controller for the model and the renderer.
 */

Model* model_;
bool DEMO = true;

/**
 * The method checks if the point specified by x and y is on an object.
 * If an object is already selected only the selected one will be checked.
 *
 * @param  x    x position of the point to be checked
 * @param  y    y position of the point to be checked
 *
 * @param  comp returns the component that the point is on or NULL
 * @param  pos  returns the Point of the component that was selected or NULL
 * @return      true if it is on a point
 */
bool on_point(const unsigned int& x,
              const unsigned int& y,
              Component*& comp,
              Point*& pos) {
  Component* selected = model_->get_selected();
  pos = NULL;

  if(selected != NULL) { // Only check the selected
    comp = selected;
    if((pos = selected->on_object(x, y, true)) != NULL) {
      // model_->set_selected_point(pos);
      return true;
    }
    return false;
  }

  //go through all available components of the model
  for(std::vector<Component*>::iterator i = model_->get_components()->begin();
      i != model_->get_components()->end(); ++i) {
    if((pos = (*i)->on_object(x, y, false)) != NULL) {
      comp = *i;
      return true;
    }
  }
  return false;
}

/**
 * Mouse button callback for GLFW.
 *
 * @param button is the number of the button that the call is from
 *               0 for left button
 *               1 for right button
 * @param press  state of the button the event is from
 *               0 for not pressed
 *               1 for pressed
 */
void mouse_button_callback(int button, int press) {
  int x, y;

  Point* pos = NULL;
  Component* comp = NULL;

  if(button == 0 && press == 1) {
    glfwGetMousePos(&x, &y); // get the position of the mouse pointer
    y = model_->get_window_height() - y; // correct the hight value

    if(on_point(x, y, comp, pos)) { // if on an component
      if(model_->get_selected() == NULL) {
        model_->set_selected(&*comp);
      }
      if(model_->get_selected() == comp)  {
        model_->set_selected_point(pos);
      }
    } else {
      comp = model_->get_selected();

      if(comp == NULL) { // if no component is selected add new component
        comp = new Polygon(model_); // default add is Polygon
        comp->add_Point(x, y);

        model_->add_component(comp);
        model_->set_selected(comp);

        on_point(x, y, comp, pos);
        model_->set_selected_point(pos);
      } else { // add new point
        comp->add_Point(x, y);

        on_point(x, y, comp, pos);
        model_->set_selected_point(pos);
      }
    }
  } else if(button == 1 && press == 1) { // deselect on right click
    model_->set_selected_point(NULL);
    model_->set_selected(NULL);
  }
}

/**
 * mouse pointer callback for GLFW
 *
 * @param x x position of the mouse pointer
 * @param y y position of the mouse pointer
 */
void mouse_point_callback(int x, int y) {
  y = model_->get_window_height() - y; // correct the hight value

  Point* pos = NULL;

  if(glfwGetMouseButton(0) == 1) { // check if the left mouse button is pressed
    if((pos = model_->get_selected_point()) != NULL) {
      pos->x = x;
      pos->y = y;

      model_->set_redraw(true); // force redraw after move
    }
  }
}

/**
 * key press callback for GLFW
 *
 * @param key   key number of the call
 * @param press state of the key
 *              0 not pressed
 *              1 pressed
 */
void key_press_callback(int key, int press) {
  Component* selected = model_->get_selected();

  // std::cout << "Key: " << key << "\tPress: " << press<<std::endl;

  if(press == 1) {
    if(selected != NULL) {
      switch(key) {
        case 49: // 1
          model_->change_type<Polygon>(selected);
          break;
        case 50: // 2
          model_->change_type<CubicInterpolation>(selected);
          break;
        case 51: // 3
          model_->change_type<QuadraticSpline>(selected);
          break;
        case 52: // 4
          model_->change_type<CubicSpline>(selected);
          break;
        case 53: // 5
          model_->change_type<Bezier>(selected);
          break;
        case 56: // 8
          if(model_->get_selected_point() != NULL) {
            selected->remove_Point(*model_->get_selected_point());
          }
          break;
        case 57: // 9
            model_->remove_component(selected);
          break;
        case 83: // S
            selected->smooth();
          break;
        case 67: // C
            auto rand_float = []() -> float{
              return (rand() % 100 + 1) / 100.0;
            };
            Color c  = Color(rand_float(), rand_float(), rand_float());
            selected->set_color(c);
          break;
      }
    }
    switch(key) {
      case 48: // 0
        model_->clear(); // Remove all components
        break;
      case 257: // ESC
        model_->set_running(false);
        break;
    }
  }
}

std::string greeter() {
  std::string s;

  s = "Hello,\n"
  "in this program you are able to play with points!\n\n"
  "Use your mouse like this:\n"
  "  Select   - Press the left mouse button anywhere on a polygon or 'curve'\n"
  "  Move     - Select a point and drag it around by pressing the left mouse "
  "button\n"
  "  Deselect - Press the right mouse button anywhere\n"
  "             (don't try this with one point - you will most probably lose"
  " it)\n"
  "\n"
  "  Add a Point   - Press the left mouse button anywhere with a selection\n"
  "  Add a Polygon - Press the left mouse button anywhere without a "
  "selection\n"
  "\n"
  "You might want to use your keyboard like this:\n"
  "  Pressing  1  will draw the selected polygon as polygon\n"
  "  Pressing  2  will draw the selected polygon as cubic interpolation curve\n"
  "  Pressing  3  will draw the selected polygon as quadratic spline curve\n"
  "  Pressing  4  will draw the selected polygon as cubic spline curve\n"
  "  Pressing  5  will draw the selected polygon as Bezier curve\n"
  "  Pressing  8  will delete the selected point\n"
  "  Pressing  9  will delete the selected polygon\n"
  "  Pressing  0  will delete all polygons / points\n"
  "  Pressing  C  will change the color of the selected polygon\n"
  "  Pressing  S  will smooth the selected polygon\n"
  "  Pressing ESC will end the program\n\n";

  return s;
}

int main () {
  std::cout << greeter() << "\nSetting up ..." << std::endl;

  try {
    std::cout << "Initialise ... " << std::endl;
    model_ = new Model(1000, 800); // Window size w, h
    Renderer* r = new Renderer(model_);

    //Demo Polygon
    if(DEMO) {
      std::cout << "Setting up the demo ... " << std::endl;
      Component* b = new Polygon(model_);
      b->add_Point(000, 100);
      b->add_Point(100, 200);
      b->add_Point(000, 300);
      b->add_Point(100, 400);
      b->add_Point(000, 500);
      b->add_Point(200, 400);
      b->add_Point(300, 500);
      b->add_Point(400, 400);
      b->add_Point(500, 500);
      b->add_Point(600, 400);
      b->add_Point(500, 300);
      b->add_Point(600, 200);
      b->add_Point(500, 100);

      Color c = Color();
      c.set_red(0.5);
      c.set_blue(1);
      c.set_green(0);

      b->set_color(c);

      model_->add_component(b);
      model_->set_selected(b);

      Point* pos = NULL;
      on_point(500, 100, b, pos);
      model_->set_selected_point(pos);

      // model_->change_type<Bezier>(b);
    }

    // set callbacks
    glfwSetMouseButtonCallback(&mouse_button_callback);
    glfwSetMousePosCallback(&mouse_point_callback);
    glfwSetKeyCallback(&key_press_callback);

    std::cout << "Done!\nHave fun :)" << std::endl;

    // main loop
  	while(model_->is_running()) {
      if(model_->is_redraw()) {
        r->update();
        model_->set_redraw(false);
      }

      glfwWaitEvents();

  		if(!glfwGetWindowParam(GLFW_OPENED)) {
  			break;
  		}
  	}

    std::cout << "\nCleaning up ..." << std::endl;

    delete model_;
    delete r;
  } catch(const std::exception &e) {
    std::cout << std::endl;
    std::cerr << e.what() << std::endl;
    return 1;
  } catch(...) {
    std::cout << std::endl;
    std::cerr << "Unknown Exception found ..." << std::endl;
    return 1;
  }

  std::cout << "\nGoodbye!" << std::endl;
	return 0;
}
