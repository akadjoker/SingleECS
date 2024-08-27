#include "pch.h"
#include "Math.hpp"
#include "Core.hpp"
#include "Utils.hpp"










static inline float Min(float a, float b) { return a < b ? a : b; }
static inline float Max(float a, float b) { return a > b ? a : b; }
static inline Vec2 Max(const Vec2 &a, const Vec2 &b) { return Vec2(Max(a.x, b.x), Max(a.y, b.y)); }
static inline Vec2 Min(const Vec2 &a, const Vec2 &b) { return Vec2(Min(a.x, b.x), Min(a.y, b.y)); }
static inline float Abs(float a) { return a > 0 ? a : -a; }
static inline float Sign(float a) { return a > 0 ? 1.0f : -1.0f; }
const float VerySmallAmount = 0.0005f;


inline float LengthSquared(const Vec2 &v)
{
    return v.x * v.x + v.y * v.y;
}

inline float Length(const Vec2 &v)
{
    return sqrt(v.x * v.x + v.y * v.y);
}

inline float DistanceSquared(const Vec2 &a, const Vec2 &b)
{
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    return dx * dx + dy * dy;
}

inline float Distance(const Vec2 &a, const Vec2 &b)
{
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    return sqrt(dx * dx + dy * dy);
}

inline Vec2 Normalize(const Vec2 &v)
{
    float len = Length(v);
    return Vec2(v.x / len, v.y / len);
}

inline float Dot(const Vec2 &a, const Vec2 &b)
{
    return a.x * b.x + a.y * b.y;
}

inline float Cross(const Vec2 &a, const Vec2 &b)
{
    return a.x * b.y - a.y * b.x;
}

inline bool NearlyEqual(float a, float b)
{
    return abs(a - b) < VerySmallAmount;
}

inline bool NearlyEqual(const Vec2 &a, const Vec2 &b)
{
    return DistanceSquared(a, b) < VerySmallAmount * VerySmallAmount;
}




inline void ProjectVertices(const std::vector<Vec2> &vertices, const Vec2 &axis, float &min, float &max)
{
    min = Dot(vertices[0], axis);
    max = min;

    for (u32 i = 1; i < vertices.size(); i++)
    {
        float projection = Dot(vertices[i], axis);
        if (projection < min)
            min = projection;
        if (projection > max)
            max = projection;
    }
}
inline bool overlapOnAxis(float minA, float maxA, float minB, float maxB)
{
    return !(maxA < minB || maxB < minA);
};


bool Collider::IntersectPolygons(const std::vector<Vec2> &verticesA, const std::vector<Vec2> &verticesB)
{
    float minA, maxA, minB, maxB;

    for (u32 i = 0; i < verticesA.size(); i++)
    {
        Vec2 va = verticesA[i];
        Vec2 vb = verticesA[(i + 1) % verticesA.size()];

        Vec2 edge = vb - va;
        Vec2 axis = Normalize(Vec2(-edge.y, edge.x));

        ProjectVertices(verticesA, axis, minA, maxA);
        ProjectVertices(verticesB, axis, minB, maxB);
        if (!overlapOnAxis(minA, maxA, minB, maxB))
        {
            return false;
        }
    }

    for (u32 i = 0; i < verticesB.size(); i++)
    {
        Vec2 va = verticesB[i];
        Vec2 vb = verticesB[(i + 1) % verticesB.size()];

        Vec2 edge = vb - va;
        Vec2 axis = Normalize(Vec2(-edge.y, edge.x));

        ProjectVertices(verticesA, axis, minA, maxA);
        ProjectVertices(verticesB, axis, minB, maxB);
        if (!overlapOnAxis(minA, maxA, minB, maxB))
        {
            return false;
        }
    }

    return true;
}




inline void ProjectCircle(const Vec2 &center, float radius, const Vec2 &axis, float &min, float &max)
{
    Vec2 direction = Normalize(axis);
    Vec2 directionAndRadius = direction * radius;

    Vec2 p1 = center + directionAndRadius;
    Vec2 p2 = center - directionAndRadius;

    min = Dot(p1, axis);
    max = Dot(p2, axis);

    if (min > max)
    {
        // swap the min and max values.
        float t = min;
        min = max;
        max = t;
    }
}

inline int FindClosestPointOnPolygon(Vec2 circleCenter, const std::vector<Vec2> &vertices)
{
    int result = -1;
    float minDistance = MaxValue;

    for (u32 i = 0; i < vertices.size(); i++)
    {
        Vec2 v = vertices[i];
        float distance = Distance(v, circleCenter);

        if (distance < minDistance)
        {
            minDistance = distance;
            result = i;
        }
    }

    return result;
}

bool Collider::IntersectCirclePolygon(const Vec2 &circleCenter, float circleRadius, const std::vector<Vec2> &vertices)
{
     if (vertices.size() < 3)
        return false;

    Vec2 axis = Vec2(0.0f, 0.0f);
    float minA, maxA, minB, maxB;

    for (u32 i = 0; i < vertices.size(); i++)
    {
        Vec2 va = vertices[i];
        Vec2 vb = vertices[(i + 1) % vertices.size()];

        Vec2 edge = vb - va;
        axis = Vec2(-edge.y, edge.x);
        axis = Normalize(axis);

        ProjectVertices(vertices, axis, minA, maxA);
        ProjectCircle(circleCenter, circleRadius, axis, minB, maxB);

        if (minA >= maxB || minB >= maxA)
        {
            return false;
        }
    }

    int cpIndex = FindClosestPointOnPolygon(circleCenter, vertices);
    Vec2 cp = vertices[cpIndex];

    axis = cp - circleCenter;
    axis = Normalize(axis);

    ProjectVertices(vertices, axis, minA, maxA);
    ProjectCircle(circleCenter, circleRadius, axis, minB, maxB);

    if (minA >= maxB || minB >= maxA)
    {
        return false;
    }

    return true;
}

void Collider::TransformPoints(std::vector<Vec2> &points, const Vec2 &position, double angle, const Vec2 &pivot, const Vec2 &scale, AABB *aabb)
{
    float minX = MaxValue;
    float minY = MaxValue;
    float maxX = MinValue;
    float maxY = MinValue;

    for (u32 i = 0; i < points.size(); i++)
    {

        points[i] = Math::RotateVector(points[i], angle * DEG2RAD, pivot);
        points[i].x *= scale.x;
        points[i].y *= scale.y;
        points[i].x += position.x;
        points[i].y += position.y;
        float x = points[i].x;
        float y = points[i].y;

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

          DrawCircle(points[i].x, points[i].y, 4, LIME);
    }
    aabb->set(minX, minY, maxX, maxY);
    DrawLine(aabb->vMin.x, aabb->vMin.y, aabb->vMin.x, aabb->vMax.y, MAGENTA); // left
    DrawLine(aabb->vMax.x, aabb->vMin.y, aabb->vMax.x, aabb->vMax.y, MAGENTA); // right
    DrawLine(aabb->vMin.x, aabb->vMin.y, aabb->vMax.x, aabb->vMin.y, MAGENTA); // top
    DrawLine(aabb->vMin.x, aabb->vMax.y, aabb->vMax.x, aabb->vMax.y, MAGENTA); // bottom
}
 bool Collider::IntersectAABBs(AABB a, AABB b)
{
    if (a.vMax.x <= b.vMin.x || b.vMax.x <= a.vMin.x ||
        a.vMax.y <= b.vMin.y || b.vMax.y <= a.vMin.y)
    {
        return false;
    }

    return true;
}
