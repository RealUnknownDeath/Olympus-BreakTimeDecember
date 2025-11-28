#ifndef SANTIS_EIGEN_H
#define SANTIS_EIGEN_H

#include <cmath>

struct ObstacleLine {
  enum Type {VERTICAL, HORIZONTAL, DIAGONAL_POS, DIAGONAL_NEG, DIAGONAL} type; //diagonal pos is lines with "/" slope while diagonal neg is lines with "\" slope

  float x1, y1;
  float x2, y2;

  ObstacleLine(Type t, float x1_, float y1_, float x2_, float y2_): type(t), x1(x1_), y1(y1_), x2(x2_), y2(y2_) {}
};

struct Segment {
  float x1, y1;
  float x2, y2;

  Segment() : x1(0), y1(0), x2(0), y2(0) {}
  Segment(float _x1, float _y1, float _x2, float _y2)
  : x1(_x1), y1(_y1), x2(_x2), y2(_y2) {}
};


struct Vector2f {
    float x;
    float y;

    //Constructors
    Vector2f() : x(0), y(0) {}
    Vector2f(float x_, float y_) : x(x_), y(y_) {}

    //Basic arithmetic operations
    Vector2f operator+(const Vector2f& other) const {
      return Vector2f(x + other.x, y + other.y);
    }

    Vector2f operator-(const Vector2f& other) const {
      return Vector2f(x - other.x, y - other.y);
    }

    Vector2f operator*(float scalar) const {
        return Vector2f(x * scalar, y * scalar);
    }

    Vector2f operator/(float scalar) const {
        return Vector2f(x / scalar, y / scalar);
    }

    Vector2f& operator+=(const Vector2f& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    Vector2f& operator-=(const Vector2f& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    Vector2f& operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    Vector2f& operator/=(float scalar) {
        x /= scalar;
        y /= scalar;
        return *this;
    }

    //Dot product
    float dot(const Vector2f& other) const {
        return x * other.x + y * other.y;
    }

    //Magnitude (length)
    float magnitude() const {
        return std::sqrt(x * x + y * y);
    }

    //Squared magnitude (useful for comparisons)
    float magnitude_squared() const {
        return x * x + y * y;
    }

    //Normalization
    Vector2f normalized() const {
        float mag = magnitude();
        if (mag == 0) return Vector2f(0, 0);
        return *this / mag;
    }

    //Distance to another vector
    float distance_to(const Vector2f& other) const {
        return (*this - other).magnitude();
    }

    //Equality
    bool equals(const Vector2f& other, float epsilon = 1e-5f) const {
        return std::fabs(x - other.x) < epsilon && std::fabs(y - other.y) < epsilon;
    }
};


struct Rotation2Df {
    float angle_rad;

    Rotation2Df() : angle_rad(0) {}
    Rotation2Df(float angle_degrees) : angle_rad(angle_degrees * 3.14159265359f / 180.0f) {} //pi wasn't working for some reason idk

    Vector2f operator()(const Vector2f& v) const {
        float cos_a = std::cos(angle_rad);
        float sin_a = std::sin(angle_rad);
        return Vector2f(
            v.x * cos_a - v.y * sin_a,
            v.x * sin_a + v.y * cos_a
        );
    }
};

struct Particle { //For AMCL
    Vector2f position;
    float heading_deg;

    float weight = 1.0f; //Default to 1.0

    Particle(Vector2f pos, float heading)
        : position(pos), heading_deg(heading), weight(1.0f) {}

    bool operator==(const Particle& other) const {
        const float epsilon = 1e-5f;
        return std::abs(position.x - other.position.x) < epsilon &&
               std::abs(position.y - other.position.y) < epsilon &&
               std::abs(heading_deg - other.heading_deg) < epsilon;
    }
};


#endif
