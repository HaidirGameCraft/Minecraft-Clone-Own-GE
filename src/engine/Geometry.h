#pragma once

#include <engine/Mesh.h>

enum GeometryType {
    Plane,
    Cube,
    DefCube
};

class Geometry : public Mesh
{
public:
    Geometry(GeometryType type);
};