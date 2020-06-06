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

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, projectionMatrix;
glm::mat4 dwarf_modelMatrix, light_modelMatrix, sword_modelMatrix;



GLuint dwarfTextureID, lightTextureID, swordTextureID;


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
    displayWindow = SDL_CreateWindow("2D Scene!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, 1280, 960);
    
    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
    
    viewMatrix = glm::mat4(1.0f);
    
    dwarf_modelMatrix = glm::mat4(1.0f);
    light_modelMatrix = glm::mat4(1.0f);
    sword_modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
    
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    //program.SetColor(1.0f, 0.0f, 0.0f, 1.0f);
    
    glUseProgram(program.programID);
    
    glClearColor(0.5f, 0.0f, 0.0f, 1.0f);
    
    //blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    dwarfTextureID = LoadTexture("ctg.png");
    lightTextureID = LoadTexture("light.png");
    swordTextureID = LoadTexture("sword.png");
}

void ProcessInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            gameIsRunning = false;
        }
    }
}

void Update() {
    
    light_modelMatrix = glm::rotate(light_modelMatrix, glm::radians(1.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    dwarf_modelMatrix = glm::translate(dwarf_modelMatrix, glm::vec3(0.01f, 0.0f, 0.0f));
    sword_modelMatrix = glm::scale(sword_modelMatrix, glm::vec3(1.01f, 1.01f, 1.0f));
    sword_modelMatrix = glm::scale(sword_modelMatrix, glm::vec3(-1.01f, -1.01f, 1.0f));
    
    sword_modelMatrix = glm::translate(sword_modelMatrix, glm::vec3(0.01f, 0.0f, 0.0f));
    
    
    
}

void drawDwarf() {
    program.SetModelMatrix(dwarf_modelMatrix);
    glBindTexture(GL_TEXTURE_2D, dwarfTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    
    
}

void drawLight() {
    program.SetModelMatrix(light_modelMatrix);
    glBindTexture(GL_TEXTURE_2D, lightTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void drawSword() {
    program.SetModelMatrix(sword_modelMatrix);
    glBindTexture(GL_TEXTURE_2D, swordTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
}
void Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    
    float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
    
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program.texCoordAttribute);
    
    drawDwarf();
    
    float vertices1[] = { 1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f };
    float texCoords1[] = { 1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f };
    
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices1);
    glEnableVertexAttribArray(program.positionAttribute);
    
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords1);
    glEnableVertexAttribArray(program.texCoordAttribute);
    
    
    drawLight();
    
    float vertices2[] = { -1.18, 0.18, -0.18, 0.18, -0.18, 1.18, -1.18, 0.18, -0.18, 1.18, -1.18, 1.18};
    float texCoords2[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
    
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices2);
    glEnableVertexAttribArray(program.positionAttribute);
    
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords2);
    glEnableVertexAttribArray(program.texCoordAttribute);
    
    drawSword();
    
    glDisableVertexAttribArray(program.positionAttribute);
    glDisableVertexAttribArray(program.texCoordAttribute);
    
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
