#include "Renderer.hpp"

/**
 * The renderer class was intended to run in it's own thread. But glfw doesn't
 * support to share the OpenGL context and thus the it was to complicated to
 * include that feature in this small homework. this class is basically dead...
 */

Renderer::Renderer() {}

Renderer::Renderer(Model* model) {
  model_ = model;
  create_window();
}

Renderer::~Renderer() {
  glfwTerminate();
}

void Renderer::update() {
  glClearColor(0.8, 0.8, 0.8, 1.0); // TODO move background color to the model
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



  for(std::vector<Component*>::iterator i = model_->get_components()->begin();
      i != model_->get_components()->end(); ++i) {
    if((*i) == model_->get_selected()) {
      (*i)->draw_selected(model_->get_selected_point());
    } else {
      (*i)->draw();
    }
  }

  glfwSwapBuffers();
}

void Renderer::create_window() {
  glfwInit();



  // glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 500);
  glfwOpenWindow( model_->get_window_width(), // width
                  model_->get_window_height(), // height
                  8, // red bits
                  8, // blue bits
                  8, // green bits
                  8, // alpha bits
                  24, // depth bits
                  0,  // stencial bits ?
                  GLFW_WINDOW); // mode

  glViewport(0, 0, model_->get_window_width(), model_->get_window_height());

  // projection matrix with clipping planes in three directions
  // (same as window size and +- 1 in z)
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, model_->get_window_width(), 0, model_->get_window_height(), -1, 1);

  // modelview transform (identity)
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();


  // // Antialiasing
  // glEnable(GL_LINE_SMOOTH);
  // glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
  // glEnable(GL_BLEND);
  // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  // glLineWidth(1.25);
  // glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 8);

}
