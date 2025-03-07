#include <Chunk.h>
#include <cmath>
#include <iostream>
#include <engine/GameObject.h>
#include <engine/Collider.h>
#include <engine/Mesh.h>
#include <engine/Frustum.h>

#include <SimplexNoise.h>

#define DB_PERLIN_IMPL
#include "perlin.hpp"

#define PUSH_VERTICES(x, y, z)  this->mesh.vertices.push_back(x); \
                                this->mesh.vertices.push_back(y); \
                                this->mesh.vertices.push_back(z);
#define PUSH_NORMALS(x, y, z)   this->mesh.normals.push_back(x); \
                                this->mesh.normals.push_back(y); \
                                this->mesh.normals.push_back(z);
#define PUSH_UVS(x, y)          this->mesh.uvs.push_back(x); \
                                this->mesh.uvs.push_back(y);

#define MAX_PERLIN_WIDTH (int) 0xffff
#define MAX_PERLIN_DEPTH (int) 0xffff
#define SCALE_PERLIN 0.1f
#define SCALE_PERLIN_CAVE 0.6f

#define INDEX(vec) std::to_string(vec.x) + + "/" + std::to_string(vec.y) + "/" + std::to_string(vec.z)
// Chunk code

void Chunk::LeftPlane(Vec3& position) {
    PUSH_VERTICES(position.x + 0, position.y +  0, position.z +  0);
    PUSH_VERTICES(position.x + 0, position.y +  1, position.z +  0);
    PUSH_VERTICES(position.x + 0, position.y +  1, position.z +  1);
    PUSH_VERTICES(position.x + 0, position.y +  0, position.z +  1);

    PUSH_NORMALS(-1, 0, 0);
    PUSH_NORMALS(-1, 0, 0);
    PUSH_NORMALS(-1, 0, 0);
    PUSH_NORMALS(-1, 0, 0);
}
void Chunk::RightPlane(Vec3& position) {
    PUSH_VERTICES(position.x +  1, position.y +  0, position.z +  1);
    PUSH_VERTICES(position.x +  1, position.y +  1, position.z +  1);
    PUSH_VERTICES(position.x +  1, position.y +  1, position.z +  0);
    PUSH_VERTICES(position.x +  1, position.y +  0, position.z +  0);

    PUSH_NORMALS( 1, 0, 0);
    PUSH_NORMALS( 1, 0, 0);
    PUSH_NORMALS( 1, 0, 0);
    PUSH_NORMALS( 1, 0, 0);
}
void Chunk::FrontPlane(Vec3& position) {
    PUSH_VERTICES(position.x +  0, position.y +  0, position.z +  1);
    PUSH_VERTICES(position.x +  0, position.y +  1, position.z +  1);
    PUSH_VERTICES(position.x +  1, position.y +  1, position.z +  1);
    PUSH_VERTICES(position.x +  1, position.y +  0, position.z +  1);

    PUSH_NORMALS(0, 0, 1);
    PUSH_NORMALS(0, 0, 1);
    PUSH_NORMALS(0, 0, 1);
    PUSH_NORMALS(0, 0, 1);
}
void Chunk::BackPlane(Vec3& position) {
    PUSH_VERTICES(position.x +  1, position.y +  0, position.z +  0);
    PUSH_VERTICES(position.x +  1, position.y +  1, position.z +  0);
    PUSH_VERTICES(position.x +  0, position.y +  1, position.z +  0);
    PUSH_VERTICES(position.x +  0, position.y +  0, position.z +  0);

    PUSH_NORMALS(0, 0, -1);
    PUSH_NORMALS(0, 0, -1);
    PUSH_NORMALS(0, 0, -1);
    PUSH_NORMALS(0, 0, -1);
}
void Chunk::TopPlane(Vec3& position) {
    PUSH_VERTICES(position.x +  0, position.y +  1, position.z +  1);
    PUSH_VERTICES(position.x +  0, position.y +  1, position.z +  0);
    PUSH_VERTICES(position.x +  1, position.y +  1, position.z +  0);
    PUSH_VERTICES(position.x +  1, position.y +  1, position.z +  1);

    PUSH_NORMALS(0, 1, 0);
    PUSH_NORMALS(0, 1, 0);
    PUSH_NORMALS(0, 1, 0);
    PUSH_NORMALS(0, 1, 0);
}
void Chunk::BottomPlane(Vec3& position) {
    PUSH_VERTICES(position.x +  0, position.y +  0, position.z +  0);
    PUSH_VERTICES(position.x +  0, position.y +  0, position.z +  1);
    PUSH_VERTICES(position.x +  1, position.y +  0, position.z +  1);
    PUSH_VERTICES(position.x +  1, position.y +  0, position.z +  0);

    PUSH_NORMALS(0, -1, 0);
    PUSH_NORMALS(0, -1, 0);
    PUSH_NORMALS(0, -1, 0);
    PUSH_NORMALS(0, -1, 0);
}

