#pragma once

#include <engine/Vector.h>
#include <Chunk.h>
#include <string>
#include <engine/GameObject.h>
#include <map>

class Player {
public:
    Vec3 position;
    Vec3 velocity;
    float gravity = 9.81f;

    float width = 0.5f;
    float height = 2.0f;

    int collider_distance = 3;
    std::map<std::string, std::vector<BlockCollider>> colliders;

    GameObject object;
    Mesh mesh;
    Material material;
    Player();
    void DestroyBlock(ChunkManager* mngr, Camera& camera, Vec3 forward);
    void PlaceBlock(ChunkManager* mngr, Camera& camera, Vec3 forward, BlockType type);
    void UpdatePhysics(ChunkManager* mngr);
};