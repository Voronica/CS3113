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

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Entity.hpp"

#define PLATFORM_COUNT 27
#define ENEMY_COUNT 3




struct GameState {

    Entity *obstacles;
    Entity *player;
    Entity *enemies;
    

    
};

GameState state;

SDL_Window* displayWindow;
bool gameIsRunning = true;
bool gameOver = false;


//Initialize fonts
GLuint fontTextureID;




ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;



GLuint LoadTexture(const char* filePath) {
    int w, h, n;
    unsigned char* image = stbi_load(filePath, &w, &h, &n, STBI_rgb_alpha);
    
    if (image == NULL) {
        std::cout << "Unable to load image. Make sure the path is correct\n";
        assert(false);
    }
    
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    stbi_image_free(image);
    return textureID;
}

 

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Textured!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
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
    
    fontTextureID = LoadTexture("font2.png");
    
    
    
    // Initialize player
    
    state.player = new Entity();
    state.player->entityName = "george";
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(-1.5, 3.25, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, -15.0, 0);
    state.player->speed = glm::vec3(1, 0, 0);
    state.player->jumpPower = 9.2f;
    state.player->textureID = LoadTexture("george_0.png");
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
    
    //Initialize Obstacles
    state.obstacles = new Entity[PLATFORM_COUNT];
    
    
    GLuint platformTextureID = LoadTexture("tile.png");
    
    state.obstacles[0].entityName = "tile";
    state.obstacles[0].textureID = platformTextureID;
    state.obstacles[0].position = glm::vec3(-1, -3.25f, 0);
    
    state.obstacles[1].entityName = "tile";
    state.obstacles[1].textureID = platformTextureID;
    state.obstacles[1].position = glm::vec3(0, -3.25f, 0);
    
    state.obstacles[2].entityName = "tile";
    state.obstacles[2].textureID = platformTextureID;
    state.obstacles[2].position = glm::vec3(1, -3.25f, 0);
    
    state.obstacles[3].entityName = "tile";
    state.obstacles[3].textureID = platformTextureID;
    state.obstacles[3].position = glm::vec3(-4.5, -3.25f, 0);
    
    state.obstacles[4].entityName = "tile";
    state.obstacles[4].textureID = platformTextureID;
    state.obstacles[4].position = glm::vec3(-4.5, -2.25f, 0);
    
    state.obstacles[5].entityName = "tile";
    state.obstacles[5].textureID = platformTextureID;
    state.obstacles[5].position = glm::vec3(-4.5f, -1.25f, 0);
    
   
    state.obstacles[6].entityName = "tile";
    state.obstacles[6].textureID = platformTextureID;
    state.obstacles[6].position = glm::vec3(-4.5f, 1.25f, 0);
    
    state.obstacles[7].entityName = "tile";
    state.obstacles[7].textureID = platformTextureID;
    state.obstacles[7].position = glm::vec3(-4.5f, 2.25f, 0);
    
    state.obstacles[8].entityName = "tile";
    state.obstacles[8].textureID = platformTextureID;
    state.obstacles[8].position = glm::vec3(-4.5f, 3.25f, 0);
    
    state.obstacles[9].entityName = "tile";
    state.obstacles[9].textureID = platformTextureID;
    state.obstacles[9].position = glm::vec3(-3.5f, 1.25f, 0);
    
    state.obstacles[10].entityName = "tile";
    state.obstacles[10].textureID = platformTextureID;
    state.obstacles[10].position = glm::vec3(-2.5f, 1.25f, 0);
    
    state.obstacles[11].entityName = "tile";
    state.obstacles[11].textureID = platformTextureID;
    state.obstacles[11].position = glm::vec3(2.0f, -3.25f, 0);
    
    state.obstacles[12].entityName = "tile";
    state.obstacles[12].textureID = platformTextureID;
    state.obstacles[12].position = glm::vec3(-2.0f, -3.25f, 0);
    
    state.obstacles[13].entityName = "tile";
    state.obstacles[13].textureID = platformTextureID;
    state.obstacles[13].position = glm::vec3(-2.3f, -3.25f, 0);
    
    state.obstacles[14].entityName = "tile";
    state.obstacles[14].textureID = platformTextureID;
    state.obstacles[14].position = glm::vec3(-2.3f, -3.25f, 0);
    
    state.obstacles[15].entityName = "tile";
    state.obstacles[15].textureID = platformTextureID;
    state.obstacles[15].position = glm::vec3(-2.3f, -3.25f, 0);
    
    state.obstacles[16].entityName = "tile";
    state.obstacles[16].textureID = platformTextureID;
    state.obstacles[16].position = glm::vec3(4.5f, -2.25f, 0);
    
    state.obstacles[17].entityName = "tile";
    state.obstacles[17].textureID = platformTextureID;
    state.obstacles[17].position = glm::vec3(4.5f, -1.25f, 0);
    
    
    state.obstacles[18].entityName = "tile";
    state.obstacles[18].textureID = platformTextureID;
    state.obstacles[18].position = glm::vec3(4.5f, -0.25f, 0);
    
    state.obstacles[19].entityName = "tile";
    state.obstacles[19].textureID = platformTextureID;
    state.obstacles[19].position = glm::vec3(4.5f, 0.75f, 0);
    
    state.obstacles[20].entityName = "tile";
    state.obstacles[20].textureID = platformTextureID;
    state.obstacles[20].position = glm::vec3(4.5f, 1.75f, 0);
    
    state.obstacles[21].entityName = "tile";
    state.obstacles[21].textureID = platformTextureID;
    state.obstacles[21].position = glm::vec3(4.5f, 2.75f, 0);
    
    state.obstacles[22].entityName = "tile";
    state.obstacles[22].textureID = platformTextureID;
    state.obstacles[22].position = glm::vec3(4.5f, 3.75f, 0);
    
    state.obstacles[23].entityName = "tile";
    state.obstacles[23].textureID = platformTextureID;
    state.obstacles[23].position = glm::vec3(1.5f, -1.0f, 0);
    
    state.obstacles[24].entityName = "tile";
    state.obstacles[24].textureID = platformTextureID;
    state.obstacles[24].position = glm::vec3(2.5f, -1.0f, 0);
    
    
    state.obstacles[25].entityName = "tile";
    state.obstacles[25].textureID = platformTextureID;
    state.obstacles[25].position = glm::vec3(-0.5f, 0.0f, 0);
    
    state.obstacles[26].entityName = "accelerator";
    state.obstacles[26].textureID = LoadTexture("landingpad.png");
    state.obstacles[26].position = glm::vec3(1.41, -0.26, 0);
    state.obstacles[26].height = 0.3f;
    state.obstacles[26].entityType = PLATFORM;
    

   
    for (int i = 0; i < 26; i++) {
        state.obstacles[i].width = 1.35f;
        state.obstacles[i].height = 0.9f;
        state.obstacles[i].entityType = PLATFORM;
    }

    
    for (int i = 0; i < PLATFORM_COUNT; i++) {
        state.obstacles[i].Update(0, state.player, NULL, 0, NULL, 0);
    }
    
    
    //the dwarf - walking - attack by jump on it
    state.enemies = new Entity[ENEMY_COUNT];
    
    

    state.enemies[0].textureID = LoadTexture("ctg.png");
    state.enemies[0].entityName = "dwarf";
    state.enemies[0].position = glm::vec3(2.0f, -3.28f, 0);
    state.enemies[0].speed = glm::vec3(1, 0, 0);
    state.enemies[0].acceleration = glm::vec3(0, -10.0f, 0);
    state.enemies[0].movement = glm::vec3(0);
    
    state.enemies[0].entityType = ENEMY;
    state.enemies[0].aiType = WAITANDGO;
    state.enemies[0].aiState = IDLE;
    
    state.enemies[0].width = 0.7f;
    state.enemies[0].height = 0.9f;
    
    //the flower - eating - attacking by jump on it
    state.enemies[1].textureID = LoadTexture("plantsSleep.png");
    state.enemies[1].entityName = "flower";
    state.enemies[1].textureID_Alter = LoadTexture("plantsActivated.png");
    state.enemies[1].position = glm::vec3(-3.45f, -3.25f, 0);
    state.enemies[1].entityType = ENEMY;
    state.enemies[1].aiType = WAITANDEAT;
    state.enemies[1].aiState = IDLE;
    
    //the bird
    
    state.enemies[2].textureID = LoadTexture("bird.png");
    state.enemies[2].entityName = "bird";
    state.enemies[2].position = glm::vec3(0, 1.5, 0);
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
    state.enemies[2].speed = glm::vec3(0, 1, 0);

}


