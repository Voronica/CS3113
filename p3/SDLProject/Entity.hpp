//
//  Entity.hpp
//  SDLProject
//
//  Created by Shucong Zhou on 2020/6/18.
//  Copyright © 2020 ctg. All rights reserved.
//

#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

class Entity {
public:
    glm::vec3 position;
    glm::vec3 movement;
    glm::vec3 acceleration;
    glm::vec3 velocity;
    
    std::string entityName;
    float width = 1;
    float height = 1;
    
    bool jump = false;
    float jumpPower = 0;
    
    float speed;
    
    GLuint textureID;
    
    glm::mat4 modelMatrix;
    
    int *animRight = NULL;
    int *animLeft = NULL;
    int *animUp = NULL;
    int *animDown = NULL;

    int *animIndices = NULL;
    int animFrames = 0;
    int animIndex = 0;
    float animTime = 0;
    int animCols = 0;
    int animRows = 0;
    
    bool isActive = true;
    
    bool collidedTop = false;
    bool collidedBottom = false;
    bool collidedLeft = false;
    bool collidedRight = false;
    
    Entity();
    
    Entity* checkCollision(Entity *other);
    
    void checkCollisionsX_Rock(Entity *objects, int objectCount);
    void checkCollisionsY_Rock(Entity *objects, int objectCount);
    
    void checkCollisionX_Plane(Entity *object);
    void checkCollisionY_Plane(Entity *object);
    
    void UpdateRockCollision(float deltaTime, Entity *rocks, int platformCount);
    void UpdatePlaneCollision(float deltaTime, Entity *plane);
    
    void Update();
    void Render(ShaderProgram *program);
    void DrawSpriteFromTextureAtlas(ShaderProgram *program, GLuint textureID, int index);
};
