#ifndef Renderer_HPP
#define Renderer_HPP

#include <stdlib.h>
#include <vector>
#include <math.h>
#include <GL/glfw.h>

#include "Component.hpp"

/**
 * The renderer class was intended to run in it's own thread. But glfw doesn't
 * support to share the OpenGL context and thus the it was to complicated to
 * include that feature in this small homework. this class is basically dead...
 */

class Renderer {
public:
  Renderer();
  Renderer(Model* model);
  ~Renderer();

  /**
   * Update will be called for a re-rendering of all components of the model
   */
  void update();
private:
  /**
   * initialises the GLFW window
   */
  void create_window();

  Model* model_;
};
#endif
