#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <cstdlib>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, player1_modelMatrix, player2_modelMatrix, ball_modelMatrix, projectionMatrix;

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


//the ball
glm::vec3 ball_position = glm::vec3(0, 0, 0);


float player1_speed = 5.0f;
float player2_speed = 5.0f;
float ball_speed = 1.0f;

bool ball_keepMoving = false;

bool ball_changeX = false;
bool ball_changeY = false;

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
    ball_modelMatrix = glm::mat4(1.0f);
    
    
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
            case SDL_KEYDOWN:
                 switch (event.key.keysym.sym) {
                    case SDLK_SPACE:
                         //ball_movement.x = 1.0f;
                         //ball_movement.y = 0.4f;
                         break;
            }
            break; // SDL_KEYDOWN
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
     
     if (keys[SDL_SCANCODE_SPACE]) {
         ball_keepMoving = true;
     }
     
}

float lastTicks = 0.0f;


void Update() {
    
    player1_modelMatrix = glm::mat4(1.0f);
    player2_modelMatrix = glm::mat4(1.0f);
    ball_modelMatrix = glm::mat4(1.0f);
    
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;

    
    // Add (direction * units per second * elapsed time) : player1
    player1_position += player1_movement * player1_speed * deltaTime;
    
    // Add (direction * units per second * elapsed time) : player2
    player2_position += player2_movement * player2_speed * deltaTime;
    

    //check for paddles
    if (player1_position.y < -3.0f ) {
        player1_position.y = -3.0f;
    }
    
    else if (player1_position.y > 3.0f ) {
        player1_position.y = 3.0f;
    }
    
    if (player2_position.y < -3.0f ) {
        player2_position.y = -3.0f;
    }
    
    else if (player2_position.y > 3.0f ) {
        player2_position.y = 3.0f;
    }
    
    //check for ball
    if (ball_keepMoving) {
        if (!ball_changeY && !ball_changeX) {
            //by default
            ball_position.x += 2.0f * deltaTime;
            ball_position.y += 1.5f * deltaTime;
        }
        
        else if (ball_changeX) {
            ball_position.x -= 2.0f * deltaTime;
            ball_position.y += 1.5f * deltaTime;
        }
        
        else if (ball_changeY) {
            ball_position.x += 2.0f * deltaTime;
            ball_position.y -= 1.5f * deltaTime;
        }
        
        //check position ball-player1_paddle
        //(w1+w2)/2 = (1.0+0.2)/2 = 0.6
        //(h1+h2)/2 = (1.6+0.2)/2 = 0.9
        if (fabs(ball_position.x - player1_position.x) - 0.6f < 0 &&
            fabs(ball_position.y - player1_position.y) - 0.9f < 0) {
            ball_changeX = true;
            //colliding
        }
        
        //check position ball-player2_paddle
        if (fabs(ball_position.x - player2_position.x) - 0.6f < 0 &&
            fabs(ball_position.y - player2_position.y) - 0.9f < 0) {
            ball_changeX = true;
            //colliding
        }
         
        //check postion ball-edge
        if (ball_position.y > 2.0f || ball_position.y < -2.0f) {
            ball_changeY = true;
        }
        
    }
    
    player1_modelMatrix = glm::translate(player1_modelMatrix, player1_position);
    player2_modelMatrix = glm::translate(player2_modelMatrix, player2_position);
    ball_modelMatrix = glm::translate(ball_modelMatrix, ball_position);
    
    

}

void drawPlayer1() {
    program.SetModelMatrix(player1_modelMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void drawPlayer2() {
    program.SetModelMatrix(player2_modelMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void drawBall() {
    program.SetModelMatrix(ball_modelMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    
    //draw player1
    float player1_vertices[] = { 4.7f, -0.8f, 5.7f, -0.8f, 5.7f, 0.8f, 4.7f, -0.8f, 5.7f, 0.8f, 4.7f, 0.8f};
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, player1_vertices);
    glEnableVertexAttribArray(program.positionAttribute);

    
    drawPlayer1();
    
    //draw player2
    float player2_vertices[] = { -5.7f, -0.8f, -4.7f, -0.8f, -4.7f, 0.8f, -4.7f, -0.8f, -4.7f, 0.8f, -5.7f, 0.8f };
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, player2_vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    
    drawPlayer2();
    
    float ball[] = { -0.1f, -0.1f, 0.1f, 0.1f, -0.1f, 0.1f, 0.1f, 0.1f, -0.1f, -0.1f, 0.1f, -0.1f };
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, ball);
    glEnableVertexAttribArray(program.positionAttribute);
    
    drawBall();
    
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
