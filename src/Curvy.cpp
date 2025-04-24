#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <iostream>

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

  while(IsRunning)
  {
    while(SDL_PollEvent(&Event))
    {
      if(Event.type == SDL_EVENT_QUIT)
        IsRunning = false;
    }

    glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
    glClear(GL_COLOR_BUFFER_BIT);

    SDL_GL_SwapWindow(MainWindow);
  }

  SDL_GL_DestroyContext(Context);
  SDL_DestroyWindow(MainWindow);
  SDL_Quit();

  return 0;
}
