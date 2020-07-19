#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>

#include <SDL_opengl.h>
#include <vector>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"


#include "Util.h"
#include "Entity.hpp"
#include "Map.h"
#include "Scene.h"

#include "Menu.h"
#include "Level1.h"
#include "Level2.h"



SDL_Window* displayWindow;
bool gameIsRunning = true;
bool gameOver = false;

GLuint fontTextureID;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;
  
Scene *currentScene;
Scene *sceneList[3];

int lives;

;

void SwitchToScene(Scene *scene) {
    currentScene = scene;
    currentScene->Initialize();
}

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Platformer - VoronicaZ!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, 1280, 960);
    
    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
    
    
    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
    
    
    
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    
    
    glUseProgram(program.programID);
    
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    //Initialize the text
    fontTextureID = Util::LoadTexture("font2.png");
    
    sceneList[0] = new Menu();
    sceneList[1] = new Level1();
    sceneList[2] = new Level2();
    SwitchToScene(sceneList[2]);
    
    lives = 3;
    
}


void ProcessInput()  {
  
        currentScene->state.player->movement = glm::vec3(0);
    
    
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                case SDL_WINDOWEVENT_CLOSE:
                    gameIsRunning = false;
                    break;
                    
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_LEFT:
                            // Move the player left
                            break;
                            
                        case SDLK_RIGHT:
                            // Move the player right
                            break;
                            
                        case SDLK_SPACE:
                            if (currentScene->state.player->collidedBottom) {
                                currentScene->state.player->jump = true;
                            }
                            break;
                    }
                    break; // SDL_KEYDOWN
            }
        }
        
        const Uint8 *keys = SDL_GetKeyboardState(NULL);

   
        
            if (keys[SDL_SCANCODE_LEFT]) {
        
                currentScene->state.player->movement.x = -2.0f;
                currentScene->state.player->animIndices = currentScene->state.player->animLeft;
                
            }
            else if (keys[SDL_SCANCODE_RIGHT]) {
                
                currentScene->state.player ->movement.x = 2.0f;
                currentScene->state.player->animIndices = currentScene->state.player->animRight;
                
            }
            
    //main menu
    if (currentScene == sceneList[0]) {
        if (glm::length(currentScene->state.player->movement) > 1.0f) {
            currentScene->state.player->movement = glm::normalize(currentScene->state.player->movement);
            }
    }
    if (currentScene == sceneList[1] or currentScene == sceneList[2]) {
        if (currentScene->state.passLevel) {
            std::cout << "Pass the level!" << std::endl;
            currentScene->state.player->movement = glm::normalize(currentScene->state.player->movement);
        }
    }
}
    

 #define FIXED_TIMESTEP 0.0166666f
float lastTicks = 0;
float accumulator = 0.0f;

void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;
    
    
    deltaTime += accumulator;
    
    if (deltaTime < FIXED_TIMESTEP) {
        accumulator = deltaTime;
        return;
        
    }
    while (deltaTime >= FIXED_TIMESTEP) {
    // Update. Notice it's FIXED_TIMESTEP. Not deltaTime
        currentScene->Update(FIXED_TIMESTEP);
        /*
        for (int i = 0; i < ENEMY_COUNT; i++) {
            state.enemies[i].Update(FIXED_TIMESTEP, state.player, state.obstacles, PLATFORM_COUNT, state.enemies, ENEMY_COUNT);
            
        }
         */
        
        deltaTime -= FIXED_TIMESTEP;
    }
    accumulator = deltaTime;
    
    viewMatrix = glm::mat4(1.0f);
    if (currentScene->state.player->position.x > 5) {
        viewMatrix = glm::translate(viewMatrix, glm::vec3(-currentScene->state.player->position.x, 3.75, 0));
    }
    else {
        viewMatrix = glm::translate(viewMatrix, glm::vec3(-5, 3.75, 0));
    }
}

void DrawText(ShaderProgram *program, GLuint fontTextureID, std::string text,
                                 float size, float spacing, glm::vec3 position) {
    
    float width = 1.0f / 16.0f; float height = 1.0f / 16.0f;
    
    std::vector<float> vertices;
    std::vector<float> texCoords;
    
    for(int i = 0; i < text.size(); i++) {
            int index = (int)text[i];
            float offset = (size + spacing) * i;
        
            float u = (float)(index % 16) / 16.0f;
            float v = (float)(index / 16) / 16.0f;
        
            vertices.insert(vertices.end(), {
                offset + (-0.5f * size), 0.5f * size,
                offset + (-0.5f * size), -0.5f * size,
                offset + (0.5f * size), 0.5f * size,
                offset + (0.5f * size), -0.5f * size,
                offset + (0.5f * size), 0.5f * size,
                offset + (-0.5f * size), -0.5f * size,
            });
            texCoords.insert(texCoords.end(), {
                u, v,
                u, v + height,
                u + width, v,
                u + width, v + height,
                u + width, v,
                u, v + height,
            });
        } // end of for loop
    
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    program->SetModelMatrix(modelMatrix);
    
    glUseProgram(program->programID);
    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices.data()); glEnableVertexAttribArray(program->positionAttribute);
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords.data()); glEnableVertexAttribArray(program->texCoordAttribute);
    
    glBindTexture(GL_TEXTURE_2D, fontTextureID);
    glDrawArrays(GL_TRIANGLES, 0, (int)(text.size() * 6));
    
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}



void Render() {
    
    glClear(GL_COLOR_BUFFER_BIT);
    program.SetViewMatrix(viewMatrix);
    
    currentScene->Render(&program);
    
    if (currentScene == sceneList[0]) {
        DrawText(&program, fontTextureID, "Little George" , 0.5f, -0.25f, glm::vec3(3.5, -3, 0));
    }
    
    DrawText(&program, fontTextureID, " x " + std::to_string(lives) , 0.5f, -0.25f, glm::vec3(8.8f, -0.83f, 0));
    
    SDL_GL_SwapWindow(displayWindow);
    
}


void Shutdown() {
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Initialize();
    
    while (gameIsRunning) {
        ProcessInput();
        Update();
        
        if(currentScene->state.nexScene >= 0) SwitchToScene(sceneList[currentScene->state.nexScene]);
        
        Render();
        
    }
    
    
    Shutdown();
    return 0;
}
