#include "Color.hpp"

Color::Color() :
  red_(0.0),
  green_(0.0),
  blue_(0.0),
  alpha_(0.0) {}

Color::Color( const float& red,
              const float& green,
              const float& blue,
              const float& alpha) {
  set_red(red);
  set_green(green);
  set_blue(blue);
  set_alpha(alpha);
}

Color::Color(const Color& copy) :
  red_(copy.red_),
  green_(copy.green_),
  blue_(copy.blue_),
  alpha_(copy.alpha_) {}

Color::~Color() {}

float Color::get_red() const {
  return red_;
}

void Color::set_red(const float& red) {
  red_ = valid_color_value(red);
}

float Color::get_green() const {
  return green_;
}

void Color::set_green(const float& green) {
  green_ = valid_color_value(green);
}

float Color::get_blue() const {
  return blue_;
}

void Color::set_blue(const float& blue) {
  blue_ = valid_color_value(blue);
}

float Color::get_alpha() const {
  return alpha_;
}

void Color::set_alpha(const float& alpha) {
  alpha_ = valid_color_value(alpha);
}

/**
 * Method checks that the values are in the range of 0.0 and 1.0
 * @param  value value to check
 * @return       validated value
 *               anything higher than 1.0 will be returned as 1.0
 *               anything smaller than 0.0 will be returned as 0.0
 */
float Color::valid_color_value(const float& value) {
  if(value > 1.0) {
    return 1.0;
  } else if (value < 0.0) {
    return 0.0;
  } else {
    return value;
  }
}
