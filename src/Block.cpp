#include <Block.h>

std::map<BlockType, Block> Block::list_block;

Block::Block(BlockType type) : type(type) {
    this->SetType(type);
}

Block::Block() {
    this->left_block = 0;
    this->right_block = 0;
    this->top_block = 0;
    this->bottom_block = 0;
    this->front_block = 0;
    this->back_block = 0;
};

void Block::SetType(BlockType type) {
    this->type = type;

    switch (this->type)
    {
    case Grass:
        this->texLeft   = Vec3(1, 3, 0);
        this->texRight  = Vec3(1, 3, 0);
        this->texTop    = Vec3(2, 3, 0);
        this->texBottom = Vec3(1, 2, 0);
        this->texFront  = Vec3(1, 3, 0);
        this->texBack   = Vec3(1, 3, 0);
        break;
    case Dirt:
        this->texLeft   = Vec3(1, 2, 0);
        this->texRight  = Vec3(1, 2, 0);
        this->texTop    = Vec3(1, 2, 0);
        this->texBottom = Vec3(1, 2, 0);
        this->texFront  = Vec3(1, 2, 0);
        this->texBack   = Vec3(1, 2, 0);
        break;
    case Stone:
        this->texLeft   = Vec3(1, 0, 0);
        this->texRight  = Vec3(1, 0, 0);
        this->texTop    = Vec3(1, 0, 0);
        this->texBottom = Vec3(1, 0, 0);
        this->texFront  = Vec3(1, 0, 0);
        this->texBack   = Vec3(1, 0, 0);
        break;
    case Log:
        this->texLeft   = Vec3(4, 0, 0);
        this->texRight  = Vec3(4, 0, 0);
        this->texTop    = Vec3(5, 0, 0);
        this->texBottom = Vec3(5, 0, 0);
        this->texFront  = Vec3(4, 0, 0);
        this->texBack   = Vec3(4, 0, 0);
        break;
    case Diamond:
        this->texLeft   = Vec3(8, 7, 0);
        this->texRight  = Vec3(8, 7, 0);
        this->texTop    = Vec3(8, 7, 0);
        this->texBottom = Vec3(8, 7, 0);
        this->texFront  = Vec3(8, 7, 0);
        this->texBack   = Vec3(8, 7, 0);
        break;
    
    default:
        break;
    }
}

void Block::InitializeBlocks() {
    list_block[BlockType::None] =  Block(BlockType::None);
    list_block[BlockType::Grass] =  Block(BlockType::Grass);
    list_block[BlockType::Dirt] =  Block(BlockType::Dirt);
    list_block[BlockType::Stone] =  Block(BlockType::Stone);
    list_block[BlockType::Log] = Block(BlockType::Log);
    list_block[BlockType::Diamond] = Block(BlockType::Diamond);
}

Block* Block::GetBlock(BlockType type) {
    return list_block.find(type) != list_block.end() ? &list_block[type] : nullptr;
}