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

#include "Processes/MainProcess.h"
#include "System/Scene.h"

int main()
{
  MainProcess App;
  App.OnStart();

  bool IsRunning = true;
  SDL_Event Event;

  Scene::Init(45.0f, 0.01f, 100.0f, 800, 600);

  auto startTime = std::chrono::high_resolution_clock::now();

  Texture groundTexture("assets/textures/BoxSheet.png");
  /// Graphics Stuff ///

  Chunk chunk;
  Scene::PushChunk(chunk);

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
      Scene::GetCamera()->OnEvent(Event);
      App.OnEvent(Event);
      if(Event.type == SDL_EVENT_QUIT)
        IsRunning = false;
    }

    groundTexture.Bind(0);
    int uTexLoc = glGetUniformLocation(Scene::GetShader()->Get(), "u_Texture0");
    glUniform1i(uTexLoc, 0);
    int uTexsLoc =
        glGetUniformLocation(Scene::GetShader()->Get(), "u_Textures");
    int texs[3] = {3, 3, 3};
    glUniform1iv(uTexsLoc, 3, texs);

    // mesh.Bind();
    // glDrawElements(GL_TRIANGLES, CubeIndices.size(), GL_UNSIGNED_INT,
    // nullptr);

    // App.OnUpdate(dt);
    Scene::GetCamera()->OnUpdate(dt);
    Scene::GetShader()->Bind();

    Scene::StartScene();
    { // draw cycle
    }
    Scene::StopScene();

    App.GetWindow()->Update();

    std::cout << "FPS: " << (int)fps << " (" << frameTime << " ms)"
              << std::endl;
  }

  SDL_Quit();

  return 0;
}
