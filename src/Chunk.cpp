#include <Chunk.h>
#include <cmath>
#include <iostream>
#include <engine/GameObject.h>
#include <engine/Mesh.h>

#include <SimplexNoise.h>

#define DB_PERLIN_IMPL
#include "perlin.hpp"

#define MAX_PERLIN_WIDTH (int) 0xffff
#define MAX_PERLIN_DEPTH (int) 0xffff
#define SCALE_PERLIN 0.1f
#define SCALE_PERLIN_CAVE 0.6f

#define INDEX(vec) std::to_string(vec.x) + "/" + std::to_string(vec.z)
// Chunk code

void Chunk::LeftPlane(Vec3& position) {
    this->vertices.push_back(Vec3(position.x + 0, position.y +  0, position.z +  0));
    this->vertices.push_back(Vec3(position.x + 0, position.y +  1, position.z +  0));
    this->vertices.push_back(Vec3(position.x + 0, position.y +  1, position.z +  1));
    this->vertices.push_back(Vec3(position.x + 0, position.y +  0, position.z +  1));

    this->normals.push_back(Vec3(-1, 0, 0));
    this->normals.push_back(Vec3(-1, 0, 0));
    this->normals.push_back(Vec3(-1, 0, 0));
    this->normals.push_back(Vec3(-1, 0, 0));
}
void Chunk::RightPlane(Vec3& position) {
    this->vertices.push_back(Vec3(position.x +  1, position.y +  0, position.z +  1));
    this->vertices.push_back(Vec3(position.x +  1, position.y +  1, position.z +  1));
    this->vertices.push_back(Vec3(position.x +  1, position.y +  1, position.z +  0));
    this->vertices.push_back(Vec3(position.x +  1, position.y +  0, position.z +  0));

    this->normals.push_back(Vec3( 1, 0, 0));
    this->normals.push_back(Vec3( 1, 0, 0));
    this->normals.push_back(Vec3( 1, 0, 0));
    this->normals.push_back(Vec3( 1, 0, 0));
}
void Chunk::FrontPlane(Vec3& position) {
    this->vertices.push_back(Vec3(position.x +  0, position.y +  0, position.z +  1));
    this->vertices.push_back(Vec3(position.x +  0, position.y +  1, position.z +  1));
    this->vertices.push_back(Vec3(position.x +  1, position.y +  1, position.z +  1));
    this->vertices.push_back(Vec3(position.x +  1, position.y +  0, position.z +  1));

    this->normals.push_back(Vec3(0, 0, 1));
    this->normals.push_back(Vec3(0, 0, 1));
    this->normals.push_back(Vec3(0, 0, 1));
    this->normals.push_back(Vec3(0, 0, 1));
}
void Chunk::BackPlane(Vec3& position) {
    this->vertices.push_back(Vec3(position.x +  1, position.y +  0, position.z +  0));
    this->vertices.push_back(Vec3(position.x +  1, position.y +  1, position.z +  0));
    this->vertices.push_back(Vec3(position.x +  0, position.y +  1, position.z +  0));
    this->vertices.push_back(Vec3(position.x +  0, position.y +  0, position.z +  0));

    this->normals.push_back(Vec3(0, 0, -1));
    this->normals.push_back(Vec3(0, 0, -1));
    this->normals.push_back(Vec3(0, 0, -1));
    this->normals.push_back(Vec3(0, 0, -1));
}
void Chunk::TopPlane(Vec3& position) {
    this->vertices.push_back(Vec3(position.x +  0, position.y +  1, position.z +  1));
    this->vertices.push_back(Vec3(position.x +  0, position.y +  1, position.z +  0));
    this->vertices.push_back(Vec3(position.x +  1, position.y +  1, position.z +  0));
    this->vertices.push_back(Vec3(position.x +  1, position.y +  1, position.z +  1));

    this->normals.push_back(Vec3(0, 1, 0));
    this->normals.push_back(Vec3(0, 1, 0));
    this->normals.push_back(Vec3(0, 1, 0));
    this->normals.push_back(Vec3(0, 1, 0));
}
void Chunk::BottomPlane(Vec3& position) {
    this->vertices.push_back(Vec3(position.x +  0, position.y +  0, position.z +  0));
    this->vertices.push_back(Vec3(position.x +  0, position.y +  0, position.z +  1));
    this->vertices.push_back(Vec3(position.x +  1, position.y +  0, position.z +  1));
    this->vertices.push_back(Vec3(position.x +  1, position.y +  0, position.z +  0));

    this->normals.push_back(Vec3(0, -1, 0));
    this->normals.push_back(Vec3(0, -1, 0));
    this->normals.push_back(Vec3(0, -1, 0));
    this->normals.push_back(Vec3(0, -1, 0));
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

    for(int z = 0; z < MAX_DEPTH; z++)
    {
        for(int x = 0; x < MAX_WIDTH; x++)
        {
            for(int y = 0; y < MID_HEIGHT - 20; y++)
            {
                this->blocks[z][x][y] = BlockType::Stone;
            }

            float perlin = SimplexNoise::noise((position.x / MAX_WIDTH + (double) x / MAX_WIDTH) * SCALE_PERLIN, (position.z / MAX_DEPTH + (double) z / MAX_DEPTH) * SCALE_PERLIN);
            float tree_perlin = db::perlin<float>((position.x / MAX_WIDTH + (double) x / MAX_WIDTH) * 0.1f, (position.z / MAX_DEPTH + (double) z / MAX_DEPTH) * 0.1f);
            perlin = (perlin < 0) ? -perlin : perlin;

            int higher_ground = 0;
            for(int y = MID_HEIGHT - 20; y < (MID_HEIGHT - 20) + perlin * 20; y++)
            {
                if( y < ((MID_HEIGHT - 20) + perlin * 20) - 1)
                    this->blocks[z][x][y] = BlockType::Dirt;
                else
                {
                    this->blocks[z][x][y] = BlockType::Grass;
                }
            }

            for(int y = 0; y < MID_HEIGHT; y++)
            {
                if( y > 4 )
                {
                    float noise = SimplexNoise::noise((position.x / MAX_WIDTH + (double) x / MAX_WIDTH) * 0.5f, (float) y / MID_HEIGHT * 2 * 0.5f, (position.z / MAX_DEPTH + (double) z / MAX_DEPTH) * 0.5f);
                    if( noise >= 0.2f && noise <= 0.3f && y < MID_HEIGHT - 10)
                    {
                        this->blocks[z][x][y] = BlockType::None;
                    }

                    noise = SimplexNoise::noise((position.x / MAX_WIDTH + (double) x / MAX_WIDTH) * 2, (float) y / MID_HEIGHT * 2 * 2, (position.z / MAX_DEPTH + (double) z / MAX_DEPTH) * 2);
                    if( noise > 0.7f && y < MID_HEIGHT - 10)
                    {
                        this->blocks[z][x][y] = BlockType::None;
                    }

                    noise = SimplexNoise::noise((position.x / MAX_WIDTH + (double) x / MAX_WIDTH) * 2, (float) y / MID_HEIGHT * 2, (position.z / MAX_DEPTH + (double) z / MAX_DEPTH) * 2);
                    if( noise > 0.94f && y < 10)
                    {
                        this->blocks[z][x][y] = BlockType::Diamond;
                    }
                }
            }

            // for(int y = 0; y < MID_HEIGHT - 10; y++)
            // {
            //     this->blocks[z][x][y] = BlockType::Stone;
            // }

            
            // // Stone
            // for(int y = 0; y < MID_HEIGHT - 10; y++)
            // {
            //     if( y >= MID_HEIGHT - 20)
            //         this->blocks[z][x][y] = BlockType::Dirt;
            //     else
            //         this->blocks[z][x][y] = BlockType::Stone;

            //     if( y >= 4 )
            //     {
            //         float noise = SimplexNoise::noise((position.x / MAX_WIDTH + (double) x / MAX_WIDTH) * SCALE_PERLIN_CAVE, (float) y / MID_HEIGHT * SCALE_PERLIN_CAVE, (position.z / MAX_DEPTH + (double) z / MAX_DEPTH) * SCALE_PERLIN_CAVE);
            //         if( noise > 0.6f )
            //             this->blocks[z][x][y] = BlockType::None;
            //     }
            // }
            

            // int perlin = db::perlin<double>((position.x / MAX_WIDTH + (double) x / MAX_WIDTH) * SCALE_PERLIN, (position.z / MAX_DEPTH + (double) z / MAX_DEPTH) * SCALE_PERLIN) * 20;


        }
    }

    //CheckAllBlock();
    GetBlockCollider();

    
}