void ProcessInput()  {
  
        state.player->movement = glm::vec3(0);
    
    
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
                            if (state.player->collidedBottom) {
                                state.player->jump = true;
                            }
                            break;
                    }
                    break; // SDL_KEYDOWN
            }
        }
        
        const Uint8 *keys = SDL_GetKeyboardState(NULL);

   
        
            if (keys[SDL_SCANCODE_LEFT]) {
        
                state.player->movement.x = -1.5f;
                state.player->animIndices = state.player->animLeft;
                
            }
            else if (keys[SDL_SCANCODE_RIGHT]) {
                
                state.player ->movement.x = 1.5f;
                state.player->animIndices = state.player->animRight;
                
            }
            
        
    
    
    if (glm::length(state.player->movement) > 1.0f) {
        state.player->movement = glm::normalize(state.player->movement);
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
        state.player->Update(FIXED_TIMESTEP, state.player, state.obstacles, PLATFORM_COUNT, state.enemies, ENEMY_COUNT);
        
        for (int i = 0; i < ENEMY_COUNT; i++) {
            state.enemies[i].Update(FIXED_TIMESTEP, state.player, state.obstacles, PLATFORM_COUNT, state.enemies, ENEMY_COUNT);
            
        }
        
        deltaTime -= FIXED_TIMESTEP;
    }
    accumulator = deltaTime;
    
   
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
    
    state.player->Render(&program);
    
    
    for (int i = 0; i < PLATFORM_COUNT; i++) {
        if (state.obstacles[i].entityName == "tile") {
            state.obstacles[i].Render(&program);
        }
        
        if (state.obstacles[i].entityName == "accelerator") {
            state.obstacles[i].RenderAccelerator(&program);
        }
        
    }
    
    for (int i = 0; i < ENEMY_COUNT; i++) {
        if (state.enemies[i].isActive) {
            if (state.enemies[i].entityName == "dwarf" ) {
                state.enemies[i].RenderDwarf(&program);
            }
            
            if (state.enemies[i].entityName == "flower") {
                if (!state.enemies[i].startAttack) {
                    state.enemies[i].RenderFlower_Sleep(&program); }
                else state.enemies[i].RenderFlower_Activated(&program);
            }
            
            if(state.enemies[i].entityName == "bird") {
                state.enemies[i].Render(&program);
            }
        
        }
    }
    
    if(state.player->collideSomething) {
        if(state.player->collideEnemy) {
            DrawText(&program, fontTextureID, "Game Over" , 0.5f, -0.25f, glm::vec3(-2.0, 0, 0));
        }
    }
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
        Render();
        //std::cout << "PLAYER:  "<< "x: " << state.player->position.x << " y: " << state.player->position.y << std::endl;
        //std::cout << "BIRD:   "<< "x: " << state.enemies[2].position.x << " y: " << state.enemies[2].position.y << std::endl;
        
    }
    
    Shutdown();
    return 0;
}