Chunk::Chunk() {
    
}

#define INDEX_BLOCK(x, y, z) (x) + (y) * MAX_WIDTH + (z) * MAX_HEIGHT * MAX_WIDTH

void Chunk::Gen() {
    for(int z = 0; z < MAX_DEPTH; z++)
    {
        for(int x = 0; x < MAX_WIDTH; x++)
        {
            for(int y = 0; y < MAX_HEIGHT; y++)
            {
                this->blocks[z][x][y] = BlockType::None;
            }
        }
    }

    // for(int z = 0; z < MAX_DEPTH; z++)
    // {
    //     for(int x = 0; x < MAX_WIDTH; x++)
    //     {
    //         int yIndex = std::floor(this->position.y / MAX_HEIGHT);
    //         int yOffset = (int)((int)this->position.y % MAX_HEIGHT);
    //         if( yIndex < std::floor((MID_HEIGHT - 20) / MAX_HEIGHT))
    //             for(int y = 0; y < MAX_HEIGHT; y++)
    //             {
    //                 this->blocks[z][x][y] = BlockType::Stone;
    //             }

    //         // float perlin = SimplexNoise::noise((position.x / MAX_WIDTH + (double) x / MAX_WIDTH) * SCALE_PERLIN, (position.z / MAX_DEPTH + (double) z / MAX_DEPTH) * SCALE_PERLIN);
    //         // float tree_perlin = db::perlin<float>((position.x / MAX_WIDTH + (double) x / MAX_WIDTH) * 0.1f, (position.z / MAX_DEPTH + (double) z / MAX_DEPTH) * 0.1f);
    //         // perlin = (perlin < 0) ? -perlin : perlin;

    //         // int higher_ground = 0;
    //         if( yIndex >= std::floor((MID_HEIGHT - 20) / MAX_HEIGHT))
    //         {
    //             for(int y = MID_HEIGHT - 20; y < (MID_HEIGHT - 20) + perlin * 20; y++)
    //             {
    //                 if( y < ((MID_HEIGHT - 20) + perlin * 20) - 1)
    //                     this->blocks[z][x][y] = BlockType::Dirt;
    //                 else
    //                 {
    //                     this->blocks[z][x][y] = BlockType::Grass;
    //                 }
    //             }
    //         }

    //         // for(int y = 0; y < MID_HEIGHT; y++)
    //         // {
    //         //     if( y > 4 )
    //         //     {
    //         //         float noise = SimplexNoise::noise((position.x / MAX_WIDTH + (double) x / MAX_WIDTH) * 0.5f, (float) y / MID_HEIGHT * 2 * 0.5f, (position.z / MAX_DEPTH + (double) z / MAX_DEPTH) * 0.5f);
    //         //         if( noise >= 0.2f && noise <= 0.3f && y < MID_HEIGHT - 10)
    //         //         {
    //         //             this->blocks[z][x][y] = BlockType::None;
    //         //         }

    //         //         noise = SimplexNoise::noise((position.x / MAX_WIDTH + (double) x / MAX_WIDTH) * 2, (float) y / MID_HEIGHT * 2 * 2, (position.z / MAX_DEPTH + (double) z / MAX_DEPTH) * 2);
    //         //         if( noise > 0.7f && y < MID_HEIGHT - 10)
    //         //         {
    //         //             this->blocks[z][x][y] = BlockType::None;
    //         //         }

    //         //         noise = SimplexNoise::noise((position.x / MAX_WIDTH + (double) x / MAX_WIDTH) * 2, (float) y / MID_HEIGHT * 2, (position.z / MAX_DEPTH + (double) z / MAX_DEPTH) * 2);
    //         //         if( noise > 0.94f && y < 10)
    //         //         {
    //         //             this->blocks[z][x][y] = BlockType::Diamond;
    //         //         }
    //         //     }
    //         // }

    //         // for(int y = 0; y < MID_HEIGHT - 10; y++)
    //         // {
    //         //     this->blocks[z][x][y] = BlockType::Stone;
    //         // }

            
    //         // // Stone
    //         // for(int y = 0; y < MID_HEIGHT - 10; y++)
    //         // {
    //         //     if( y >= MID_HEIGHT - 20)
    //         //         this->blocks[z][x][y] = BlockType::Dirt;
    //         //     else
    //         //         this->blocks[z][x][y] = BlockType::Stone;

    //         //     if( y >= 4 )
    //         //     {
    //         //         float noise = SimplexNoise::noise((position.x / MAX_WIDTH + (double) x / MAX_WIDTH) * SCALE_PERLIN_CAVE, (float) y / MID_HEIGHT * SCALE_PERLIN_CAVE, (position.z / MAX_DEPTH + (double) z / MAX_DEPTH) * SCALE_PERLIN_CAVE);
    //         //         if( noise > 0.6f )
    //         //             this->blocks[z][x][y] = BlockType::None;
    //         //     }
    //         // }
            

    //         // int perlin = db::perlin<double>((position.x / MAX_WIDTH + (double) x / MAX_WIDTH) * SCALE_PERLIN, (position.z / MAX_DEPTH + (double) z / MAX_DEPTH) * SCALE_PERLIN) * 20;


    //     }
    // }

    //CheckAllBlock();

    
}

