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

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Entity.hpp"

#define PLATFORM_COUNT 26

struct GameState {
    Entity *ship;
    Entity *obstacles;
    Entity *landingPlane;
};

GameState state;

SDL_Window* displayWindow;
bool gameIsRunning = true;

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
    
   
    // Initialize Game Objects
    
    // Initialize ship
    state.ship = new Entity();
    state.ship->entityName = "spaceShip";
    state.ship->position = glm::vec3(-1.75, 3.25, 0);
    state.ship->movement = glm::vec3(0);
    state.ship->acceleration = glm::vec3(0, -0.05f, 0);
    state.ship->speed = 1.5f;
    state.ship->textureID = LoadTexture("ship.png");
    
    
    
    state.ship->height = 0.8f;
    state.ship->width = 0.6f;
    
    //Initialize landingPlane
    
    state.landingPlane = new Entity();
    state.landingPlane->entityName = "plane";
    state.landingPlane->position = glm::vec3(5, -2.25, 0);
    state.landingPlane->movement = glm::vec3(0);
    state.landingPlane->textureID = LoadTexture("landingpad.png");
    
    state.landingPlane->height = 0.8f;
    state.landingPlane->width = 0.6f;
    
    
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
    state.obstacles[6].position = glm::vec3(-4.5f, -0.25f, 0);
    
    state.obstacles[7].entityName = "tile";
    state.obstacles[7].textureID = platformTextureID;
    state.obstacles[7].position = glm::vec3(-4.5f, 1.25f, 0);
    
    state.obstacles[8].entityName = "tile";
    state.obstacles[8].textureID = platformTextureID;
    state.obstacles[8].position = glm::vec3(-4.5f, 2.25f, 0);
    
    state.obstacles[9].entityName = "tile";
    state.obstacles[9].textureID = platformTextureID;
    state.obstacles[9].position = glm::vec3(-4.5f, 3.25f, 0);
    
    state.obstacles[10].entityName = "tile";
    state.obstacles[10].textureID = platformTextureID;
    state.obstacles[10].position = glm::vec3(-3.5f, 1.25f, 0);
    
    state.obstacles[11].entityName = "tile";
    state.obstacles[11].textureID = platformTextureID;
    state.obstacles[11].position = glm::vec3(-2.5f, 1.25f, 0);
    
    state.obstacles[12].entityName = "tile";
    state.obstacles[12].textureID = platformTextureID;
    state.obstacles[12].position = glm::vec3(2.0f, -3.25f, 0);
    
    state.obstacles[13].entityName = "tile";
    state.obstacles[13].textureID = platformTextureID;
    state.obstacles[13].position = glm::vec3(-2.0f, -3.25f, 0);
    
    state.obstacles[14].entityName = "tile";
    state.obstacles[14].textureID = platformTextureID;
    state.obstacles[14].position = glm::vec3(-3.0f, -3.25f, 0);
    
    state.obstacles[15].entityName = "tile";
    state.obstacles[15].textureID = platformTextureID;
    state.obstacles[15].position = glm::vec3(3.0f, -3.25f, 0);
    
    state.obstacles[16].entityName = "tile";
    state.obstacles[16].textureID = platformTextureID;
    state.obstacles[16].position = glm::vec3(4.5f, -3.25f, 0);
    
    state.obstacles[17].entityName = "tile";
    state.obstacles[17].textureID = platformTextureID;
    state.obstacles[17].position = glm::vec3(4.5f, -2.25f, 0);
    
    state.obstacles[18].entityName = "tile";
    state.obstacles[18].textureID = platformTextureID;
    state.obstacles[18].position = glm::vec3(4.5f, -1.25f, 0);
    
    
    state.obstacles[19].entityName = "tile";
    state.obstacles[19].textureID = platformTextureID;
    state.obstacles[19].position = glm::vec3(4.5f, -0.25f, 0);
    
    state.obstacles[20].entityName = "tile";
    state.obstacles[20].textureID = platformTextureID;
    state.obstacles[20].position = glm::vec3(4.5f, 0.75f, 0);
    
    state.obstacles[21].entityName = "tile";
    state.obstacles[21].textureID = platformTextureID;
    state.obstacles[21].position = glm::vec3(4.5f, 1.75f, 0);
    
    state.obstacles[22].entityName = "tile";
    state.obstacles[22].textureID = platformTextureID;
    state.obstacles[22].position = glm::vec3(4.5f, 2.75f, 0);
    
    state.obstacles[23].entityName = "tile";
    state.obstacles[23].textureID = platformTextureID;
    state.obstacles[23].position = glm::vec3(4.5f, 3.75f, 0);
    
    state.obstacles[24].entityName = "tile";
    state.obstacles[24].textureID = platformTextureID;
    state.obstacles[24].position = glm::vec3(1.5f, 0.75f, 0);
    
    state.obstacles[25].entityName = "tile";
    state.obstacles[25].textureID = platformTextureID;
    state.obstacles[25].position = glm::vec3(2.5f, 0.75f, 0);
    
    

    
    for (int i = 0; i < PLATFORM_COUNT; i++) {
        state.obstacles[i].Update(0, NULL, 0);
    }
 
}

void ProcessInput() {
    
    state.ship->movement = glm::vec3(0);
    
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
                        // Move the ship left
                        break;
                        
                    case SDLK_RIGHT:
                        // Move the ship right
                        break;
                        
                    case SDLK_SPACE:
                        break;
                }
                break; // SDL_KEYDOWN
        }
    }
    
    const Uint8 *keys = SDL_GetKeyboardState(NULL);

    if (keys[SDL_SCANCODE_LEFT]) {
        state.ship->movement.x = -0.5f;
    
    }
    else if (keys[SDL_SCANCODE_RIGHT]) {
        state.ship->movement.x = 0.5f;
    }
    


    if (glm::length(state.ship->movement) > 1.0f) {
        state.ship->movement = glm::normalize(state.ship->movement);
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
        state.ship->Update(FIXED_TIMESTEP, state.obstacles, PLATFORM_COUNT);
        state.ship->Update(FIXED_TIMESTEP, state.landingPlane, PLATFORM_COUNT);
        
        deltaTime -= FIXED_TIMESTEP;
    }
    accumulator = deltaTime;
}


void Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    for (int i = 0; i < PLATFORM_COUNT; i++) {
        state.obstacles[i].Render(&program);
    }
    
    
    state.ship->Render(&program);
    state.landingPlane->Render(&program);
    
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
    }
    
    Shutdown();
    return 0;
}