void Chunk::Regenerate() {
    GetBlockCollider();
    this->mesh.Clear();
    this->mesh.ClearData();

    this->GenerateChunk();
    
    if( this->left != nullptr )
    {
        this->left->right = this;
        this->left->GenerateChunk();
    }
    
    if( this->right != nullptr )
    {
        this->right->left = this;
        this->right->GenerateChunk();
    }
    
    if( this->front != nullptr )
    {
        this->front->back = this;
        this->front->GenerateChunk();
    }

    if( this->back != nullptr )
    {
        this->back->front = this;
        this->back->GenerateChunk();
    }
}

void Chunk::GetBlockCollider() {
    this->colliders.clear();
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

void Chunk::GenerateChunk(Chunk* left, Chunk* right, Chunk* front, Chunk* back)
{
    this->left = left;
    this->right = right;
    this->front = front;
    this->back = back;

    this->GenerateChunk();
    
    if( this->left != nullptr )
    {
        this->left->right = this;
        this->left->GenerateChunk();
    }
    
    if( this->right != nullptr )
    {
        this->right->left = this;
        this->right->GenerateChunk();
    }
    
    if( this->front != nullptr )
    {
        this->front->back = this;
        this->front->GenerateChunk();
    }

    if( this->back != nullptr )
    {
        this->back->front = this;
        this->back->GenerateChunk();
    }
}

void Chunk::GenerateChunk()
{
    this->vertices.clear();
    this->indices.clear();
    this->normals.clear();
    this->uvs.clear();

    this->mesh.Clear();
    this->mesh.ClearData();

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

                if( y - 1 < 0 || (y - 1 >= 0 && this->blocks[z][x][y-1] == None) )
                {
                    BottomPlane(position);
                    UvsPlane(block->texBottom.x, block->texBottom.y, 160, 256, 16, 16);
                    planes++;
                }

                if( y + 1 >= MAX_HEIGHT || (y + 1 < MAX_HEIGHT && this->blocks[z][x][y+1] == None) )
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
        this->indices.push_back(i * 4 + 0);
        this->indices.push_back(i * 4 + 1);
        this->indices.push_back(i * 4 + 2);

        this->indices.push_back(i * 4 + 0);
        this->indices.push_back(i * 4 + 2);
        this->indices.push_back(i * 4 + 3);
    }

    this->mesh.SetVertices(this->vertices);
    this->mesh.SetNormals(this->normals);
    this->mesh.SetUvs(this->uvs);
    this->mesh.SetIndices(this->indices);
}

