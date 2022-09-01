#pragma once

#include <cmath>

class geom_vector_t {
public:
  int x;
  int y;

  geom_vector_t(int x0 = -1, int y0 = -1) {
    x = x0;
    y = y0;
  }

  geom_vector_t(const geom_vector_t& v) {
    x = v.x;
    y = v.y;
  }

  geom_vector_t& operator=(const geom_vector_t& v) {
    x = v.x;
    y = v.y;

    return *this;
  }

  geom_vector_t operator+(const geom_vector_t& v) const {
    return geom_vector_t(x + v.x, y + v.y);
  }

  geom_vector_t& operator+=(const geom_vector_t& v) {
    x += v.x;
    y += v.y;

    return *this;
  }

  geom_vector_t operator-(const geom_vector_t& v) const {
    return geom_vector_t(x - v.x, y - v.y);
  }

  geom_vector_t& operator-=(const geom_vector_t& v) {
    x -= v.x;
    y -= v.y;

    return *this;
  }

  geom_vector_t operator*(int a) const {
    return geom_vector_t(x * a, y * a);
  }

  geom_vector_t& operator*=(int a) {
    x *= a;
    y *= a;

    return *this;
  }

  geom_vector_t operator/(int a) const {
    return geom_vector_t(x / a, y / a);
  }
  
  geom_vector_t& operator/=(int a) {
    x /= a;
    y /= a;

    return *this;
  }

  bool operator==(const geom_vector_t& v) const {
    return x == v.x && y == v.y;
  }

  double Len(void) const {
    return sqrt(x * x + y * y);
  }
};