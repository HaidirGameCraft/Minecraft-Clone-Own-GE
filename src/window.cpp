#include <glad/glad.h>
#include <engine/window.h>
#include <engine/Camera.h>
#include <engine/Shader.h>
#include <engine/GameObject.h>
#include <engine/Geometry.h>
#include <engine/Collider.h>
#include <engine/Frustum.h>
#include <engine/RenderList.h>

#include <Chunk.h>
#include <Block.h>
#include <Player.h>
#include <iostream>
#include <chrono>

#include <thread>
#include <mutex>

std::vector<ShaderFile> shader_files;

char keys[256] = {0};
char buttons[12] = {0};

void glfwKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if( action == GLFW_PRESS && key == GLFW_KEY_ESCAPE )
        glfwSetWindowShouldClose(window, true);

    if( action == GLFW_PRESS )
        keys[key] = true;
    else if ( action == GLFW_RELEASE )
        keys[key] = false;
}

void glfwMouseEvent(GLFWwindow* window, int button, int action, int mods)
{
    if( action == GLFW_PRESS )
        buttons[button] = true;
    else if (action == GLFW_RELEASE)
        buttons[button] = false;
}

void ErrorCallBack(int error_code, const char* message )
{
    std::cerr << " Error Code: " << error_code << ", " << message << std::endl;
}

Window::Window(const char* text, int width, int height): renderer(width, height)
{
    this->text = text;
    this->width = width;
    this->height = height;
}

void Window::SetSize(int width, int height)
{
    this->width = width;
    this->height = height;
}

int Window::Initialize()
{
    if( !glfwInit() )
    {
        std::cerr << "Err> Cannot Initialize GLFW Window" << std::endl;
        return 1;
    }
    glfwSetErrorCallback(ErrorCallBack);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    //glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);


    this->window = glfwCreateWindow(this->width, this->height, this->text, NULL, NULL);

    if( !this->window )
    {
        std::cerr << "Err> Cannot creating new Window" << std::endl;
        glfwTerminate();
        return 1;
    }

    // Initialize Keyboard
    glfwSetKeyCallback(this->window, glfwKeyEvent);
    glfwSetMouseButtonCallback(this->window, glfwMouseEvent);

    glfwMakeContextCurrent(window);
    
    
    if( !gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        glfwTerminate();
        std::cerr << "Err> Cannot Initialize GLAD in GLFW" << std::endl;
        return 1;
    }

    glfwSwapInterval(1);

    return 0;
}

ChunkManager chunkmanager(5, 8, 5);
PerspectiveCamera camera(55, 800 / 600, 0.01, 1000);
bool is_quit = false;
std::mutex mtx_chunk;

int64_t GetMiliseconds() {
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
}

