#include "Vector.h"

// 3D vector float
const Vec3f Vec3f::invalid = Vec3f(FLT_MAX, FLT_MAX, FLT_MAX);
const Vec3f Vec3f::right = Vec3f(1, 0, 0);
const Vec3f Vec3f::up = Vec3f(0, 1, 0);
const Vec3f Vec3f::forward = Vec3f(0, 0, 1);
const Vec3f Vec3f::zero = Vec3f(0,0,0);
const Vec3f Vec3f::one = Vec3f(1,1,1);

// 2D vector float
const Vec2f Vec2f::invalid = Vec2f(FLT_MAX, FLT_MAX);
const Vec2f Vec2f::right = Vec2f(1, 0);
const Vec2f Vec2f::up = Vec2f(0, 1);
const Vec2f Vec2f::zero = Vec2f(0, 0);
const Vec2f Vec2f::one = Vec2f(1, 1);