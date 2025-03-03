#pragma once

#include <vector>
#include <engine/Vector.h>

class Mesh 
{
private:
    unsigned int buffers[4];
    unsigned int vao_id;
public:
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> uvs;
    std::vector<unsigned int> indices;

    void Clear();
    void ClearData();

    void SetVertices(std::vector<Vec3> vertices);
    void SetNormals(std::vector<Vec3> normals);
    void SetUvs(std::vector<Vec3> uvs);
    void SetIndices(std::vector<unsigned int> indices);

    void GenVBO();
    unsigned int GetID() const;
};