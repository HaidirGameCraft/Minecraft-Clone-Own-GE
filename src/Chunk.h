#pragma once

#include <stdint.h>
#include <engine/OpenGL.h>
#include <engine/Vector.h>
#include <engine/Mesh.h>
#include <engine/Material.h>
#include <engine/Collider.h>

#include <map>
#include <string>
#include <vector>


#include <Block.h>

#define MAX_WIDTH 16
#define MAX_DEPTH 16
#define MAX_HEIGHT 128
#define MID_HEIGHT 64

class Chunk {
public:
    Vec3 position;

    std::vector<Vec3> vertices;
    std::vector<Vec3> normals;
    std::vector<Vec3> uvs;
    std::vector<uint32_t> indices;

    GameObject object;

    Chunk* left;
    Chunk* right;
    Chunk* front;
    Chunk* back;

    Mesh mesh;
    BlockType blocks[MAX_DEPTH][MAX_WIDTH][MAX_HEIGHT];
    std::vector<BlockCollider> colliders;
    Chunk();
    ~Chunk();

    void Gen();
    void Regenerate();
    void UvsPlane(int x, int y, int texWidth, int texHeight, int w, int h);
    void LeftPlane(Vec3& position);
    void RightPlane(Vec3& position);
    void FrontPlane(Vec3& position);
    void BackPlane(Vec3& position);
    void TopPlane(Vec3& position);
    void BottomPlane(Vec3& position);

    void GetBlockCollider();
    std::vector<BlockCollider> GetAllBlockCollider();

    void CheckAllBlock();

    void GenerateChunk(Chunk* left, Chunk* right, Chunk* front, Chunk* back);
    void GenerateChunk();
    std::string Index();
};

class ChunkManager {
public:
    std::map<std::string, Chunk> chunks;
    int max_width;
    int max_depth;
    Material* material;
    
    ChunkManager(int max_width, int max_depth);
    ~ChunkManager();
    Chunk* FindChunk(Vec3 idx);
    void GenerateChunks(Material* material);
    void UpdateGenerateChunk(int x, int z);
    void GenerateChunk(int x, int z);
    void ChunkRender(OpenGLRenderer& renderer, Camera* camera, Transform* transform);
};