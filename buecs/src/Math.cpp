
#include "pch.h"
#include "Math.hpp"

namespace Math
{

    int sign(float value) { return value < 0 ? -1 : (value > 0 ? 1 : 0); }

    static const double rand_scale = 1.0 / (1 << 16) / (1 << 16);
    double range()
    {
        unsigned int lo = rand() & 0xfff;
        unsigned int mid = rand() & 0xfff;
        unsigned int hi = rand() & 0xff;
        double result = (lo | (mid << 12) | (hi << 24)) * rand_scale;
        return result;
    }
    double range(double min, double max)
    {

        unsigned int lo = rand() & 0xfff;  // 12 bits
        unsigned int mid = rand() & 0xfff; // 12 bits
        unsigned int hi = rand() & 0xff;   // 8 bits

        double normalized = (lo | (mid << 12) | (hi << 24)) * rand_scale;

        return min + normalized * (max - min);
    }

    float clip(float value, float min, float max)
    {
        if (value < min)
            return min;
        if (value > max)
            return max;
        return value;
    }

    float clamp(float value, float min, float max)
    {
        if (value < min)
            return min;
        if (value > max)
            return max;
        return value;
    }

    float lerp(float a, float b, float t)
    {
        return a + t * (b - a);
    }

    float normalize_angle(float angle)
    {
        while (angle > 360)
            angle -= 360;
        while (angle < 0)
            angle += 360;
        return angle;
    }

    float clamp_angle(float angle, float min, float max)
    {

        angle = normalize_angle(angle);
        if (angle > 180)
        {
            angle -= 360;
        }
        else if (angle < -180)
        {
            angle += 360;
        }

        min = normalize_angle(min);
        if (min > 180)
        {
            min -= 360;
        }
        else if (min < -180)
        {
            min += 360;
        }

        max = normalize_angle(max);
        if (max > 180)
        {
            max -= 360;
        }
        else if (max < -180)
        {
            max += 360;
        }

        return clamp(angle, min, max);
    }

    double hermite(double value1, double tangent1, double value2, double tangent2, double amount)
    {
        double v1 = value1;
        double v2 = value2;
        double t1 = tangent1;
        double t2 = tangent2;
        double s = amount;
        double result;
        double sCubed = s * s * s;
        double sSquared = s * s;

        if (amount == 0)
            result = value1;
        else if (amount == 1)
            result = value2;
        else
            result = (2 * v1 - 2 * v2 + t2 + t1) * sCubed +
                     (3 * v2 - 3 * v1 - 2 * t1 - t2) * sSquared +
                     t1 * s +
                     v1;
        return result;
    }

    double smooth_step(double value1, double value2, double amount)
    {

        double result = clamp(amount, 0, 1);
        result = hermite(value1, 0, value2, 0, result);

        return result;
    }

    float repeat(double t, double length)
    {
        return clamp(t - floor(t / length) * length, 0.0f, length);
    }
    double ping_pong(double t, double length)
    {
        t = repeat(t, length * 2.0f);
        return length - abs(t - length);
    }

    double get_distx(double a, double d)
    {
        double angulo = (double)a * RAD;
        return ((double)(cos(angulo) * d));
    }

    double get_disty(double a, double d)
    {
        double angulo = (double)a * RAD;
        return (-(double)(sin(angulo) * d));
    }

     Vec2 RotateVector(const Vec2 &vector, double angle, const Vec2 &pivot)
    {
        Vec2 rotated(vector.x, vector.y);
        double x = rotated.x - pivot.x;
        double y = rotated.y - pivot.y;
        rotated.x = ((x - pivot.x) * cos(angle) - (y - pivot.y) * sin(angle)) + pivot.x;
        rotated.y = ((x - pivot.x) * sin(angle) + (y - pivot.y) * cos(angle)) + pivot.y;
        return rotated;
    }

} // namespace Math

//*********************************************************************************************************************
//**                         Vec2                                                                             **
//*********************************************************************************************************************

Vec2::Vec2() : x(0.0), y(0.0)
{
}

Vec2::Vec2(float x, float y) : x(x), y(y)
{
}

void Vec2::Set(float x, float y)
{
    this->x = x;
    this->y = y;
}

void Vec2::Add(const Vec2 &v)
{
    x += v.x;
    y += v.y;
}

void Vec2::Sub(const Vec2 &v)
{
    x -= v.x;
    y -= v.y;
}

void Vec2::Scale(const float n)
{
    x *= n;
    y *= n;
}


float Vec2::Magnitude() const
{
    return sqrtf(x * x + y * y);
}

float Vec2::MagnitudeSquared() const
{
    return (x * x + y * y);
}

Vec2 &Vec2::Normalize()
{
    float length = Magnitude();
    if (length != 0.0)
    {
        x /= length;
        y /= length;
    }
    return *this;
}

Vec2 Vec2::UnitVector() const
{
    Vec2 result = Vec2(0, 0);
    float length = Magnitude();
    if (length != 0.0)
    {
        result.x = x / length;
        result.y = y / length;
    }
    return result;
}

