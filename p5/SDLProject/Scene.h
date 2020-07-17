//
//  Scene.hpp
//  SDLProject
//
//  Created by Shucong Zhou on 2020/7/16.
//  Copyright © 2020 ctg. All rights reserved.
//
#pragma once
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
#include "Util.h"
#include "Entity.hpp"
#include "Map.h"

struct GameState {
    Map *map;
    Entity *player;
    Entity *enemies;
    Entity *clouds;
    Entity *icons;
    Entity *heart;
    int nexScene;
};

class Scene {
public:
    GameState state;
    virtual void Initialize() = 0;
    virtual void Update(float deltaTime) = 0;
    virtual void Render(ShaderProgram *program) = 0;
};
