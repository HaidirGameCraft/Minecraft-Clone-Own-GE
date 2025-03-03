#include <glad/glad.h>

#include <string>

#include <iostream>

#include <engine/OpenGL.h>
#include <engine/Camera.h>
#include <engine/Shader.h>
#include <engine/RenderList.h>

using namespace std;

void LightRender(int program)
{
    for(int i = 0; i < GLRenderList::light_list.size(); i++)
    {
        Light* light = GLRenderList::light_list[i].get();
        string lightstr = "lights[" + to_string(i) + "]";

        ShaderProgram::SetColor(program, (lightstr + ".color"), light->color);
        ShaderProgram::SetVector3(program, (lightstr + ".position"), light->transform.position);
        ShaderProgram::SetBool(program, (lightstr + ".use"), 1);
    }
}

void OpenGLRenderer::RenderObject(GameObject* object, Camera* camera)
{
    object->UpdateMatrix();
    object->mesh->GenVBO();

    Mesh* mesh = object->mesh;
    Material* material = object->material;

    glBindVertexArray(object->mesh->GetID() );
    glUseProgram( material->id_program );



    int projection_position = glGetUniformLocation(material->id_program, "projection_matrix");
    int world_inverse = glGetUniformLocation(material->id_program, "world_matrix");
    int modelView_matrix = glGetUniformLocation(material->id_program, "modelView_matrix");

    glUniformMatrix4fv(modelView_matrix, 1, false, object->transform.local_matrix.m);
    glUniformMatrix4fv(projection_position, 1, false, camera->transform.projection_matrix.m);
    glUniformMatrix4fv(world_inverse, 1, false, camera->transform.world_matrix.m);

    ShaderProgram::SetVector3(material->id_program, "CameraView", camera->transform.position);
    // Material Configuration for Object
    ShaderProgram::SetColor(material->id_program, "object_color", material->color);

    if( material->texture != nullptr )
    {
        material->texture->GenerateTexture();

        ShaderProgram::SetBool(material->id_program, "use_texture", true);
        ShaderProgram::SetInt(material->id_program, "texture1", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, material->texture->GetID());
    }

    LightRender( material->id_program );

    if( mesh->indices.size() != 0 )
    {
        glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, 0);
    }

    if( material->texture != nullptr )
    {
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    glBindVertexArray(NULL);
    glUseProgram(NULL);
}

OpenGLRenderer::OpenGLRenderer(int width, int height)
{
    this->width = width;
    this->height = height;

}
void OpenGLRenderer::SetColor(float r, float g, float b, float a)
{
    this->color.r = r;
    this->color.g = g;
    this->color.b = b;
    this->color.a = a;
}

void OpenGLRenderer::Render(PerspectiveCamera* camera)
{
    glViewport(0, 0, this->width, this->height);
    glClearColor(this->color.r, this->color.g, this->color.b, this->color.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glDepthFunc(GL_LESS);
    glCullFace(GL_FRONT);

    // Sort Objects
    // Next time    

    camera->Create_Projection();
    camera->UpdateMatrix();

    for(int i = 0; i < GLRenderList::gameobject_list.size(); i++)
    {
        GameObject* gameObject = GLRenderList::gameobject_list[i].get();
        RenderObject(gameObject, camera);
    }

}