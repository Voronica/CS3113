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
    speed = glm::vec3(0);
    
    modelMatrix = glm::mat4(1.0f);
}

Entity* Entity::checkCollision(Entity *other) {
    if (isActive == false || other->isActive == false) return nullptr;
    
    float xdist = fabs(position.x - other->position.x) - ((width + other->width) /2.0f);
    float ydist = fabs(position.y - other->position.y) - ((height + other->height) /2.0f);
    
    if (xdist < 0 && ydist < 0) {
        collideSomething = true;
        
        return other;}
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

Entity* Entity::checkCollisionsY_Rock(Entity *objects, int objectCount) {
    Entity *RockStandOn = nullptr;
    
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
                RockStandOn = object;

                }
            }
        }
    return RockStandOn;
    }

Entity* Entity::checkCollisionsY_Enemy(Entity *objects, int objectCount) {
    Entity *EnemyKilled = nullptr;
    
    for (int i = 0; i < objectCount; i++) {
    Entity *object = &objects[i];
     if (checkCollision(object) != nullptr) {
    
         
         float ydist = fabs(position.y - object->position.y);
         float penetrationY = fabs(ydist - (height / 2.0f) - (object->height / 2.0f));
         
         if (velocity.y > 0) {
             position.y -= penetrationY;
             velocity.y = 0;
             collidedTop = true;
             //collide with an enemy, lose
             collideEnemy = true;
                }
         else if (velocity.y < 0) {
             position.y += penetrationY;
             velocity.y = 0;
             collidedBottom = true;
             
             //if collide on top, kill enemy success
             killEnemySuccess = true;
             EnemyKilled = object;
             std::cout << "Beat " << EnemyKilled->entityName <<std::endl;
             EnemyKilled->isActive = false;
             
             }
         }
     }
    
    return EnemyKilled;
}
    
void Entity::checkCollisionsX_Enemy(Entity *objects, int objectCount) {
    for (int i = 0; i < objectCount; i++) {
    Entity *object = &objects[i];
    if (checkCollision(object) != nullptr) {
        
        float xdist = fabs(position.x - object->position.x);
        float penetrationX = fabs(xdist - (width / 2.0f) - (object->width / 2.0f));
        if (velocity.x > 0) {
            position.x -= penetrationX;
            velocity.x = 0;
            collidedRight = true;
            //collide with an enemy, lose
            collideEnemy = true;
               }
        else if (velocity.x < 0) {
            position.x += penetrationX;
            velocity.x = 0;
            collidedLeft = true;
            //collide with an enemy, lose
            collideEnemy = true;
               }
        }
    }
}

//the dwarf
void Entity::AIWAITANDGO(Entity *player) {
    switch (aiState) {
            
        case IDLE:
            if (glm::distance(position, player->position) < 3.0f) {
                aiState = WALKING;
            }
            break;
            
        case WALKING:
            
            if (player->position.x - position.x < -2.0f) {
                movement = glm::vec3(-1, 0, 0);
            }
            
            else if (player->position.x - position.x > 2.0f) {
                movement = glm::vec3(1, 0, 0);
            }
            break;
            
        case ATTACKING:
            break;
            
        case FLY:
            break;
    }
}

// the flower
void Entity::AIWAITANDEAT(Entity *player) {
    switch(aiState) {
        case IDLE:
            if(glm::distance(position, player->position) < 2.0f) {
                aiState = ATTACKING;
                
            }
        case WALKING:
            break;
        case ATTACKING:
            startAttack = true;  //flower change to attacking mode
            position = glm::vec3(-3.5f, -2.6f, 0);
            break;
            
        case FLY:
            break;
    }
}

//the bird
void Entity::AIFLYANDATTACK(Entity *player) {
    switch(aiState) {
        case IDLE:
            break;
        case WALKING:
            break;
        case ATTACKING:
            break;
        case FLY:
            if (position.y <= 2.2f) {
                movement.y += 0.24;
            }
            
            else movement.y -= 0.24;
            
            break;
    }
}
void Entity::AI(Entity *player) {
    switch(aiType) {
        case WAITANDEAT:
            AIWAITANDEAT(player);
            break;
        case WALKER:
            break;
        case WAITANDGO:
            AIWAITANDGO(player);
            break;
        case FLYANDATTACK:
            AIFLYANDATTACK(player);
            break;
    }
}


