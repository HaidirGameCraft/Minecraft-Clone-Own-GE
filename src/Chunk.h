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
#include <optional>


#include <Block.h>

#define MAX_WIDTH 16
#define MAX_DEPTH 16
#define MAX_HEIGHT 16
#define MID_HEIGHT 64

class ChunkManager;

class Chunk {
public:
    Vec3 position;
    std::optional<GameObject> object;

    Chunk* left;
    Chunk* right;
    Chunk* front;
    Chunk* back;
    ChunkManager* manager;

    Mesh mesh;
    BlockType blocks[MAX_DEPTH][MAX_WIDTH][MAX_HEIGHT];
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

    std::vector<BlockCollider> GetBlockCollider();
    std::vector<BlockCollider> GetAllBlockCollider();

    void CheckAllBlock();

    void GenerateChunks();
    void GenerateChunk();
    std::string Index();
};

class ChunkManager {
public:
    std::map<std::string, Chunk> chunks;
    int max_width;
    int max_height;
    int max_depth;
    Material* material;
    
    ChunkManager(int max_width, int max_height, int max_depth);
    ~ChunkManager();
    Chunk* FindChunk(Vec3 idx);
    void GenerateChunks(Material* material);
    void UpdateGenerateChunk(int x, int z);
    void UpdatePlayerChunk(Camera* camera);
    void GenerateChunk(int x, int z);
    void ChunkRender(OpenGLRenderer& renderer, Camera* camera, Transform* transform);
};