void Chunk::Regenerate() {
    this->mesh.Clear();

    Chunk* left = this->manager->FindChunk((this->position / 16) + Vec3(-1, 0, 0));
    Chunk* right = this->manager->FindChunk((this->position / 16) + Vec3(1, 0, 0));
    Chunk* top = this->manager->FindChunk((this->position / 16) + Vec3(0, 1, 0));
    Chunk* bottom = this->manager->FindChunk((this->position / 16) + Vec3(0, -1, 0));
    Chunk* front = this->manager->FindChunk((this->position / 16) + Vec3(0, 0, 1));
    Chunk* back = this->manager->FindChunk((this->position / 16) + Vec3(0, 0,-1));

    this->GenerateChunk();
    if( left != nullptr )
    {
        //this->left->right = this;
        left->GenerateChunk();
    }
    
    if( right != nullptr )
    {
        //this->right->left = this;
        right->GenerateChunk();
    }
    
    if( front != nullptr )
    {
        //this->front->back = this;
        front->GenerateChunk();
    }

    if( back != nullptr )
    {
        //this->back->front = this;
        back->GenerateChunk();
    }

    if( top != nullptr )
    {
        //this->front->back = this;
        top->GenerateChunk();
    }

    if( bottom != nullptr )
    {
        //this->back->front = this;
        bottom->GenerateChunk();
    }
}