Vec2 Vec2::Normal() const
{
    return Vec2(y, -x).Normalize();
}

float Vec2::Dot(const Vec2 &v) const
{
    return (x * v.x) + (y * v.y);
}

float Vec2::Cross(const Vec2 &v) const
{
    return (x * v.y) - (y * v.x);
}

// Vec2 &Vec2::operator=(const Vec2 &v)
// {
//     x = v.x;
//     y = v.y;
//     return *this;
// }

bool Vec2::operator==(const Vec2 &v) const
{
    return x == v.x && y == v.y;
}

bool Vec2::operator!=(const Vec2 &v) const
{
    return !(*this == v);
}

Vec2 Vec2::operator+(const Vec2 &v) const
{
    Vec2 result;
    result.x = x + v.x;
    result.y = y + v.y;
    return result;
}

Vec2 Vec2::operator-(const Vec2 &v) const
{
    Vec2 result;
    result.x = x - v.x;
    result.y = y - v.y;
    return result;
}

Vec2 Vec2::operator*(const float n) const
{
    Vec2 result;
    result.x = x * n;
    result.y = y * n;
    return result;
}

Vec2 Vec2::operator/(const float n) const
{
    Vec2 result;
    result.x = x / n;
    result.y = y / n;
    return result;
}

Vec2 &Vec2::operator+=(const Vec2 &v)
{
    x += v.x;
    y += v.y;
    return *this;
}

Vec2 &Vec2::operator-=(const Vec2 &v)
{
    x -= v.x;
    y -= v.y;
    return *this;
}

Vec2 &Vec2::operator*=(const float n)
{
    x *= n;
    y *= n;
    return *this;
}

Vec2 &Vec2::operator/=(const float n)
{
    x /= n;
    y /= n;
    return *this;
}

Vec2 Vec2::operator-()
{
    Vec2 result;
    result.x = x * -1;
    result.y = y * -1;
    return result;
}
Vec2 Vec2::RotatePoint(float X, float Y, float PivotX, float PivotY, float Angle)
{
    float sin = 0;
    float cos = 0;
    float radians = Angle * -0.017453293f;

    while (radians < -3.14159265f)
        radians += 6.28318531f;
    while (radians > 3.14159265f)
        radians -= 6.28318531f;

    if (radians < 0)
    {
        sin = 1.27323954f * radians + 0.405284735f * radians * radians;
        if (sin < 0)
            sin = 0.225f * (sin * -sin - sin) + sin;
        else
            sin = 0.225f * (sin * sin - sin) + sin;
    }
    else
    {
        sin = 1.27323954f * radians - 0.405284735f * radians * radians;
        if (sin < 0)
            sin = 0.225f * (sin * -sin - sin) + sin;
        else
            sin = 0.225f * (sin * sin - sin) + sin;
    }

    radians += 1.57079632f; // +90 graus
    if (radians > 3.14159265f)
        radians -= 6.28318531f;
    if (radians < 0)
    {
        cos = 1.27323954f * radians + 0.405284735f * radians * radians;
        if (cos < 0)
            cos = 0.225f * (cos * -cos - cos) + cos;
        else
            cos = 0.225f * (cos * cos - cos) + cos;
    }
    else
    {
        cos = 1.27323954f * radians - 0.405284735f * radians * radians;
        if (cos < 0)
            cos = 0.225f * (cos * -cos - cos) + cos;
        else
            cos = 0.225f * (cos * cos - cos) + cos;
    }

    float dx = X - PivotX;
    float dy = PivotY - Y;

    Vec2 result;

    result.x = PivotX + cos * dx - sin * dy;
    result.y = PivotY - sin * dx - cos * dy;

    return result;
}


Vec2 Vec2::Rotate(const float angle) const
{
    Vec2 result;
    result.x = x * cos(angle) - y * sin(angle);
    result.y = x * sin(angle) + y * cos(angle);
    return result;
}
//*********************************************************************************************************************
//**                         Polar                                                                             **
//*********************************************************************************************************************

void Polar::scale(float factor)
{
    m_magnitude *= factor;
}

void Polar::scale_to(float value)
{
    m_magnitude = value;
}

void Polar::rotate(float theta)
{
    m_angle -= theta;
    while (m_angle < 0 || m_angle >= 360)
        m_angle += (m_angle >= 360 ? -360.0f : 360.0f);
}

void Polar::rotate_to(float value)
{
    m_angle = value;
}

void Polar::add(const Polar &v)
{
    float rx, ry;

    rx = dir_x() + v.dir_x();
    ry = dir_y() + v.dir_y();

    m_magnitude = sqrt(pow(rx, 2.0f) + pow(ry, 2.0f));
    if (rx > 0)
    {
        if (ry >= 0)
        {
            m_angle = atan(ry / rx);
            m_angle = float((180.0 * m_angle) / PI);
        }
        else // (ry < 0)
        {
            m_angle = atan(ry / rx);
            m_angle = float((180.0 * m_angle) / PI) + 360.0f;
        }
    }
    else if (rx < 0)
    {
        m_angle = atan(ry / rx);
        m_angle = float((180.0 * m_angle) / PI) + 180.0f;
    }
    else
    {
        if (ry > 0)
            m_angle = 90.0f;
        else if (ry < 0)
            m_angle = 270.0f;
        else
            m_angle = v.m_angle;
    }
}