Chunk::~Chunk()
{
    
}

void Chunk::UvsPlane(int x, int y, int texWidth, int texHeight, int w, int h) {
    float _iw = (float)w / (float) texWidth;
    float _ih = (float) h / (float) texHeight;

    float _x = _iw * (float) x;
    float _y = _ih * (float) y;

    this->uvs.push_back(Vec3(_x + 0,_y + _ih, 0));
    this->uvs.push_back(Vec3(_x + 0,_y + 0, 0));
    this->uvs.push_back(Vec3(_x + _iw,_y + 0, 0));
    this->uvs.push_back(Vec3(_x + _iw,_y + _ih, 0));
}

std::string Chunk::Index() {
    std::string str_index;
    str_index += std::to_string(this->position.x / MAX_WIDTH) + "/";
    str_index += std::to_string(this->position.z / MAX_DEPTH);

    return str_index;
}

ChunkManager::ChunkManager(int max_width, int max_depth) {
    this->max_width = max_width;
    this->max_depth = max_depth;
    
}

ChunkManager::~ChunkManager() {
    
}

void ChunkManager::GenerateChunks(Material* material) {
    this->material = material;
    for(int i = -this->max_depth; i < this->max_depth; i++)
    {
        for(int j = -this->max_width; j < this->max_width; j++)
        {
            Vec3 position = Vec3(j * MAX_DEPTH, 0, i * MAX_WIDTH);
            Vec3 idx(j, 0, i);

            Chunk chunk;
            chunk.position = position;
            this->chunks[INDEX(idx)] = chunk;
        }
   }

   for(int i = -this->max_depth; i < this->max_depth; i++)
    {
        for(int j = -this->max_width; j < this->max_width; j++)
        {
            Vec3 index(j, 0, i);
            std::string indstr = INDEX(index);

            Chunk* chunk = &this->chunks[indstr];
            chunk->Gen();
        }
   }

   for(int i = -this->max_depth; i < this->max_depth; i++)
    {
        for(int j = -this->max_width; j < this->max_width; j++)
        {
            Vec3 index(j, 0, i);
            std::string indstr = INDEX(index);

            Chunk* chunk = &this->chunks[indstr];
            Chunk* left =   FindChunk(index + Vec3(-1, 0, 0));
            Chunk* right =  FindChunk(index + Vec3( 1, 0, 0));
            Chunk* back =   FindChunk(index + Vec3( 0, 0,-1));
            Chunk* front =  FindChunk(index + Vec3( 0, 0, 1));

            chunk->GenerateChunk(left, right, front, back);
        }
   }


   for (auto& chunk : this->chunks)
   {
        Mesh* mesh = (Mesh*) &chunk.second.mesh;
        chunk.second.object.mesh = mesh;
        chunk.second.object.transform.position = chunk.second.position;
        chunk.second.object.material = material;
   }
   
}