std::vector<BlockCollider> Chunk::GetBlockCollider() {
    std::vector<BlockCollider> colliders;
    for(int z = 0; z < MAX_DEPTH; z++)
    {
        for(int x = 0; x < MAX_WIDTH; x++)
        {
            for(int y = 0; y < MAX_HEIGHT; y++)
            {
                if( this->blocks[z][x][y] == BlockType::None )
                    continue;

                BlockCollider collide;
                collide.index = Vec3(x, y, z);
                collide.min = this->position + Vec3(x, y, z);
                collide.max = this->position + Vec3(x + 1, y + 1, z + 1);
                colliders.push_back( collide );
            }
        }
    }
    return colliders;
}

std::vector<BlockCollider> Chunk::GetAllBlockCollider() {
    std::vector<BlockCollider> colliders;
    for(int z = 0; z < MAX_DEPTH; z++)
    {
        for(int x = 0; x < MAX_WIDTH; x++)
        {
            for(int y = 0; y < MAX_HEIGHT; y++)
            {

                BlockCollider collide;
                collide.index = Vec3(x, y, z);
                collide.min = this->position + Vec3(x, y, z);
                collide.max = this->position + Vec3(x + 1, y + 1, z + 1);
                colliders.push_back( collide );
            }
        }
    }
    return colliders;
}

void Chunk::CheckAllBlock() {
    for(int z = 0; z < MAX_DEPTH; z++)
    {
        for(int x = 0; x < MAX_WIDTH; x++)
        {
            for(int y = 0; y < MAX_HEIGHT; y++)
            {
                Block* block = Block::GetBlock(this->blocks[z][x][y]);
                if( block->type == BlockType::None )
                    continue;

                if( z - 1 >= 0 && this->blocks[z - 1][x][y] == BlockType::None)
                    block->back_block = 1;

                if( z + 1 < MAX_DEPTH && this->blocks[z + 1][x][y] == BlockType::None)
                    block->front_block = 1;

                if( y - 1 >= 0 && this->blocks[z][x][y - 1] == BlockType::None)
                    block->bottom_block = 1;

                if( y + 1 < MAX_HEIGHT && this->blocks[z][x][y + 1] == BlockType::None)
                    block->top_block = 1;
                
                if( x - 1 >= 0 && this->blocks[z][x - 1][y] == BlockType::None)
                    block->left_block = 1;

                if( x + 1 < MAX_WIDTH && this->blocks[z][x + 1][y] == BlockType::None)
                    block->right_block = 1;
            }
        }
    }
}

void Chunk::GenerateChunks()
{
    //std::cout << "[DEBUG] Position: " << this->position << std::endl;
    Chunk* left = this->manager->FindChunk((this->position / 16) + Vec3(-1, 0, 0));
    Chunk* right = this->manager->FindChunk((this->position / 16) + Vec3(1, 0, 0));
    Chunk* top = this->manager->FindChunk((this->position / 16) + Vec3(0, 1, 0));
    Chunk* bottom = this->manager->FindChunk((this->position / 16) + Vec3(0, -1, 0));
    Chunk* front = this->manager->FindChunk((this->position / 16) + Vec3(0, 0, 1));
    Chunk* back = this->manager->FindChunk((this->position / 16) + Vec3(0, 0,-1));

    

    this->GenerateChunk();
    if( left != nullptr )
    {
        //this->left->right = this;
        left->GenerateChunk();
    }
    
    if( right != nullptr )
    {
        //this->right->left = this;
        right->GenerateChunk();
    }
    
    if( front != nullptr )
    {
        //this->front->back = this;
        front->GenerateChunk();
    }

    if( back != nullptr )
    {
        //this->back->front = this;
        back->GenerateChunk();
    }

    if( top != nullptr )
    {
        //this->front->back = this;
        top->GenerateChunk();
    }

    if( bottom != nullptr )
    {
        //this->back->front = this;
        bottom->GenerateChunk();
    }
}

