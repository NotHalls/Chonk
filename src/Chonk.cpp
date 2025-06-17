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
  Chunk chunk2;
  chunk2.SetPosition(chunk2.GetPosition() + glm::vec3(20.0f, 0.0f, 0.0f));
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
      Scene::GetCamera()->OnEvent(Event);
      App.OnEvent(Event);
      if(Event.type == SDL_EVENT_QUIT)
        IsRunning = false;
    }
    // App.OnUpdate();
    Scene::GetCamera()->OnUpdate(dt);

    Scene::GetShader()->Bind();
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

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    { // draw cycle
      glm::mat4 model = glm::translate(glm::mat4(1.0f), chunk.GetPosition());
      glm::mat4 mvp = Scene::GetCamera()->GetVPMatrix() * model;
      int uMVPLoc = glGetUniformLocation(Scene::GetShader()->Get(), "u_MVP");
      glUniformMatrix4fv(uMVPLoc, 1, GL_FALSE, glm::value_ptr(mvp));
      chunk.Bind();
      chunk.Draw();

      glm::mat4 model2 = glm::translate(glm::mat4(1.0f), chunk2.GetPosition());
      glm::mat4 mvp2 = Scene::GetCamera()->GetVPMatrix() * model2;
      int uMVPLoc2 = glGetUniformLocation(Scene::GetShader()->Get(), "u_MVP");
      glUniformMatrix4fv(uMVPLoc2, 1, GL_FALSE, glm::value_ptr(mvp2));
      chunk2.Bind();
      chunk2.Draw();
    }

    App.GetWindow()->Update();

    std::cout << "FPS: " << (int)fps << " (" << frameTime << " ms)"
              << std::endl;
  }

  SDL_Quit();

  return 0;
}
