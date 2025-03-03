#include <Player.h>

#include <engine/Shader.h>
#include <engine/Geometry.h>
#include <engine/Material.h>
#include <engine/Collider.h>
#include <engine/Raycast.h>
#include <Block.h>
#include <cmath>

Player::Player() {
    this->mesh = Geometry(GeometryType::DefCube);
    this->material = Material();

    this->material.color = Color(1.0f, 0.0f, 0.0f, 1.0f);
    this->material.id_program = ShaderProgram::program_lists["BasicShader"];
    

    this->object = GameObject(&this->mesh);
    this->object.transform.scale = Vec3(width, 2, width);
    this->object.material = &this->material;
}

bool IsCollide3D(Vec3 min, Vec3 max, Vec3 min1, Vec3 max1)
{
    return (min.x < max1.x && max.x > min1.x) && (min.y < max1.y && max.y > min1.y ) && (min.z < max1.z && max.z > min1.z);
}

bool IsCollide2D(Vec3 min, Vec3 max, Vec3 min1, Vec3 max1)
{
    return (min.x < max1.x && max.x > min1.x) && (min.y < max1.y && max.y > min1.y );
}

void Player::DestroyBlock(ChunkManager* mngr, Camera& camera, Vec3 forward) {
    Transform* transform = &camera.transform;
    Vec3 index((int)(transform->position.x / MAX_WIDTH), 0, (int)(transform->position.z / MAX_DEPTH));

    index.x = ( transform->position.x < 0 ) ? (int)(transform->position.x / MAX_WIDTH - 1) : (int)(transform->position.x / MAX_WIDTH);
    index.z = ( transform->position.z < 0 ) ? (int)(transform->position.z / MAX_DEPTH - 1) : (int)(transform->position.z / MAX_DEPTH);

    Chunk* current = mngr->FindChunk( index );
    if( current != nullptr )
    {
        Raycast ray(transform->position, forward * -1);
        Chunk* left = current->left;
        Chunk* right = current->right;
        Chunk* front = current->front;
        Chunk* back = current->back;

        BlockCollider* item = nullptr;
        float distance = 99999;

        for(auto& box_collide : current->colliders )
        {
            BoxCollider box(box_collide.min, box_collide.max);
            RayIntersect<BoxCollider> c = ray.IntersectBox(box, 5);
            if( c.object != nullptr )
            {
                if( c.t < distance )
                {
                    distance = c.t;
                    item = &box_collide;
                }
            }
        }

        // Left Chunk collider
        for(auto& box_collide : left->colliders )
        {
            BoxCollider box(box_collide.min, box_collide.max);
            RayIntersect<BoxCollider> c = ray.IntersectBox(box, 5);
            if( c.object != nullptr )
            {
                if( c.t < distance )
                {
                    distance = c.t;
                    item = &box_collide;
                    current = left;
                }
            }
        }

        // Right Chunk collider
        for(auto& box_collide : right->colliders )
        {
            BoxCollider box(box_collide.min, box_collide.max);
            RayIntersect<BoxCollider> c = ray.IntersectBox(box, 5);
            if( c.object != nullptr )
            {
                if( c.t < distance )
                {
                    distance = c.t;
                    item = &box_collide;
                    current = right;
                }
            }
        }

        // Front Chunk collider
        for(auto& box_collide : front->colliders )
        {
            BoxCollider box(box_collide.min, box_collide.max);
            RayIntersect<BoxCollider> c = ray.IntersectBox(box, 5);
            if( c.object != nullptr )
            {
                if( c.t < distance )
                {
                    distance = c.t;
                    item = &box_collide;
                    current = front;
                }
            }
        }

        // Back Chunk collider
        for(auto& box_collide : back->colliders )
        {
            BoxCollider box(box_collide.min, box_collide.max);
            RayIntersect<BoxCollider> c = ray.IntersectBox(box, 5);
            if( c.object != nullptr )
            {
                if( c.t < distance )
                {
                    distance = c.t;
                    item = &box_collide;
                    current = back;
                }
            }
        }

        
        if( item != nullptr )
        {
            current->blocks[(int)item->index.z][(int)item->index.x][(int)item->index.y] = BlockType::None;
            current->Regenerate();
        }
    }
}

