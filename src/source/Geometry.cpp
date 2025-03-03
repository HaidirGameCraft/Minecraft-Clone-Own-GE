#include <engine/Geometry.h>

Geometry::Geometry(GeometryType type)
{
    std::vector<Vec3> vertices;
    std::vector<Vec3> normals;
    std::vector<Vec3> uvs;
    std::vector<unsigned int> indices;

    if( type == GeometryType::Plane )
    {
        vertices.push_back( Vec3(-1.0f, 0.0f, 1.0f) );
        vertices.push_back( Vec3(-1.0f, 0.0f,-1.0f) );
        vertices.push_back( Vec3( 1.0f, 0.0f,-1.0f) );
        vertices.push_back( Vec3( 1.0f, 0.0f, 1.0f) );

        normals.push_back( Vec3(0.0f, 1.0f, 0.0f) );
        normals.push_back( Vec3(0.0f, 1.0f, 0.0f) );
        normals.push_back( Vec3(0.0f, 1.0f, 0.0f) );
        normals.push_back( Vec3(0.0f, 1.0f, 0.0f) );

        uvs.push_back( Vec3(0.0f, 1.0f, 0.0f) );
        uvs.push_back( Vec3(0.0f, 0.0f, 0.0f) );
        uvs.push_back( Vec3(1.0f, 0.0f, 0.0f) );
        uvs.push_back( Vec3(1.0f, 1.0f, 0.0f) );

        indices.push_back(0);
        indices.push_back(1);
        indices.push_back(2);
        indices.push_back(0);
        indices.push_back(2);
        indices.push_back(3);
    } else if ( type == GeometryType::Cube )
    {
        // Left Plane
        vertices.push_back( Vec3(-1.0f,-1.0f,-1.0f) );
        vertices.push_back( Vec3(-1.0f, 1.0f,-1.0f) );
        vertices.push_back( Vec3(-1.0f, 1.0f, 1.0f) );
        vertices.push_back( Vec3(-1.0f,-1.0f, 1.0f) );

        normals.push_back( Vec3(-1.0f, 0.0f, 0.0f) );
        normals.push_back( Vec3(-1.0f, 0.0f, 0.0f) );
        normals.push_back( Vec3(-1.0f, 0.0f, 0.0f) );
        normals.push_back( Vec3(-1.0f, 0.0f, 0.0f) );

        // Right Plane
        vertices.push_back( Vec3( 1.0f,-1.0f, 1.0f) );
        vertices.push_back( Vec3( 1.0f, 1.0f, 1.0f) );
        vertices.push_back( Vec3( 1.0f, 1.0f,-1.0f) );
        vertices.push_back( Vec3( 1.0f,-1.0f,-1.0f) );

        normals.push_back( Vec3( 1.0f, 0.0f, 0.0f) );
        normals.push_back( Vec3( 1.0f, 0.0f, 0.0f) );
        normals.push_back( Vec3( 1.0f, 0.0f, 0.0f) );
        normals.push_back( Vec3( 1.0f, 0.0f, 0.0f) );

        // Front Plane
        vertices.push_back( Vec3(-1.0f,-1.0f, 1.0f) );
        vertices.push_back( Vec3(-1.0f, 1.0f, 1.0f) );
        vertices.push_back( Vec3( 1.0f, 1.0f, 1.0f) );
        vertices.push_back( Vec3( 1.0f,-1.0f, 1.0f) );

        normals.push_back( Vec3(0.0f, 0.0f, 1.0f) );
        normals.push_back( Vec3(0.0f, 0.0f, 1.0f) );
        normals.push_back( Vec3(0.0f, 0.0f, 1.0f) );
        normals.push_back( Vec3(0.0f, 0.0f, 1.0f) );

        // Back Plane
        vertices.push_back( Vec3( 1.0f,-1.0f,-1.0f) );
        vertices.push_back( Vec3( 1.0f, 1.0f,-1.0f) );
        vertices.push_back( Vec3(-1.0f, 1.0f,-1.0f) );
        vertices.push_back( Vec3(-1.0f,-1.0f,-1.0f) );

        normals.push_back( Vec3(0.0f, 0.0f,-1.0f) );
        normals.push_back( Vec3(0.0f, 0.0f,-1.0f) );
        normals.push_back( Vec3(0.0f, 0.0f,-1.0f) );
        normals.push_back( Vec3(0.0f, 0.0f,-1.0f) );

        // Top Plane
        vertices.push_back( Vec3(-1.0f, 1.0f, 1.0f) );
        vertices.push_back( Vec3(-1.0f, 1.0f,-1.0f) );
        vertices.push_back( Vec3( 1.0f, 1.0f,-1.0f) );
        vertices.push_back( Vec3( 1.0f, 1.0f, 1.0f) );

        normals.push_back( Vec3(0.0f, 1.0f, 0.0f) );
        normals.push_back( Vec3(0.0f, 1.0f, 0.0f) );
        normals.push_back( Vec3(0.0f, 1.0f, 0.0f) );
        normals.push_back( Vec3(0.0f, 1.0f, 0.0f) );

        // Bottom Plane
        vertices.push_back( Vec3(-1.0f,-1.0f,-1.0f) );
        vertices.push_back( Vec3(-1.0f,-1.0f, 1.0f) );
        vertices.push_back( Vec3( 1.0f,-1.0f, 1.0f) );
        vertices.push_back( Vec3( 1.0f,-1.0f,-1.0f) );

        normals.push_back( Vec3(0.0f,-1.0f, 0.0f) );
        normals.push_back( Vec3(0.0f,-1.0f, 0.0f) );
        normals.push_back( Vec3(0.0f,-1.0f, 0.0f) );
        normals.push_back( Vec3(0.0f,-1.0f, 0.0f) );

        for(int i = 0; i < 6; i++)
        {
            uvs.push_back( Vec3(0.0f, 1.0f, 0.0f) );
            uvs.push_back( Vec3(0.0f, 0.0f, 0.0f) );
            uvs.push_back( Vec3(1.0f, 0.0f, 0.0f) );
            uvs.push_back( Vec3(1.0f, 1.0f, 0.0f) );
            
            indices.push_back(i * 4 + 0);
            indices.push_back(i * 4 + 1);
            indices.push_back(i * 4 + 2);
            indices.push_back(i * 4 + 0);
            indices.push_back(i * 4 + 2);
            indices.push_back(i * 4 + 3);
        }
    } else if ( type == GeometryType::DefCube )
    {
        // Left Plane
        vertices.push_back( Vec3(0, 0.0f, 0.0f) );
        vertices.push_back( Vec3(0, 1.0f, 0.0f) );
        vertices.push_back( Vec3(0, 1.0f, 1.0f) );
        vertices.push_back( Vec3(0, 0.0f, 1.0f) );

        normals.push_back( Vec3(-1.0f, 0.0f, 0.0f) );
        normals.push_back( Vec3(-1.0f, 0.0f, 0.0f) );
        normals.push_back( Vec3(-1.0f, 0.0f, 0.0f) );
        normals.push_back( Vec3(-1.0f, 0.0f, 0.0f) );

        // Right Plane
        vertices.push_back( Vec3( 1.0f, 0.0f, 1.0f) );
        vertices.push_back( Vec3( 1.0f, 1.0f, 1.0f) );
        vertices.push_back( Vec3( 1.0f, 1.0f, 0.0f) );
        vertices.push_back( Vec3( 1.0f, 0.0f, 0.0f) );

        normals.push_back( Vec3( 1.0f, 0.0f, 0.0f) );
        normals.push_back( Vec3( 1.0f, 0.0f, 0.0f) );
        normals.push_back( Vec3( 1.0f, 0.0f, 0.0f) );
        normals.push_back( Vec3( 1.0f, 0.0f, 0.0f) );

        // Front Plane
        vertices.push_back( Vec3( 0.0f, 0.0f, 1.0f) );
        vertices.push_back( Vec3( 0.0f, 1.0f, 1.0f) );
        vertices.push_back( Vec3( 1.0f, 1.0f, 1.0f) );
        vertices.push_back( Vec3( 1.0f, 0.0f, 1.0f) );

        normals.push_back( Vec3(0.0f, 0.0f, 1.0f) );
        normals.push_back( Vec3(0.0f, 0.0f, 1.0f) );
        normals.push_back( Vec3(0.0f, 0.0f, 1.0f) );
        normals.push_back( Vec3(0.0f, 0.0f, 1.0f) );

        // Back Plane
        vertices.push_back( Vec3( 1.0f, 0.0f, 0.0f) );
        vertices.push_back( Vec3( 1.0f, 1.0f, 0.0f) );
        vertices.push_back( Vec3( 0.0f, 1.0f, 0.0f) );
        vertices.push_back( Vec3( 0.0f, 0.0f, 0.0f) );

        normals.push_back( Vec3(0.0f, 0.0f,-1.0f) );
        normals.push_back( Vec3(0.0f, 0.0f,-1.0f) );
        normals.push_back( Vec3(0.0f, 0.0f,-1.0f) );
        normals.push_back( Vec3(0.0f, 0.0f,-1.0f) );

        // Top Plane
        vertices.push_back( Vec3( 0.0f, 1.0f, 1.0f) );
        vertices.push_back( Vec3( 0.0f, 1.0f, 0.0f) );
        vertices.push_back( Vec3( 1.0f, 1.0f, 0.0f) );
        vertices.push_back( Vec3( 1.0f, 1.0f, 1.0f) );

        normals.push_back( Vec3(0.0f, 1.0f, 0.0f) );
        normals.push_back( Vec3(0.0f, 1.0f, 0.0f) );
        normals.push_back( Vec3(0.0f, 1.0f, 0.0f) );
        normals.push_back( Vec3(0.0f, 1.0f, 0.0f) );

        // Bottom Plane
        vertices.push_back( Vec3( 0.0f, 0.0f, 0.0f) );
        vertices.push_back( Vec3( 0.0f, 0.0f, 1.0f) );
        vertices.push_back( Vec3( 1.0f, 0.0f, 1.0f) );
        vertices.push_back( Vec3( 1.0f, 0.0f, 0.0f) );

        normals.push_back( Vec3(0.0f,-1.0f, 0.0f) );
        normals.push_back( Vec3(0.0f,-1.0f, 0.0f) );
        normals.push_back( Vec3(0.0f,-1.0f, 0.0f) );
        normals.push_back( Vec3(0.0f,-1.0f, 0.0f) );

        for(int i = 0; i < 6; i++)
        {
            uvs.push_back( Vec3(0.0f, 1.0f, 0.0f) );
            uvs.push_back( Vec3(0.0f, 0.0f, 0.0f) );
            uvs.push_back( Vec3(1.0f, 0.0f, 0.0f) );
            uvs.push_back( Vec3(1.0f, 1.0f, 0.0f) );
            
            indices.push_back(i * 4 + 0);
            indices.push_back(i * 4 + 1);
            indices.push_back(i * 4 + 2);
            indices.push_back(i * 4 + 0);
            indices.push_back(i * 4 + 2);
            indices.push_back(i * 4 + 3);
        }
    }

    this->SetVertices(vertices);
    this->SetNormals(normals);
    this->SetUvs(uvs);
    this->SetIndices(indices);
}