void Entity::Update(float deltaTime, Entity *player, Entity *obstacles, int platformCount, Entity *enemies, int enemyCount) {
    
        collidedTop = false;
        collidedBottom = false;
        collidedLeft = false;
        collidedRight = false;
    
        if (isActive == false) return;  //If is not active, don't draw

        if (entityType == ENEMY) {
            AI(player);
            //std::cout << "Checking collisions!" << std::endl;
            checkCollisionsY_Enemy(enemies, enemyCount);
            
        }
        if (entityType == PLAYER){
            
            //check which kind of platform player is on
            Entity *result = checkCollisionsY_Rock(obstacles, platformCount);
            
            if (result != nullptr) {
                if (result->entityName == "accelerator") {
                    jumpPower = 12;
                }
                else if (result->entityName == "tile") {
                    //std::cout << "touch the tile!" << std::endl;
                }
            }
            checkCollisionsX_Enemy(enemies, enemyCount);
            checkCollisionsY_Enemy(enemies, enemyCount);
        }
        
        
       
        
        
        if (animIndices != NULL) {
            if (glm::length(movement) != 0) {
                animTime += deltaTime;
                
                if(animTime >= 0.25f) {
                    animTime = 0.0f;
                    animIndex++;
                    if (animIndex >= animFrames) {
                        animIndex = 0;
                    }
                }
            }
            else animIndex = 0;
        }
        
        

        if (jump) {
            jump = false;
            
            velocity.y += jumpPower;
        }
        velocity.x = movement.x * speed.x;
    
        if(entityName == "bird") {
            velocity.y = movement.y * speed.y;
        }
    
        velocity += acceleration * deltaTime;
        
        
        position.y += velocity.y * deltaTime;// Move on Y
        if (entityType != PLAYER) checkCollisionsY_Rock(obstacles, platformCount);

        position.x += velocity.x * deltaTime;     // Move on X
        if (entityType != PLAYER) checkCollisionsX_Rock(obstacles, platformCount);
    
        
        
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

    program->SetModelMatrix(modelMatrix);
    
    if (animIndices != NULL) {
        DrawSpriteFromTextureAtlas(program, textureID, animIndices[animIndex]);
        return;
    }
        
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

void Entity::RenderFlower_Activated(ShaderProgram *program) {
    program->SetModelMatrix(modelMatrix);
    
        
    float vertices[]  = { -0.2, -0.5, 0.5, -0.5, 0.5, 0.5, -0.2, -0.5, 0.5, 0.5, -0.2, 0.5 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
    
    glBindTexture(GL_TEXTURE_2D, textureID_Alter);
    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);
    
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

void Entity::RenderFlower_Sleep(ShaderProgram *program) {
    program->SetModelMatrix(modelMatrix);
    
    
    float vertices[]  = { -0.2, -0.5, 0.5, -0.5, 0.5, 0.5, -0.2, -0.5, 0.5, 0.5, -0.2, 0.5 };
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


void Entity::RenderDwarf(ShaderProgram *program) {
    program->SetModelMatrix(modelMatrix);
    
    if (animIndices != NULL) {
           DrawSpriteFromTextureAtlas(program, textureID, animIndices[animIndex]);
           return;
       }
    
    float vertices[]  = { -0.8, -0.5, 0.5, -0.5, 0.5, 0.5, -0.8, -0.5, 0.5, 0.5, -0.8, 0.5 };
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

void Entity::RenderAccelerator(ShaderProgram *program) {
    program->SetModelMatrix(modelMatrix);
    
    
    float vertices[]  = { -0.45, -0.1, 0.7, -0.1, 0.7, 0.1, -0.45, -0.1, 0.7, 0.1, -0.45, 0.1 };
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