void Chunk::GenerateChunk()
{
    this->mesh.vertices.clear();
    this->mesh.indices.clear();
    this->mesh.normals.clear();
    this->mesh.uvs.clear();

    //std::cout << "{DEBUG] GC started..." << std::endl;

    this->mesh.ClearData();
    //std::cout << "{DEBUG] GC mesh clear data..." << std::endl;

    Chunk* left = this->manager->FindChunk((this->position / 16) + Vec3(-1, 0, 0));
    Chunk* right = this->manager->FindChunk((this->position / 16) + Vec3(1, 0, 0));
    Chunk* top = this->manager->FindChunk((this->position / 16) + Vec3(0, 1, 0));
    Chunk* bottom = this->manager->FindChunk((this->position / 16) + Vec3(0, -1, 0));
    Chunk* front = this->manager->FindChunk((this->position / 16) + Vec3(0, 0, 1));
    Chunk* back = this->manager->FindChunk((this->position / 16) + Vec3(0, 0,-1));

    int planes = 0;
    for(int z = 0; z < MAX_DEPTH; z++)
    {
        for(int x = 0; x < MAX_WIDTH; x++)
        {
            for(int y = 0; y < MAX_HEIGHT; y++)
            {
                if( this->blocks[z][x][y] == None )
                    continue;

                Vec3 position(x, y, z);
                Block* block = Block::GetBlock(this->blocks[z][x][y]);

                // Back Planes
                if( z - 1 == -1 && back != nullptr && back->blocks[MAX_DEPTH - 1][x][y] == None) {
                    BackPlane(position);
                    UvsPlane(block->texBack.x, block->texBack.y, 160, 256, 16, 16);
                    planes++;
                }
                else if (z - 1 == -1 && back == nullptr )
                {
                    BackPlane(position);
                    UvsPlane(block->texBack.x, block->texBack.y, 160, 256, 16, 16);
                    planes++;
                }
                else if( z - 1 >= 0 && this->blocks[z - 1][x][y] == None)
                {
                    BackPlane(position);
                    UvsPlane(block->texBack.x, block->texBack.y, 160, 256, 16, 16);
                    planes++;
                }

                // Front Planes
                if (z == MAX_DEPTH - 1 && front != nullptr && front->blocks[0][x][y] == None)
                {
                    FrontPlane(position);
                    UvsPlane(block->texFront.x, block->texFront.y, 160, 256, 16, 16);
                    planes++;
                }
                else if ( z == MAX_DEPTH - 1 && front == nullptr )
                {
                    FrontPlane(position);
                    UvsPlane(block->texFront.x, block->texFront.y, 160, 256, 16, 16);
                    planes++;
                }
                else if( z + 1 < MAX_DEPTH && this->blocks[z+1][x][y] == None) 
                {
                    
                    FrontPlane(position);
                    UvsPlane(block->texFront.x, block->texFront.y, 160, 256, 16, 16);
                    planes++;
                }


                // Left Plane of Block
                if(x - 1 == -1 && left != nullptr && left->blocks[z][MAX_WIDTH - 1][y] == None)
                {
                    LeftPlane(position);
                    UvsPlane(block->texLeft.x, block->texLeft.y, 160, 256, 16, 16);
                    planes++;
                }
                else if ( x - 1 == -1 && left == nullptr )
                {
                    LeftPlane(position);
                    UvsPlane(block->texLeft.x, block->texLeft.y, 160, 256, 16, 16);
                    planes++;
                }
                else if( (x - 1 >= 0 && this->blocks[z][x-1][y] == None) )
                {
                    LeftPlane(position);
                    UvsPlane(block->texLeft.x, block->texLeft.y, 160, 256, 16, 16);
                    planes++;
                }

                // Right Plane of Block
                if(x + 1 == MAX_WIDTH && right != nullptr && right->blocks[z][0][y] == None) {
                    RightPlane(position);
                    UvsPlane(block->texRight.x, block->texRight.y, 160, 256, 16, 16);
                    planes++;
                }
                else if ( x + 1 == MAX_WIDTH && right == nullptr )
                {
                    RightPlane(position);
                    UvsPlane(block->texRight.x, block->texRight.y, 160, 256, 16, 16);
                    planes++;
                }
                else if( x + 1 < MAX_WIDTH && this->blocks[z][x+1][y] == None )
                {
                    RightPlane(position);
                    UvsPlane(block->texRight.x, block->texRight.y, 160, 256, 16, 16);
                    planes++;
                }

                if(y - 1 == -1 && bottom != nullptr && bottom->blocks[z][x][MAX_HEIGHT - 1] == None)
                {
                    BottomPlane(position);
                    UvsPlane(block->texBottom.x, block->texBottom.y, 160, 256, 16, 16);
                    planes++;
                }
                else if ( y - 1 == -1 && bottom == nullptr )
                {
                    BottomPlane(position);
                    UvsPlane(block->texBottom.x, block->texBottom.y, 160, 256, 16, 16);
                    planes++;
                }
                else if( (y - 1 >= 0 && this->blocks[z][x][y-1] == None) )
                {
                    BottomPlane(position);
                    UvsPlane(block->texBottom.x, block->texBottom.y, 160, 256, 16, 16);
                    planes++;
                }

                if(y + 1 == MAX_HEIGHT && top != nullptr && top->blocks[z][x][0] == None)
                {
                    TopPlane(position);
                    UvsPlane(block->texTop.x, block->texTop.y, 160, 256, 16, 16);
                    planes++;
                }
                else if ( y + 1 == MAX_HEIGHT && top == nullptr )
                {
                    TopPlane(position);
                    UvsPlane(block->texTop.x, block->texTop.y, 160, 256, 16, 16);
                    planes++;
                }
                else if( (y + 1 < MAX_HEIGHT && this->blocks[z][x][y+1] == None) )
                {
                    TopPlane(position);
                    UvsPlane(block->texTop.x, block->texTop.y, 160, 256, 16, 16);
                    planes++;
                }
            }
        }
    }

    for(int i = 0; i < planes; i++)
    {
        this->mesh.indices.push_back(i * 4 + 0);
        this->mesh.indices.push_back(i * 4 + 1);
        this->mesh.indices.push_back(i * 4 + 2);
        this->mesh.indices.push_back(i * 4 + 0);
        this->mesh.indices.push_back(i * 4 + 2);
        this->mesh.indices.push_back(i * 4 + 3);
    }

    this->mesh.isChanged = true;
}

