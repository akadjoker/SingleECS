#pragma once

#include "Config.hpp"
#include <raylib.h>

const float RECIPROCAL_PI = 1.0f / PI;
const float HALF_PI = PI / 2.0f;
const float DEGTORAD = PI / 180.0f;
const float RADTODEG = 180.0f / PI;
const float MinValue = -3.40282347E+38;
const float MaxValue = 3.40282347E+38;
const float Epsilon = 1.401298E-45;

#define PI_TIMES_TWO 6.28318530718f
#define PI2 PI * 2
#define DEG -180 / PI
#define RAD PI / -180

struct Vec2
{
    float x;
    float y;

    Vec2();
    Vec2(float x, float y);

    ~Vec2() = default;


    void Set(float x, float y);

    void Add(const Vec2 &v);              
    void Sub(const Vec2 &v);              
    void Scale(const float n);           
    Vec2 Rotate(const float angle) const; 

    float Magnitude() const;     
    float MagnitudeSquared() const;

    Vec2 &Normalize();      
    Vec2 UnitVector() const; 
    Vec2 Normal() const;   

    float Dot(const Vec2 &v) const;   
    float Cross(const Vec2 &v) const; 

   // Vec2 &operator=(const Vec2 &v);       
    bool operator==(const Vec2 &v) const;
    bool operator!=(const Vec2 &v) const; 

    Vec2 operator+(const Vec2 &v) const;
    Vec2 operator-(const Vec2 &v) const; 
    Vec2 operator*(const float n) const; 
    Vec2 operator/(const float n) const; 
    Vec2 operator-();                    

    Vec2 &operator+=(const Vec2 &v); 
    Vec2 &operator-=(const Vec2 &v); 
    Vec2 &operator*=(const float n); 
    Vec2 &operator/=(const float n);

    Vec2 RotatePoint(float X, float Y, float PivotX, float PivotY, float Angle);

};


struct AABB
{
    Vec2 vMin;
    Vec2 vMax;

    AABB();
    AABB(const Vec2 &min, const Vec2 &max);
    AABB(float minX, float minY, float maxX, float maxY);
    void set(const Vec2 &min, const Vec2 &max);
    void set(float minX, float minY, float maxX, float maxY);


    bool intersects(const AABB &other) const;
    bool Intersects(const Vec2 &center, float radius);

    bool contains(const AABB &other) const;
    bool contains(const Vec2 &point) const;

    void encapsulate(const float x, const float y);
    void encapsulate(const AABB &other);
    void encapsulate(const Vec2 &point);

    void render(Color color) const;

     static AABB Transform(const Vec2 &pos, const Vec2 &pivot, const Vec2 &scale, float rot, float width, float height) ;

    float get_radius() const;
    Vec2 get_center() const;
    Vec2 get_extents() const;
    Vec2 get_size() const;
};

class Polar
{
public:
    Polar() : m_angle(0.0f), m_magnitude(0.0f) {}
    Polar(float a, float m) : m_angle(a), m_magnitude(m) {}

    void scale(float factor);
    void scale_to(float value);

    void rotate(float theta);
    void rotate_to(float value);

    void add(const Polar &other);

    float angle() const;
    float magnitude() const;

    float dir_x() const;
    float dir_y() const;
    float radians() const;

private:
    float m_angle;
    float m_magnitude;
};

namespace Math
{

    int sign(float value);
    float clip(float value, float min, float max);
    float clamp(float value, float min, float max);
    float lerp(float a, float b, float t);
    float normalize_angle(float angle);
    float clamp_angle(float angle, float min, float max);
    double hermite(double value1, double tangent1, double value2, double tangent2, double amount);
    double smooth_step(double value1, double value2, double amount);
    float repeat(double t, double length);
    double ping_pong(double t, double length);
    double get_distx(double a, double d);
    double get_disty(double a, double d);

    double range();
    double range(double min, double max);

    Vec2 RotateVector(const Vec2 &vector, double angle, const Vec2 &pivot);

} // namespace Math