void Player::PlaceBlock(ChunkManager* mngr, Camera& camera, Vec3 forward, BlockType type) {
    Transform* transform = &camera.transform;
    Vec3 index((int)(transform->position.x / MAX_WIDTH), 0, (int)(transform->position.z / MAX_DEPTH));

    index.x = ( transform->position.x < 0 ) ? (int)(transform->position.x / MAX_WIDTH - 1) : (int)(transform->position.x / MAX_WIDTH);
    index.z = ( transform->position.z < 0 ) ? (int)(transform->position.z / MAX_DEPTH - 1) : (int)(transform->position.z / MAX_DEPTH);

    Chunk* current = mngr->FindChunk( index );
    Chunk* center = current;
    if( current != nullptr )
    {
        Raycast ray(transform->position, forward * -1);
        Chunk* left = current->left;
        Chunk* right = current->right;
        Chunk* front = current->front;
        Chunk* back = current->back;

        BlockCollider tmp;
        BlockCollider* item = nullptr;
        RayIntersect<BoxCollider> ray_inter;
        float distance = 99999;

        for(auto& box_collide : current->GetAllBlockCollider() )
        {
            BoxCollider box(box_collide.min, box_collide.max);
            RayIntersect<BoxCollider> c = ray.IntersectBox(box, 5);
            if( c.object != nullptr )
            {
                if( c.t < distance )
                {
                    Vec3& index = box_collide.index;
                    if(
                        (index.x - 1 >= 0 && center->blocks[(int)index.z][(int)index.x - 1][(int)index.y] != BlockType::None ) ||
                        (index.x + 1 < MAX_WIDTH && center->blocks[(int)index.z][(int)index.x + 1][(int)index.y] != BlockType::None ) ||

                        (index.z - 1 >= 0 && center->blocks[(int)index.z - 1][(int)index.x][(int)index.y] != BlockType::None ) ||
                        (index.z + 1 < MAX_DEPTH && center->blocks[(int)index.z + 1][(int)index.x][(int)index.y] != BlockType::None ) ||

                        (index.y - 1 >= 0 && center->blocks[(int)index.z][(int)index.x][(int)index.y - 1] != BlockType::None ) ||
                        (index.y + 1 < MAX_HEIGHT && center->blocks[(int)index.z][(int)index.x][(int)index.y + 1] != BlockType::None )
                    )
                    {
                        distance = c.t;
                        tmp = box_collide;
                        item = &tmp;
                        ray_inter = c;
                    }

                }
            }
        }

        // Left Chunk collider
        if( left != nullptr )
        {
            for(auto& box_collide : left->colliders )
            {
                BoxCollider box(box_collide.min, box_collide.max);
                RayIntersect<BoxCollider> c = ray.IntersectBox(box, 5);
                if( c.object != nullptr )
                {
                    if( c.t < distance )
                    {
                        distance = c.t;
                        //item = &box_collide;
                        current = left;
                        ray_inter = c;
                    }
                }
            }
        }

        // Right Chunk collider
        if( right != nullptr )
        {
            for(auto& box_collide : right->colliders )
            {
                BoxCollider box(box_collide.min, box_collide.max);
                RayIntersect<BoxCollider> c = ray.IntersectBox(box, 5);
                if( c.object != nullptr )
                {
                    if( c.t < distance )
                    {
                        distance = c.t;
                        //item = &box_collide;
                        current = right;
                        ray_inter = c;
                    }
                }
            }
        }

        // Front Chunk collider
        if( front != nullptr )
        {
            for(auto& box_collide : front->colliders )
            {
                BoxCollider box(box_collide.min, box_collide.max);
                RayIntersect<BoxCollider> c = ray.IntersectBox(box, 5);
                if( c.object != nullptr )
                {
                    if( c.t < distance )
                    {
                        distance = c.t;
                        //item = &box_collide;
                        current = front;
                        ray_inter = c;
                    }
                }
            }
        }

        // Back Chunk collider
        if( back != nullptr )
        {
            for(auto& box_collide : back->colliders )
            {
                BoxCollider box(box_collide.min, box_collide.max);
                RayIntersect<BoxCollider> c = ray.IntersectBox(box, 5);
                if( c.object != nullptr )
                {
                    if( c.t < distance )
                    {
                        distance = c.t;
                        //item = &box_collide;
                        current = back;
                        ray_inter = c;
                    }
                }
            }
        }

        
        if( item != nullptr )
        {
            // Vec3 center = (item->min + item->max) / 2;
            // Vec3 dir = (center - ray_inter.point);
            // dir.Normalize();
            // printf("Dir: %f %f %f\n", std::round(dir.x), std::round(dir.y), std::round(dir.z));
            // printf("Point Index: %i %i %i\n", (int)((int)(ray_inter.point.x - current->position.x) % MAX_WIDTH), (int)((int)(ray_inter.point.y) % MAX_HEIGHT), (int)((int)(ray_inter.point.z - current->position.z) % MAX_DEPTH));
            // printf("World Index: %i %i\n", (int)((int)(ray_inter.point.x - center->position.x) / MAX_WIDTH) + ((current->position.x - center->position.x < 0 ? -1 : 0)), (int)((int)(ray_inter.point.z - center->position.z) / MAX_DEPTH) + ((current->position.z - center->position.z < 0 ? -1 : 0)));

            // // Back Chunk
            // if( (int)((int)(ray_inter.point.z - center->position.z) / MAX_DEPTH) + ((current->position.z - center->position.z < 0 ? -1 : 0)) == -1 )
            // {
            //     current->blocks[0][(int)((int)(ray_inter.point.x - current->position.x) % MAX_WIDTH)][(int)((int)(ray_inter.point.y) % MAX_HEIGHT)] = type;  
            // }
            // // Front Chunk
            // else if ( (int)((int)(ray_inter.point.z - center->position.z) / MAX_DEPTH) + ((current->position.z - center->position.z < 0 ? -1 : 0)) == 1 )
            // {
            //     current->blocks[MAX_DEPTH-1][(int)((int)(ray_inter.point.x - current->position.x) % MAX_WIDTH)][(int)((int)(ray_inter.point.y) % MAX_HEIGHT)] = type;  
            // }
            // // Left Chunk
            // if( (int)((int)(ray_inter.point.x - center->position.x) / MAX_WIDTH) + ((current->position.x - center->position.x < 0 ? -1 : 0)) == -1 )
            // {
            //     current->blocks[(int)((int)(ray_inter.point.z - current->position.z) % MAX_DEPTH)][0][(int)((int)(ray_inter.point.y) % MAX_HEIGHT)] = type;  
            // }
            // // Right Chunk
            // else if ( (int)((int)(ray_inter.point.x - center->position.x) / MAX_WIDTH) + ((current->position.x - center->position.x < 0 ? -1 : 0)) == 1 )
            // {
            //     current->blocks[(int)((int)(ray_inter.point.z - current->position.z) % MAX_DEPTH)][MAX_WIDTH - 1][(int)((int)(ray_inter.point.y) % MAX_HEIGHT)] = type;  
            // }
            // else {
                 current->blocks[(int)item->index.z][(int)item->index.x][(int)item->index.y] = type;  
            // }
            // //current->blocks[(int)item->index.z][(int)item->index.x][(int)item->index.y] = BlockType::None;
            current->Regenerate();
        }
    }
}