Chunk::~Chunk()
{
    
}

void Chunk::UvsPlane(int x, int y, int texWidth, int texHeight, int w, int h) {
    float _iw = (float)w / (float) texWidth;
    float _ih = (float) h / (float) texHeight;

    float _x = _iw * (float) x;
    float _y = _ih * (float) y;

    PUSH_UVS(_x + 0,_y + _ih);
    PUSH_UVS(_x + 0,_y + 0);
    PUSH_UVS(_x + _iw,_y + 0);
    PUSH_UVS(_x + _iw,_y + _ih);
}

std::string Chunk::Index() {
    std::string str_index;
    str_index += std::to_string(this->position.x / MAX_WIDTH) + "/";
    str_index += std::to_string(this->position.z / MAX_DEPTH);

    return str_index;
}

ChunkManager::ChunkManager(int max_width, int max_height,  int max_depth) {
    this->max_width = max_width;
    this->max_depth = max_depth;
    this->max_height = max_height;
}

ChunkManager::~ChunkManager() {
    
}

void ChunkManager::GenerateChunks(Material* material) {
    this->material = material;
    for(int i = -this->max_depth; i < this->max_depth; i++)
    {
        for(int j = -this->max_width; j < this->max_width; j++)
        {
            GenerateChunk(j, i);
            // for(int k = 0; k < this->max_height; k++)
            // {
            //     Vec3 position = Vec3(j * MAX_DEPTH, k * MAX_HEIGHT, i * MAX_WIDTH);
            //     Vec3 idx(j, k, i);

            //     Chunk chunk;
            //     chunk.manager = this;
            //     chunk.position = position;
            //     this->chunks[INDEX(idx)] = chunk;
            // }
        }
   }

//    for(int i = -this->max_depth; i < this->max_depth; i++)
//     {
//         for(int j = -this->max_width; j < this->max_width; j++)
//         {
//             for(int k = 0; k < this->max_height; k++)
//             {
//                 Vec3 index(j, k, i);
//                 std::string indstr = INDEX(index);

//                 Chunk* chunk = &this->chunks[indstr];
//                 chunk->Gen();
//             }
//         }
//    }

//    for(int i = -this->max_depth; i < this->max_depth; i++)
//     {
//         for(int j = -this->max_width; j < this->max_width; j++)
//         {
//             for(int k = 0; k < this->max_height; k++)
//             {
//                 Vec3 index(j, k, i);
//                 std::string indstr = INDEX(index);

//                 Chunk* chunk = &this->chunks[indstr];
//                 chunk->GenerateChunks();
//             }
//         }
//    }


//    for (auto& chunk : this->chunks)
//    {
        
//         Mesh* mesh = (Mesh*) &chunk.second.mesh;
//         chunk.second.object = GameObject();
//         chunk.second.object->mesh = mesh;
//         chunk.second.object->transform.position = chunk.second.position;
//         chunk.second.object->material = material;
//    }
   
}

