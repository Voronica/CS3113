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

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, player1_modelMatrix, player2_modelMatrix, projectionMatrix;

//player1
// Start at 0, 0, 0

// Don’t go anywhere (yet).
glm::vec3 player1_position = glm::vec3(0, 0, 0);
glm::vec3 player1_movement;

//player2
// Start at 0, 0, 0

// Don’t go anywhere (yet).
glm::vec3 player2_position = glm::vec3(0, 0, 0);
glm::vec3 player2_movement;


 
float player1_speed = 1.0f;
float player2_speed = 1.0f;



void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("PONG!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, 1280, 960);
    
    program.Load("shaders/vertex.glsl", "shaders/fragment.glsl");
    
    viewMatrix = glm::mat4(1.0f);
    
    //Initializa player1, player2 modelMatrixes
    player1_modelMatrix = glm::mat4(1.0f);
    player2_modelMatrix = glm::mat4(1.0f);
    
    
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
    
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    program.SetColor(1.0f, 0.0f, 0.0f, 1.0f);
    
    glUseProgram(program.programID);
    
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
}

 void ProcessInput() {
     player1_movement = glm::vec3(0);
     player2_movement = glm::vec3(0);
     
     SDL_Event event;
     while (SDL_PollEvent(&event)) {
         switch (event.type) {
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE:
                gameIsRunning = false;
                break;
        }
    }
     const Uint8 *keys = SDL_GetKeyboardState(NULL);
     
     if (keys[SDL_SCANCODE_W]) {
             player2_movement.y = 1.0f;
         
     }
     else if (keys[SDL_SCANCODE_S]) {
            player2_movement.y = -1.0f;
     }
     
     if (keys[SDL_SCANCODE_DOWN]) {
            player1_movement.y = -1.0f;
     }
     else if (keys[SDL_SCANCODE_UP]) {
            player1_movement.y = 1.0f;
     }
     
     if (glm::length(player1_movement) > 1.0f) {
         player1_movement = glm::normalize(player1_movement);
     }
     
     if (glm::length(player2_movement) > 1.0f) {
         player2_movement = glm::normalize(player2_movement);
     }
     
     
}

float lastTicks = 0.0f;

void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;
    
    
    // Add (direction * units per second * elapsed time) : player1
    player1_position += player1_movement * player1_speed * deltaTime;
    
    // Add (direction * units per second * elapsed time) : player2
    player2_position += player2_movement * player2_speed * deltaTime;
    
    if (player1_position.y < -3.75f ) {
        player1_position.y = -3.75f;
    }
    
    else if (player1_position.y > 3.75f ) {
        player1_position.y = 3.75f;
    }
    
    if (player2_position.y < -3.75f ) {
        player2_position.y = -3.75f;
    }
    
    else if (player2_position.y > 3.75f ) {
        player2_position.y = 3.75f;
    }
    
    
    
    
    
    player1_modelMatrix = glm::translate(player1_modelMatrix, player1_position);
    player2_modelMatrix = glm::translate(player2_modelMatrix, player2_position);
    

}

void drawPlayer1() {
    program.SetModelMatrix(player1_modelMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void drawPlayer2() {
    program.SetModelMatrix(player2_modelMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}


void Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    
    //draw player1
    float player1_vertices[] = { -0.5+5.2, -0.5-0.3, 0.5+5.2, -0.5-0.3, 0.5+5.2, 0.5+0.3, -0.5+5.2, -0.5-0.3, 0.5+5.2, 0.5+0.3, -0.5+5.2, 0.5+0.3 };
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, player1_vertices);
    glEnableVertexAttribArray(program.positionAttribute);

    
    drawPlayer1();
    
    //draw player2
    float player2_vertices[] = { -0.5-5.2, -0.5-0.3, 0.5-5.2, -0.5-0.3, 0.5-5.2, 0.5+0.3, -0.5-5.2, -0.5-0.3, 0.5-5.2, 0.5+0.3, -0.5-5.2, 0.5+0.3 };
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, player2_vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    
    
    drawPlayer2();
    
    glDisableVertexAttribArray(program.positionAttribute);
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