void Player::UpdatePhysics(ChunkManager* mngr) {
    Transform* transform = &this->object.transform;
    Vec3* pos = &transform->position;
    
    
    velocity.y -= this->gravity * 0.0001f;

    Vec3 index((int)(transform->position.x / MAX_WIDTH), 0, (int)(transform->position.z / MAX_DEPTH));

    index.x = ( transform->position.x < 0 ) ? (int)(transform->position.x / MAX_WIDTH - 1) : (int)(transform->position.x / MAX_WIDTH);
    index.z = ( transform->position.z < 0 ) ? (int)(transform->position.z / MAX_DEPTH - 1) : (int)(transform->position.z / MAX_DEPTH);

    Chunk* current = mngr->FindChunk(index);


    if( (int)pos->x % MAX_WIDTH == 0 )
    {
        index.x = (int) std::round(transform->position.x / MAX_WIDTH) - 1;
        index.z = (int) std::round(transform->position.z / MAX_DEPTH);
        Chunk* left = mngr->FindChunk(index);

        std::vector<BlockCollider>& colliders = left->colliders;
    
        for(const auto& collider : colliders) {
    
            if( 
                (pos->x < collider.max.x && pos->x + width > collider.min.x) &&
                (pos->z < collider.max.z && pos->z + width > collider.min.z)
            )
            {
                if( (pos->y < collider.max.y && pos->y + height > collider.min.y) )
                {
                    velocity.y = 0;
                    pos->y = collider.max.y;
                }
            }
        }
    }

    if( (int)pos->z % (MAX_DEPTH - 1) == 0 )
    {
        index.x = (int) std::round(transform->position.x / MAX_WIDTH);
        index.z = ((int) transform->position.z / MAX_DEPTH) + 1;
        Chunk* back = mngr->FindChunk(index);

        std::vector<BlockCollider>& colliders = back->colliders;
    
        for(const auto& collider : colliders) {
    
            if( 
                (pos->x < collider.max.x && pos->x + width > collider.min.x) &&
                (pos->z < collider.max.z && pos->z + width > collider.min.z)
            )
            {
                if( (pos->y < collider.max.y && pos->y + height > collider.min.y) )
                {
                    velocity.y = 0;
                    pos->y = collider.max.y;
                }
            }
        }
    }

    if( (int)pos->x % (MAX_WIDTH - 1) == 0 )
    {
        index.x = (int) std::round(transform->position.x / MAX_WIDTH) + 1;
        index.z = (int) std::round(transform->position.z / MAX_DEPTH);
        Chunk* left = mngr->FindChunk(index);

        std::vector<BlockCollider>& colliders = left->colliders;
    
        for(const auto& collider : colliders) {
    
            if( 
                (pos->x < collider.max.x && pos->x + width > collider.min.x) &&
                (pos->z < collider.max.z && pos->z + width > collider.min.z)
            )
            {
                if( (pos->y < collider.max.y && pos->y + height > collider.min.y) )
                {
                    velocity.y = 0;
                    pos->y = collider.max.y;
                }
            }
        }
    }

    if( (int)pos->z % MAX_DEPTH == 0 )
    {
        index.x = (int) std::round(transform->position.x / MAX_WIDTH);
        index.z = ((int) transform->position.z / MAX_DEPTH) - 1;
        Chunk* back = mngr->FindChunk(index);

        std::vector<BlockCollider>& colliders = back->colliders;
    
        for(const auto& collider : colliders) {
    
            if( 
                (pos->x < collider.max.x && pos->x + width > collider.min.x) &&
                (pos->z < collider.max.z && pos->z + width > collider.min.z)
            )
            {
                if( (pos->y < collider.max.y && pos->y + height > collider.min.y) )
                {
                    velocity.y = 0;
                    pos->y = collider.max.y;
                }
            }
        }
    }

    std::vector<BlockCollider>& colliders = current->colliders;   
    for(const auto& collider : colliders) {
        //bool front = IsCollide(*pos, *pos + Vec3(width, height, -width), collider.min, collider.max);

        if( 
            (pos->x < collider.max.x && pos->x + width > collider.min.x) &&
            (pos->z < collider.max.z && pos->z + width > collider.min.z)
        )
        {
            if( (pos->y < collider.max.y && pos->y + height > collider.min.y) )
            {
                velocity.y = 0;
                pos->y = collider.max.y;
            }
        }
    }

    // for(int z = -collider_distance; z < collider_distance; z++)
    // {
    //     for(int x = -collider_distance; x < collider_distance; x++)
    //     {
    //         Vec3 index((int)(transform->position.x / MAX_WIDTH) + x, 0, (int)(transform->position.z / MAX_DEPTH) + z);
    //         Chunk* current = mngr->FindChunk(index);

    //         std::vector<BlockCollider>& colliders = current->colliders;
        
        
    //         for(const auto& collider : colliders) {
    //             bool front = IsCollide(*pos, *pos + Vec3(width, height, -width), collider.min, collider.max);
        
    //             if( 
    //                 (pos->x < collider.max.x && pos->x + width > collider.min.x) &&
    //                 (pos->z < collider.max.z && pos->z + width > collider.min.z)
    //             )
    //             {
    //                 if( (pos->y < collider.max.y && pos->y + height > collider.min.y) )
    //                 {
    //                     velocity.y = 0;
    //                     pos->y = collider.max.y;
    //                 }
    //             }
    //         }
    //     }
    // }

    pos->Add( velocity );
    
}