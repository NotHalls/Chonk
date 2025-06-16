#include <chrono>
#include <iostream>

#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Chonk.h"
#include "Renderer/Chunk.h"
#include "Renderer/Mesh.h"
#include "Renderer/Shader.h"
#include "include/tempData.h"

#include "System/Context.h"
#include "System/Init.h"
#include "System/Window.h"

int main()
{
  SDL_Init();
  Window window(800, 600);
  Context context(window);
  GLAD_Init();

  bool IsRunning = true;
  SDL_Event Event;

  auto startTime = std::chrono::high_resolution_clock::now();

  // shader setup
  Shader defaultShader(
      {{"assets/shaders/Default.vert.glsl", ShaderType::Vertex},
       {"assets/shaders/Default.frag.glsl", ShaderType::Fragment}});

  Texture groundTexture("assets/textures/BoxSheet.png");
  /// Graphics Stuff ///

  Chunk chunk;
  // Mesh mesh(CubeVertices, CubeIndices);

  while(IsRunning)
  {
    auto curtTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> delta = curtTime - startTime;
    startTime = curtTime;
    float dt = delta.count();
    float fps = 1 / dt;
    float frameTime = dt * 1000.0f;

    while(SDL_PollEvent(&Event))
    {
      cam.OnEvent(Event);

      // mouse State
      if(Event.type == SDL_EVENT_KEY_DOWN &&
         Event.key.scancode == SDL_SCANCODE_F)
      {
        FreeCursor = !FreeCursor;
        SDL_SetWindowRelativeMouseMode(window.Get(), FreeCursor);
      }
      if(Event.type == SDL_EVENT_QUIT)
        IsRunning = false;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    cam.OnUpdate(dt);

    defaultShader.Bind();
    groundTexture.Bind(0);

    glm::mat4 model = glm::translate(glm::mat4(1.0f), chunk.GetPosition());
    glm::mat4 mvp = cam.GetVPMatrix() * model;

    int uTexLoc = glGetUniformLocation(defaultShader.Get(), "u_Texture0");
    glUniform1i(uTexLoc, 0);
    int uMVPLoc = glGetUniformLocation(defaultShader.Get(), "u_MVP");
    glUniformMatrix4fv(uMVPLoc, 1, GL_FALSE, glm::value_ptr(mvp));
    int uTexsLoc = glGetUniformLocation(defaultShader.Get(), "u_Textures");
    int texs[3] = {3, 3, 3};
    glUniform1iv(uTexsLoc, 3, texs);

    // mesh.Bind();
    // glDrawElements(GL_TRIANGLES, CubeIndices.size(), GL_UNSIGNED_INT,
    // nullptr);

    chunk.Bind();
    chunk.Draw();

    SDL_GL_SwapWindow(window.Get());

    std::cout << "FPS: " << (int)fps << " (" << frameTime << " ms)"
              << std::endl;
  }

  SDL_Quit();

  return 0;
}
