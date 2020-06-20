//
//  Entity.cpp
//  SDLProject
//
//  Created by Shucong Zhou on 2020/6/18.
//  Copyright © 2020 ctg. All rights reserved.
//

#include "Entity.hpp"

Entity::Entity()
{
    //position = glm::vec3(0);
    movement = glm::vec3(0);
    acceleration = glm::vec3(0);
    velocity = glm::vec3(0);
    speed = 0;
    
    modelMatrix = glm::mat4(1.0f);
}

Entity* Entity::checkCollision(Entity *other) {
    if (isActive == false || other->isActive == false) return nullptr;
    
    float xdist = fabs(position.x - other->position.x) - ((width + other->width) /2.0f);
    float ydist = fabs(position.y - other->position.y) - ((height + other->height) /2.0f);
    
    if (xdist < 0 && ydist < 0) return other;
    return nullptr;
}

void Entity::checkCollisionsX_Rock(Entity *objects, int objectCount) {
    for (int i = 0; i < objectCount; i++) {
        Entity *object = &objects[i];
        if (checkCollision(object) != nullptr) {
            float xdist = fabs(position.x - object->position.x);
            float penetrationX = fabs(xdist - (width / 2.0f) - (object->width / 2.0f));
            if (velocity.x > 0) {
                position.x -= penetrationX;
                velocity.x = 0;
                collidedRight = true;
                   }
            else if (velocity.x < 0) {
                position.x += penetrationX;
                velocity.x = 0;
                collidedLeft = true;
                   }
            }
        }
    }

void Entity::checkCollisionsY_Rock(Entity *objects, int objectCount) {
    
    for (int i = 0; i < objectCount; i++) {
        Entity *object = &objects[i];
        if (checkCollision(object) != nullptr) {
            float ydist = fabs(position.y - object->position.y);
            float penetrationY = fabs(ydist - (height / 2.0f) - (object->height / 2.0f));
            if (velocity.y > 0) {
                position.y -= penetrationY;
                velocity.y = 0;
                collidedTop = true;
                   }
            else if (velocity.y < 0) {
                position.y += penetrationY;
                velocity.y = 0;
                collidedBottom = true;
                }
            }
        }
    }
void Entity::checkCollisionX_Plane(Entity *object) {
    if(checkCollision(object) != nullptr) {
        float xdist = fabs(position.x - object->position.x);
        float penetrationX = fabs(xdist - (width / 2.0f) - (object->width / 2.0f));
        if (velocity.x > 0) {
            position.x -= penetrationX;
            velocity.x = 0;
            collidedRight = true;
               }
        else if (velocity.x < 0) {
            position.x += penetrationX;
            velocity.x = 0;
            collidedLeft = true;
               }
    }
}

void Entity::checkCollisionY_Plane(Entity *object) {
    if(checkCollision(object) != nullptr) {
        float ydist = fabs(position.y - object->position.y);
        float penetrationY = fabs(ydist - (height / 2.0f) - (object->height / 2.0f));
        if (velocity.y > 0) {
            position.y -= penetrationY;
            velocity.y = 0;
            collidedTop = true;
               }
        else if (velocity.y < 0) {
            position.y += penetrationY;
            velocity.y = 0;
            collidedBottom = true;
            }
        }
    }




void Entity::UpdateRockCollision(float deltaTime, Entity *rocks, int platformCount) {
    
    
    checkCollisionsY_Rock(rocks, platformCount);    // Fix if needed
    
    checkCollisionsX_Rock(rocks, platformCount);// Fix if needed
    

}

void Entity::UpdatePlaneCollision(float deltaTime, Entity *plane) {
    
    
    
    checkCollisionY_Plane(plane);    // Fix if needed
    
    checkCollisionX_Plane(plane);// Fix if needed
    
    
    
}

void Entity::Update(float deltaTime, Entity *object, int platformCount) {
    if (isActive == false) return;
    
    collidedTop = false;
    collidedBottom = false;
    collidedLeft = false;
    collidedRight = false;
    
    if(checkCollision(object) != nullptr) {
        if(checkCollision(object)->entityName == "tile") {
            UpdateRockCollision(deltaTime, object, platformCount);
        }
        if(checkCollision(object)->entityName == "plane") {
            UpdatePlaneCollision(deltaTime, object);
        }
    }
    velocity.x = movement.x * speed;
    velocity += acceleration * deltaTime;
    position += velocity * deltaTime;
    
    position.y += velocity.y * deltaTime;     // Move on Y
    position.x += velocity.x * deltaTime;       // Move on X
    
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
}


void Entity::DrawSpriteFromTextureAtlas(ShaderProgram *program, GLuint textureID, int index)
{
    float u = (float)(index % animCols) / (float)animCols;
    float v = (float)(index / animCols) / (float)animRows;
    
    float width = 1.0f / (float)animCols;
    float height = 1.0f / (float)animRows;
    
    float texCoords[] = { u, v + height, u + width, v + height, u + width, v,
        u, v + height, u + width, v, u, v};
    
    float vertices[]  = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);
    
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

void Entity::Render(ShaderProgram *program) {
    if (isActive == false) return;
    program->SetModelMatrix(modelMatrix);
    
    
    float vertices[]  = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
    
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);
    
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}
