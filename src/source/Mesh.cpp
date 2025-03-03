#include <engine/Mesh.h>
#include <glad/glad.h>
#include <stdio.h>

void Mesh::SetVertices(std::vector<Vec3> vertices) {
    this->vertices.clear();
    for(int i = 0; i < vertices.size(); i++)
    {
        this->vertices.push_back( vertices[i].x );
        this->vertices.push_back( vertices[i].y );
        this->vertices.push_back( vertices[i].z );
    }
}
void Mesh::SetNormals(std::vector<Vec3> normals) {
    this->normals.clear();
    for(int i = 0; i < normals.size(); i++)
    {
        this->normals.push_back( normals[i].x );
        this->normals.push_back( normals[i].y );
        this->normals.push_back( normals[i].z );
    }
}
void Mesh::SetUvs(std::vector<Vec3> uvs) {
    this->uvs.clear();
    for(int i = 0; i < uvs.size(); i++)
    {
        this->uvs.push_back( uvs[i].x );
        this->uvs.push_back( uvs[i].y );
    }
}
void Mesh::SetIndices(std::vector<unsigned int> indices) {
    this->indices = indices;
}

void Mesh::GenVBO()
{

    if( glIsVertexArray(vao_id) )
        return;
        
    glGenBuffers(4, this->buffers);
    glCreateVertexArrays(1, &vao_id);
    glBindVertexArray(vao_id);

    if( this->vertices.size() != 0 )
    {
        glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
        glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(float), this->vertices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, (void*) 0);
        glEnableVertexAttribArray(0);
    } 
    
    if ( this->normals.size() != 0 )
    {
        glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
        glBufferData(GL_ARRAY_BUFFER, this->normals.size() * sizeof(float), this->normals.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, false, 0, 0);
    } 
    
    if( this->uvs.size() != 0 )
    {
        glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
        glBufferData(GL_ARRAY_BUFFER, this->uvs.size() * sizeof(float), this->uvs.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, false, 0, 0);
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[3]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(unsigned int), this->indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(NULL);
}

void Mesh::Clear() {
    glDeleteVertexArrays(1, &this->vao_id);
    glDeleteBuffers(4, this->buffers);
}

void Mesh::ClearData() {
    this->vertices.clear();
    this->uvs.clear();
    this->normals.clear();
    this->indices.clear();
}

unsigned int Mesh::GetID() const {
    return this->vao_id;
}