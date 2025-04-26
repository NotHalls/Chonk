#include <chrono>
#include <iostream>

#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include "Curvy.h"
#include "Renderer/Shader.h"
#include "tempData.h"

int main()
{
  if(!SDL_Init(SDL_INIT_VIDEO))
  {
    std::cerr << "Failed To Init SDL_VIDEO: " << SDL_GetError() << std::endl;
    return -1;
  }

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  SDL_Window *MainWindow = SDL_CreateWindow(
      "Curvy", 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
  if(!MainWindow)
  {
    std::cerr << "Failed To Create Window: " << SDL_GetError() << std::endl;
    return -1;
  }

  SDL_GLContext Context = SDL_GL_CreateContext(MainWindow);
  if(!Context)
  {
    std::cerr << "Failed To Create SDL_GLContext: " << SDL_GetError()
              << std::endl;
    return -1;
  }

  if(!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    std::cerr << "Failed To Init Glad: " << SDL_GetError() << std::endl;

  bool IsRunning = true;
  SDL_Event Event;

  auto startTime = std::chrono::high_resolution_clock::now();

  /// Graphics Stuff ///
  // setup
  glViewport(0, 0, 800, 600);

  glCreateVertexArrays(1, &VAO);
  glCreateBuffers(1, &VBO);
  glCreateBuffers(1, &IBO);

  glBindVertexArray(VAO);

  // vertex buffer
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices,
               GL_STATIC_DRAW);
  // glBindBuffer(GL_ARRAY_BUFFER, 0);
  // index buffer
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices,
               GL_STATIC_DRAW);
  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  // vertex array setup
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
  glBindVertexArray(0);

  // shader setup
  Shader defaultShader(
      {{"assets/shaders/Default.vert.glsl", ShaderType::Vertex},
       {"assets/shaders/Default.frag.glsl", ShaderType::Fragment}});

  Texture groundTexture("assets/textures/grass_side.png");
  /// Graphics Stuff ///
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
        SDL_SetWindowRelativeMouseMode(MainWindow, FreeCursor);
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

    glm::mat4 MVP = cam.GetVPMatrix() * CubeTransform.Get();
    groundTexture.Bind(0);

    defaultShader.Bind();
    int uMVPLocation = glGetUniformLocation(defaultShader.Get(), "u_MVP");
    glUniformMatrix4fv(uMVPLocation, 1, GL_FALSE, glm::value_ptr(MVP));
    int uTexLoc = glGetUniformLocation(defaultShader.Get(), "u_Texture0");
    glUniform1i(uTexLoc, 0);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

    SDL_GL_SwapWindow(MainWindow);

    // std::cout << "FPS: " << (int)fps << " (" << frameTime << " ms)"
    //           << std::endl;
  }

  SDL_GL_DestroyContext(Context);
  SDL_DestroyWindow(MainWindow);
  SDL_Quit();

  return 0;
}
