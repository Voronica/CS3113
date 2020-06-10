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

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
ShaderProgram program_textured;

GLuint ballTextureID;

glm::mat4 viewMatrix, player1_modelMatrix, player2_modelMatrix, ball_modelMatrix, projectionMatrix;

//player1
// Start at 5.0 0, 0

// Don’t go anywhere (yet).
glm::vec3 player1_position = glm::vec3(5.0, 0, 0);
glm::vec3 player1_movement;

//player2
// Start at 5.0, 0, 0

// Don’t go anywhere (yet).
glm::vec3 player2_position = glm::vec3(-5.0, 0, 0);
glm::vec3 player2_movement;

//the ball
glm::vec3 ball_position = glm::vec3(0, 0, 0);


float player1_speed = 5.0f;
float player2_speed = 5.0f;
float ball_speed_x = 0.05f;
float ball_speed_y = 0.05f;

bool ball_keepMoving = false;


GLuint LoadTexture(const char* filePath) {
    int w, h, n;
    unsigned char* image = stbi_load(filePath, &w, &h, &n, STBI_rgb_alpha);
    if (image == NULL) {
        std::cout << "Unable to load image. Make sure the path is correct\n"; assert(false);
    }
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    stbi_image_free(image);
    return textureID;
}


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
    program_textured.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
    
    
    
    
    viewMatrix = glm::mat4(1.0f);
    
    //Initializa player1, player2 modelMatrixes
    player1_modelMatrix = glm::mat4(1.0f);
    player2_modelMatrix = glm::mat4(1.0f);
    ball_modelMatrix = glm::mat4(1.0f);
    
    
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
    
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    
    program_textured.SetProjectionMatrix(projectionMatrix);
    program_textured.SetViewMatrix(viewMatrix);

    
    glUseProgram(program.programID);
    glUseProgram(program_textured.programID);
    
    
    
    
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    
    //blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    ballTextureID = LoadTexture("ball.png");
    
    
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
        
        //check ball-edge collision
        if (ball_position.y >= 3.6f || ball_position.y <= -3.6f) {
            ball_speed_y *= -1.0f;
        }
        
        if (fabs(ball_position.x - player1_position.x) - 0.4f < 0 &&
            fabs(ball_position.y - player1_position.y) - 0.9f < 0) {
            ball_speed_x *= -1.0f;
            //colliding
        }
        
        else if (fabs(ball_position.x - player2_position.x) - 0.4f < 0 &&
            fabs(ball_position.y - player2_position.y) - 0.9f < 0) {
            ball_speed_x *= -1.0f;
            //colliding
        }
        
        //ball goes out of window
        //game over
        if (fabs(ball_position.x) > 6.5f) {
            gameIsRunning = false;
        }

        ball_position.x += ball_speed_x;
        ball_position.y += ball_speed_y;
        
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
    glBindTexture(GL_TEXTURE_2D, ballTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    
    //draw player1
    float player1_vertices[] = { -0.3f, -0.7f, 0.3f, 0.7f, -0.3f, 0.7f, 0.3f, 0.7f, -0.3f, -0.7f, 0.3f, -0.7f};
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, player1_vertices);
    glEnableVertexAttribArray(program.positionAttribute);

    
    drawPlayer1();
    
    //draw player2
    float player2_vertices[] = { -0.3f, -0.7f, 0.3f, 0.7f, -0.3f, 0.7f, 0.3f, 0.7f, -0.3f, -0.7f, 0.3f, -0.7f};
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, player2_vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    
    drawPlayer2();
    
    float ball[] = { -0.1f, -0.1f, 0.1f, 0.1f, -0.1f, 0.1f, 0.1f, 0.1f, -0.1f, -0.1f, 0.1f, -0.1f };
    float ballCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, ball);
    glEnableVertexAttribArray(program.positionAttribute);
    
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, ballCoords);
    glEnableVertexAttribArray(program.texCoordAttribute);
    
    
    drawBall();
    
    glDisableVertexAttribArray(program.positionAttribute);
    glDisableVertexAttribArray(program.texCoordAttribute);
    
    SDL_GL_SwapWindow(displayWindow);
}

void Shutdown() {
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    
    Initialize();
    
    cout << "Press Space to Start" << endl;

    while (gameIsRunning) {
        ProcessInput();
        Update();
        Render();
    }
    
    Shutdown();
    return 0;
}