float Polar::angle() const
{
    return m_angle;
}

float Polar::magnitude() const
{
    return m_magnitude;
}

float Polar::dir_x() const
{
    return m_magnitude * std::cos(radians());
}

float Polar::dir_y() const
{
    return m_magnitude * std::sin(radians());
}

float Polar::radians() const
{
    return (float)(m_angle * PI / 180.0f);
}



//*********************************************************************************************************************
//**                         AABB                                                                              **
//*********************************************************************************************************************

AABB::AABB()
{
    vMin.x = 0;
    vMin.y = 0;
    vMax.x = 0;
    vMax.y = 0;
}

AABB::AABB(const Vec2 &min, const Vec2 &max)
{
    vMin = min;
    vMax = max;
}

AABB::AABB(float minX, float minY, float maxX, float maxY)
{
    vMin.x = minX;
    vMin.y = minY;
    vMax.x = maxX;
    vMax.y = maxY;
}

void AABB::set(const Vec2 &min, const Vec2 &max)
{
    vMin = min;
    vMax = max;
}

void AABB::set(float minX, float minY, float maxX, float maxY)
{
    vMin.x = minX;
    vMin.y = minY;
    vMax.x = maxX;
    vMax.y = maxY;
}

bool AABB::intersects(const AABB &other) const
{

    return (vMin.x < other.vMax.x && vMax.x > other.vMin.x &&
            vMin.y < other.vMax.y && vMax.y > other.vMin.y);
}

bool AABB::Intersects(const Vec2 &center, float radius)
{
    float closestX = std::max(vMin.x, std::min(center.x, vMax.x));
    float closestY = std::max(vMin.y, std::min(center.y, vMax.y));
    float distanceX = center.x - closestX;
    float distanceY = center.y - closestY;
    return (distanceX * distanceX + distanceY * distanceY) < (radius * radius);
}

bool AABB::contains(const AABB &other) const
{
    return (vMin.x <= other.vMin.x && vMax.x >= other.vMax.x && vMin.y <= other.vMin.y && vMax.y >= other.vMax.y);
}

bool AABB::contains(const Vec2 &point) const
{
    return (point.x >= vMin.x && point.x <= vMax.x && point.y >= vMin.y && point.y <= vMax.y);
}

void AABB::encapsulate(const float x, const float y)
{
    vMin.x = std::min(vMin.x, x);
    vMin.y = std::min(vMin.y, y);
    vMax.x = std::max(vMax.x, x);
    vMax.y = std::max(vMax.y, y);
}

void AABB::encapsulate(const AABB &other)
{
    encapsulate(other.vMin.x, other.vMin.y);
    encapsulate(other.vMax.x, other.vMax.y);
}

void AABB::encapsulate(const Vec2 &point)
{
    encapsulate(point.x, point.y);
}

void AABB::render(Color color) const
{
    DrawLine(vMin.x, vMin.y, vMin.x, vMax.y, color); // left
    DrawLine(vMax.x, vMin.y, vMax.x, vMax.y, color); // right
    DrawLine(vMin.x, vMin.y, vMax.x, vMin.y, color); // top
    DrawLine(vMin.x, vMax.y, vMax.x, vMax.y, color); // bottom
}

AABB AABB::Transform(const Vec2 &pos, const Vec2 &pivot, const Vec2 &scale, float rot, float width, float height)
{

    Vec2 v[4];
    float TempX1 = 0;
    float TempY1 = 0;
    float TempX2 = width;
    float TempY2 = height;

    v[1].x = TempX1;
    v[1].y = TempY1;

    v[0].x = TempX1;
    v[0].y = TempY2;

    v[3].x = TempX2;
    v[3].y = TempY2;

    v[2].x = TempX2;
    v[2].y = TempY1;


    AABB box;
    float minX = MaxValue;
    float minY = MaxValue;
    float maxX = MinValue;
    float maxY = MinValue;

    for (int i = 0; i < 4; i++)
    {
      

        v[i] = Math::RotateVector(v[i], rot, pivot);
        v[i].x *= scale.x;
        v[i].y *= scale.y;
        v[i].x += pivot.x;
        v[i].y += pivot.y;

        float x = v[i].x;
        float y = v[i].y;

      
        if (x < minX)
        {
            minX = x;
        }
        if (x > maxX)
        {
            maxX = x;
        }
        if (y < minY)
        {
            minY = y;
        }
        if (y > maxY)
        {
            maxY = y;
        }


    }


    box.set(minX, minY, maxX, maxY);

    return box;
}

Vec2 AABB::get_size() const
{
    return vMax - vMin;
}

Vec2 AABB::get_center() const
{
    return (vMin + vMax) * 0.5f;
}

Vec2 AABB::get_extents() const
{
    return (vMax - vMin) * 0.5f;
}