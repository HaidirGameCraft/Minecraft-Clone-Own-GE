#pragma once

#include <engine/Vector.h>
#include <stdint.h>
#include <map>
#include <memory>

enum BlockType {
    None = 0,
    Grass,
    Dirt,
    Stone,
    Log,
    Diamond,
};

class BlockCollider {
public:
    Vec3 min;
    Vec3 max;
    Vec3 index;
};

class Block {
public:
    uint16_t id;
    BlockType type;
    const char* name;

    Vec3 texLeft;
    Vec3 texRight;
    Vec3 texTop;
    Vec3 texBottom;
    Vec3 texFront;
    Vec3 texBack;

    bool left_block;
    bool right_block;
    bool top_block;
    bool bottom_block;
    bool front_block;
    bool back_block;

    Block();
    Block(BlockType type);
    void SetType(BlockType type);

    static void InitializeBlocks();
    static std::map<BlockType, Block> list_block;
    static Block* GetBlock(BlockType type);
};