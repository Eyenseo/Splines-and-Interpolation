#ifndef COLOR_HPP
#define COLOR_HPP

/**
 * Basic color class that stores Float colors
 */
class Color {
  float red_;
  float green_;
  float blue_;
  float alpha_;

public:
  Color();
  Color(const float& red,
        const float& green,
        const float& blue,
        const float& alpha = 0.0);

  Color(const Color& copy);
  ~Color();

   float get_red() const;
   void set_red(const float& red);

   float get_green() const;
   void set_green(const float& green);

   float get_blue() const;
   void set_blue(const float& blue);

   float get_alpha() const;
   void set_alpha(const float& alpha);

private:
  float valid_color_value(const float& value);
};

#endif