void Window::Render()
{
    
    // Initialize Shader
    shader_files.push_back(ShaderFile{"BasicShader", "./shadercode/vertex.glsl", "./shadercode/fragment.glsl"});

    ShaderProgram::CreatePrograms(shader_files);

    float speedCameraMove = 0.5;
    float sensitivity = 0.08;

    Material material;
    material.color = Color(0.0f, 1.0f, 0.0f, 1.0f);
    material.id_program = ShaderProgram::program_lists["BasicShader"];

    Texture texture("./tilemap.png");
    material.texture = &texture;

    
    // Blocks
    Block::InitializeBlocks();
    
    // Chunk
    chunkmanager.GenerateChunks(&material);
    
    auto chunk_thread_handle = [=]() -> void {
        while( !is_quit ) {
            std::lock_guard<std::mutex> lock(mtx_chunk);
            chunkmanager.UpdatePlayerChunk(&camera);
        }
    };

    std::thread cthread(chunk_thread_handle);
    

    Player player;
    player.object.transform.position.y = 84;
    
    camera.transform.position.y = 6 * MAX_HEIGHT + MAX_HEIGHT;
    // camera.transform.position.x = 5 * 8 / 2;
    camera.transform.position.z = 16;

    std::shared_ptr<Light> light = std::make_shared<Light>();
    light->transform.position = Vec3(0.9f, 0.8f, 0.7f);
    GLRenderList::light_list.push_back( light );

    this->renderer.SetColor(0, 0.5, 0.8, 1.0);

    camera.Create_Projection();
    camera.UpdateMatrix();
    std::cout << "Projection Matrix: \n" << camera.transform.world_matrix * camera.transform.projection_matrix << std::endl;
    
    
    Frustum frustum;
    frustum.CalculateFrustum(camera);
    camera.frustum = &frustum;

    Mesh box = Geometry(Cube);
    GameObject cube_object(&box);
    cube_object.material = &material;
    cube_object.transform.position.z = -10;

    BoxCollider collider;
    collider.CalculateGameObject(cube_object);


    double xpos = 0, ypos = 0;
    double pxpos = 0, pypos = 0;
    Vec3 forward(0, 0, 1);
    Vec3 up(0, 1, 0);
    Vec3 right(1, 0, 0);
    bool is_right_click = false;
    bool is_left_click = false;

    // Frame
    int frames = 0;
    int64_t begin_time = GetMiliseconds();
    int64_t prev_time = begin_time;

    /* Render Object */
    while( !glfwWindowShouldClose(this->window) )
    {

        begin_time = GetMiliseconds();

        glfwGetWindowSize(this->window, &this->width, &this->height);
        camera.aspect = (float)this->width / (float)this->height;
        this->renderer.width = this->width;
        this->renderer.height = this->height;

        
        glfwGetCursorPos(this->window, &xpos, &ypos);
        if( buttons[GLFW_MOUSE_BUTTON_LEFT] )
        {
            double xMove = xpos - pxpos;
            double yMove = ypos - pypos;
            Vec3 movement(xMove, yMove, 0);
            movement.Normalize();

            camera.transform.rotation.x += movement.y * 2 * sensitivity * 0.4;
            camera.transform.rotation.y -= movement.x * 2 * sensitivity * 0.4;

            Mat4 mat;
            mat.Identify();
            Mat4::Rotation(mat, camera.transform.rotation, &mat);

            forward = Vec3(0, 0, 1) * mat;
            up = Vec3(0, 1, 0) * mat;
            right.CrossProduct(up, forward);

            pxpos = xpos;
            pypos = ypos;
        } else {
            pxpos = xpos;
            pypos = ypos;
        }

        if( keys[GLFW_KEY_A] || keys[GLFW_KEY_D] )
        {
            camera.transform.position.x += camera.transform.right.x * (keys[GLFW_KEY_A] ? -speedCameraMove : speedCameraMove);
            //camera.transform.position.y += right.y * (keys[GLFW_KEY_A] ? -speedCameraMove : speedCameraMove);
            camera.transform.position.z += camera.transform.right.z * (keys[GLFW_KEY_A] ? -speedCameraMove : speedCameraMove);
        }

        if( keys[GLFW_KEY_W] || keys[GLFW_KEY_S] ) {
            camera.transform.position.x += forward.x * (keys[GLFW_KEY_W] ? -speedCameraMove : speedCameraMove);
            //camera.transform.position.y += forward.y * (keys[GLFW_KEY_W] ? -speedCameraMove : speedCameraMove);
            camera.transform.position.z += forward.z * (keys[GLFW_KEY_W] ? -speedCameraMove : speedCameraMove);
        }

        if( keys[GLFW_KEY_Q] || keys[GLFW_KEY_E] )
        {
            camera.transform.position.y += keys[GLFW_KEY_E] ? speedCameraMove : -speedCameraMove;
        }


        if( keys[GLFW_KEY_J] || keys[GLFW_KEY_L] )
            player.object.transform.position.x += keys[GLFW_KEY_J] ? -0.1f : 0.1f;

        if( keys[GLFW_KEY_I] || keys[GLFW_KEY_K] )
            player.object.transform.position.z += keys[GLFW_KEY_K] ? -0.1f : 0.1f;
        

        this->renderer.Render(&camera);
        frustum.CalculateFrustum(camera);

        chunkmanager.ChunkRender(renderer, &camera, &camera.transform);

        renderer.RenderObject(&player.object, &camera);
        player.UpdatePhysics(&chunkmanager);

        if( buttons[GLFW_MOUSE_BUTTON_RIGHT] )
        {
            if( is_right_click == false )
            {
                //player.PlaceBlock(&chunkmanager, camera, forward, Grass);
                is_right_click = true;
            }
        } else {
            is_right_click = false;
        }

        if( buttons[GLFW_MOUSE_BUTTON_LEFT] )
        {
            if( is_left_click == false )
            {
                //player.DestroyBlock(&chunkmanager, camera, forward);
                is_left_click = true;
            }
        } else {
            is_left_click = false;
        }


        //std::cout << "Camera: " << camera.transform.position << std::endl;

        glfwSwapBuffers(window);
        glfwPollEvents();

        if( keys[GLFW_KEY_P] ) {
            glfwSetWindowShouldClose(this->window, true);
            break;
        }

        int64_t deltaTime = begin_time - prev_time;
        frames++;
        if( deltaTime > 1000 )
        {
            double fps = ( (double) frames / (double) deltaTime ) * 1000.0;
            std::cout << "FPS Counter: " << fps << std::endl;

            frames = 0;
            prev_time = begin_time;
        }
    }
    is_quit = true;
    cthread.join();
    //chunkmanager.~ChunkManager();

    printf("OpenGL End");
    GLRenderList::clear();
}

void Window::Clear()
{
    glfwDestroyWindow( this->window );
    glfwTerminate();
}