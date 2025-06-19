#include "ImGui.h"
#include "System/App.h"
#include "System/Scene.h"

#include <SDL3/SDL.h>
#include <imgui.h>
// imgui.h must be included before these two down here
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_sdl3.h>

void GUI::OnStart()
{
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

  ImGui_ImplSDL3_InitForOpenGL(
      App::GetWindow()->Get(),
      static_cast<SDL_GLContext>(App::GetContext()->GetRaw()));
  ImGui_ImplOpenGL3_Init("#version 460");
}

void GUI::OnUpdate(float dt) {}

void GUI::OnEvent(const SDL_Event &event)
{
  ImGui_ImplSDL3_ProcessEvent(&event);
}

// GUI Functions
void GUI::Begin()
{
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplSDL3_NewFrame();
  ImGui::NewFrame();
  ImGui::ShowDemoWindow();
}

void GUI::End()
{
  SDL_GL_MakeCurrent(App::GetWindow()->Get(),
                     static_cast<SDL_GLContext>(App::GetContext()->GetRaw()));

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUI::Destroy()
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL3_Shutdown();
  ImGui::DestroyContext();
}