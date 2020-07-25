//
//  Level2.cpp
//  SDLProject
//
//  Created by Shucong Zhou on 2020/7/16.
//  Copyright © 2020 ctg. All rights reserved.
//

#include "Level2.h"

#define LEVEL2_WIDTH 14
#define LEVEL2_HEIGHT 8
#define LEVEL2_ENEMY_COUNT 3


unsigned int level2_data[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
    1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0
};

void Level2::Initialize() {
    
    state.nexScene = -1;
    state.passLevel = false;
    
    GLuint mapTextureID = Util::LoadTexture("tileset.png");
    state.map = new Map(LEVEL2_WIDTH, LEVEL2_HEIGHT, level2_data, mapTextureID, 1.0f, 4, 1);
    
    // Move over all of the player and enemy code from initialization.
    
    GLuint fontTextureID = Util::LoadTexture("font2.png");
    
    // Initialize clouds
    state.clouds = new Entity();
    state.clouds->entityName = "cloud";
    state.clouds->position = glm::vec3(7, -3, 0);
    state.clouds->textureID = Util::LoadTexture("clouds.png");
     
    //Initialize hearts
    state.heart = new Entity();
    state.heart->entityName = "heart";
    state.heart->position = glm::vec3(3 , 2.95f, 0);
    state.heart->textureID = Util::LoadTexture("heart.png");
    state.heart->number = 3;
    
    //Intitialize door
    
    state.door = new Entity();
    state.door->entityName = "door";
    state.door->position = glm::vec3(8.32, -4.08, 0);
    state.door->textureID = Util::LoadTexture("door.png");
    state.door->entityType = DOOR;
    state.door->height = 0.8;
    state.door->width = 0.8;
    
     // Initialize player
     
     state.player = new Entity();
     state.player->entityName = "george";
     state.player->entityType = PLAYER;
    state.player->position = glm::vec3(1.5, 0, 0);
     state.player->movement = glm::vec3(0);
     state.player->acceleration = glm::vec3(0, -15.0, 0);
    state.player->speed = glm::vec3(1, 0, 0);
     state.player->jumpPower = 10.2f;
     state.player->textureID = Util::LoadTexture("george_0.png");
     state.player->animRight = new int[4] {3, 7, 11, 15};
     state.player->animLeft = new int[4] {1, 5, 9, 13};
     state.player->animUp = new int[4] {2, 6, 10, 14};
     state.player->animDown = new int[4] {0, 4, 8, 12};

     state.player->animIndices = state.player->animRight;
     state.player->animFrames = 4;
     state.player->animIndex = 0;
     state.player->animTime = 0;
     state.player->animCols = 4;
     state.player->animRows = 4;
     
     state.player->height = 0.75f;
     state.player->width = 0.4f;
     
     
     //the dwarf - walking - beat by jump on it / let flower eat it
     state.enemies = new Entity[LEVEL2_ENEMY_COUNT];
     
     

     state.enemies[0].textureID = Util::LoadTexture("ctg.png");
     state.enemies[0].entityName = "dwarf";
    state.enemies[0].position = glm::vec3(1, -6, 0);
     state.enemies[0].speed = glm::vec3(1, 0, 0);
     state.enemies[0].movement = glm::vec3(0);
     
     state.enemies[0].entityType = ENEMY;
     state.enemies[0].aiType = WAITANDGO;
     state.enemies[0].aiState = IDLE;
     
     state.enemies[0].width = 0.7f;
     state.enemies[0].height = 0.9f;
    
     
     //the flower - eating - beat by jump on it
     state.enemies[1].textureID = Util::LoadTexture("plantsSleep.png");
     state.enemies[1].entityName = "flower";
     state.enemies[1].textureID_Alter = Util::LoadTexture("plantsActivated.png");
    state.enemies[1].position = glm::vec3(3.5, -7, 0);
     state.enemies[1].entityType = ENEMY;
     state.enemies[1].aiType = WAITANDEAT;
     state.enemies[1].aiState = IDLE;
    state.enemies[1].isActive = false;
     
     //the bird - attacking - beat by jump on it
     
     state.enemies[2].textureID = Util::LoadTexture("bird.png");
     state.enemies[2].entityName = "bird";
     state.enemies[2].position = glm::vec3(4.0f, -4, 0);
     state.enemies[2].entityType = ENEMY;
     state.enemies[2].aiType = FLYANDATTACK;
     state.enemies[2].aiState = FLY;
     state.enemies[2].animUpANDDown = new int[4] {0, 1, 2, 3};
     state.enemies[2].animIndices = state.enemies[2].animUpANDDown;
     state.enemies[2].animFrames = 4;
     state.enemies[2].animIndex = 0;
     state.enemies[2].animTime = 0;
     state.enemies[2].animCols = 4;
     state.enemies[2].animRows = 1;
     state.enemies[2].speed = glm::vec3(0, 0.8, 0);
    state.enemies[2].isActive = false;
     
     
     //Load Map
     Util::LoadTexture("tileset.png");
     state.map = new Map(LEVEL2_WIDTH, LEVEL2_HEIGHT, level2_data, mapTextureID, 1.0f, 4, 1);
    
}

#define FIXED_TIMESTEP 0.0166666f
void Level2::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, state.map, state.enemies, LEVEL2_ENEMY_COUNT,state.door);
    state.clouds->Update(deltaTime, state.player, state.map, state.enemies, LEVEL2_ENEMY_COUNT,state.door);
    state.heart->Update(deltaTime, state.player, state.map, state.enemies, LEVEL2_ENEMY_COUNT,state.door);
    state.door->Update(deltaTime, state.player, state.map, state.enemies, LEVEL2_ENEMY_COUNT,state.door);
    
    for (int i = 0; i < LEVEL2_ENEMY_COUNT; i++) {
        state.enemies[i].Update(FIXED_TIMESTEP, state.player, state.map, state.enemies, LEVEL2_ENEMY_COUNT,state.door);
    }
    
    if(state.player->touchSuccess) {
        state.passLevel = true;
        state.nexScene = 3;
        std::cout << "Pass Level 2" << std::endl;
        
    }
    if(state.player->loseLife || state.player->position.y < -10) {
        std::cout << "Lose One Life" << std::endl;
        loseLife = true;
        state.nexScene = 2;
    }
    
}
    
void Level2::Render(ShaderProgram *program, ShaderProgram *program_UI) {
    
    state.map->Render(program);
    state.clouds->RenderClouds(program);
    state.door->RenderDoor(program);
    state.heart->RenderHeart(program_UI);
    state.player->Render(program);

    
    for (int i = 0; i < LEVEL2_ENEMY_COUNT; i++) {
        if (state.enemies[i].isActive) {
            if (state.enemies[i].entityName == "dwarf" ) {
                state.enemies[i].RenderDwarf(program);
            }
            
            if (state.enemies[i].entityName == "flower") {
                if (!state.enemies[i].startAttack) {
                    state.enemies[i].RenderFlower_Sleep(program); }
                else {
                    state.enemies[i].RenderFlower_Activated(program);
                    
                }
            }
            
            if(state.enemies[i].entityName == "bird") {
                state.enemies[i].Render(program);
            }
        
        }
    }
     //std::cout << "Player Position: " << state.player->position.x << ' ' << state.player->position.y << std::endl;
    
   // std::cout << "Dwarf Position: " << state.enemies[0].position.x << ' ' << state.enemies[0].position.y << std::endl;
}