void ChunkManager::UpdateGenerateChunk(int x, int z) {
    Vec3 index(x, 0, z);
    std::string indstr = INDEX(index);

    Chunk* chunk = &this->chunks[indstr];
    Chunk* left =   nullptr;
    Chunk* right =  nullptr;
    Chunk* back =   nullptr;
    Chunk* front =  nullptr;

    chunk->GenerateChunks();
}

Chunk* ChunkManager::FindChunk(Vec3 idx) {
    std::string strndx = INDEX(idx);
    return this->chunks.find(strndx) != this->chunks.end() ? &this->chunks[strndx] : nullptr;
}

void ChunkManager::GenerateChunk(int x, int z) {
    for(int y = 0; y < this->max_height; y++)
    {
        Vec3 ind(x, y, z);
        std::string strndx = INDEX(ind);
    
        this->chunks[strndx] = Chunk();
    
        //std::cout << "[DEBUG] Generate Chunk Test 1..." << std::endl;
        Chunk *chunk = &this->chunks[strndx];
        
        chunk->manager = this;
        chunk->position = Vec3(x * MAX_WIDTH, y * MAX_HEIGHT, z * MAX_DEPTH);
        chunk->Gen();
        //std::cout << "[DEBUG] Generate Chunk Test 3..." << std::endl;
    
        Mesh* mesh = &chunk->mesh;
        chunk->object = GameObject();
        chunk->object->mesh = mesh;
        chunk->object->transform.position = chunk->position;
        chunk->object->material = material;
    }

    Vec3 ind(x, 0, z);
    for(z = 0; z < MAX_DEPTH; z++)
    {
        for(x = 0; x < MAX_WIDTH; x++)
        {
            for(int y = 0; y < MID_HEIGHT - 20; y++)
            {
                ind.y = std::floor(y / MAX_HEIGHT);
                Chunk* chunk = FindChunk(ind);
                chunk->blocks[z][x][y % MAX_HEIGHT] = BlockType::Stone;
            }

            float perlin = SimplexNoise::noise((ind.x * MAX_WIDTH / MAX_WIDTH + (double) x / MAX_WIDTH) * SCALE_PERLIN, (ind.z * MAX_DEPTH / MAX_DEPTH + (double) z / MAX_DEPTH) * SCALE_PERLIN);
            //float tree_perlin = db::perlin<float>((position.x / MAX_WIDTH + (double) x / MAX_WIDTH) * 0.1f, (position.z / MAX_DEPTH + (double) z / MAX_DEPTH) * 0.1f);
            perlin = (perlin < 0) ? -perlin : perlin;

            // int higher_ground = 0;
            for(int y = MID_HEIGHT - 20; y < (MID_HEIGHT - 20) + perlin * 20; y++)
            {
                ind.y = std::floor(y / MAX_HEIGHT);
                Chunk* chunk = FindChunk(ind);
                if( y < ((MID_HEIGHT - 20) + perlin * 20) - 1)
                    chunk->blocks[z][x][y % MAX_HEIGHT] = BlockType::Dirt;
                else
                {
                    chunk->blocks[z][x][y % MAX_HEIGHT] = BlockType::Grass;
                }
            }

            for(int y = 0; y < MID_HEIGHT; y++)
            {
                if( y > 4 )
                {
                    ind.y = std::floor(y / MAX_HEIGHT);
                    Chunk* chunk = FindChunk(ind);
                    float noise = SimplexNoise::noise((ind.x + (double) x / MAX_WIDTH) * 0.5f, (float) y / MID_HEIGHT * 2 * 0.5f, (ind.z + (double) z / MAX_DEPTH) * 0.5f);
                    if( noise >= 0.2f && noise <= 0.3f && y < MID_HEIGHT - 10)
                    {
                        chunk->blocks[z][x][y % MAX_HEIGHT] = BlockType::None;
                    }

                    noise = SimplexNoise::noise((ind.x + (double) x / MAX_WIDTH) * 2, (float) y / MID_HEIGHT * 2 * 2, (ind.z + (double) z / MAX_DEPTH) * 2);
                    if( noise > 0.7f && y < MID_HEIGHT - 10)
                    {
                        chunk->blocks[z][x][y % MAX_HEIGHT] = BlockType::None;
                    }

                    noise = SimplexNoise::noise((ind.x + (double) x / MAX_WIDTH) * 2, (float) y / MID_HEIGHT * 2, (ind.z + (double) z / MAX_DEPTH) * 2);
                    if( noise > 0.94f && y < 10)
                    {
                        chunk->blocks[z][x][y % MAX_HEIGHT] = BlockType::Diamond;
                    }
                }
            }
        }
    }

    for(int y = 0; y < this->max_height; y++)
    {
        ind.y = y;
        Chunk *chunk = FindChunk(ind);
        chunk->GenerateChunks();
    }
}