void ChunkManager::UpdateGenerateChunk(int x, int z) {
    Vec3 index(x, 0, z);
    std::string indstr = INDEX(index);

    Chunk* chunk = &this->chunks[indstr];
    Chunk* left =   FindChunk(index + Vec3(-1, 0, 0));
    Chunk* right =  FindChunk(index + Vec3( 1, 0, 0));
    Chunk* back =   FindChunk(index + Vec3( 0, 0,-1));
    Chunk* front =  FindChunk(index + Vec3( 0, 0, 1));

    chunk->GenerateChunk(left, right, front, back);
}

Chunk* ChunkManager::FindChunk(Vec3 idx) {
    std::string strndx = INDEX(idx);
    return this->chunks.find(strndx) != this->chunks.end() ? &this->chunks[strndx] : nullptr;
}

void ChunkManager::GenerateChunk(int x, int z) {
    Vec3 ind(x, 0, z);
    std::string strndx = INDEX(ind);

    this->chunks[strndx] = Chunk();
    
    Chunk *chunk = &this->chunks[strndx];
    chunk->position = Vec3(x * MAX_WIDTH, 0, z * MAX_DEPTH);
    chunk->Gen();

    chunk->GenerateChunk(
        FindChunk(ind + Vec3(-1, 0, 0)),
        FindChunk(ind + Vec3( 1, 0, 0)),
        FindChunk(ind + Vec3( 0, 0, 1)),
        FindChunk(ind + Vec3( 0, 0,-1))
    );

    Mesh* mesh = &chunk->mesh;
    chunk->object.mesh = mesh;
    chunk->object.transform.position = chunk->position;
    chunk->object.material = material;
}

void ChunkManager::ChunkRender(OpenGLRenderer& renderer, Camera* camera, Transform* transform) {
    Vec3* position = &transform->position;

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
            
            if( chunk != nullptr )
                renderer.RenderObject(&chunk->object, camera);
            else
            {
                this->GenerateChunk(index.x, index.z);
            }
        }
    }
    
}