void ChunkManager::ChunkRender(OpenGLRenderer& renderer, Camera* camera, Transform* transform) {
    Vec3* position = &transform->position;

    int render_distance = 5;
    for(int z = -render_distance; z < render_distance; z++)
    {
        for(int x = -render_distance; x < render_distance; x++)
        {
            for(int y = -10; y < 10; y++)
            {
                Vec3 index(
                    (int)(position->x / MAX_WIDTH) + x,
                    (int)(position->y / MAX_HEIGHT) + y,
                    (int)(position->z / MAX_DEPTH) + z
                );
    
                Chunk* chunk = FindChunk(index);
                
                if( chunk != nullptr && chunk->object.has_value() )
                    renderer.RenderObject(&chunk->object.value(), camera);
            }
        }
    }
    
}

void ChunkManager::UpdatePlayerChunk(Camera* camera) {
    Vec3* position = &camera->transform.position;

    int render_distance = 5;
    for(int z = -render_distance; z < render_distance; z++)
    {
        for(int x = -render_distance; x < render_distance; x++)
        {
            Vec3 index(
                (int)(position->x / MAX_WIDTH) + x,
                0,
                (int)(position->z / MAX_DEPTH) + z
            );

            Chunk* chunk = FindChunk(index);
            
            if( chunk == nullptr )
            {
                this->GenerateChunk(index.x, index.z);
            }
        }
    }
}