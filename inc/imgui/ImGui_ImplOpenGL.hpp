#pragma once
// ImGui Renderer for: OpenGL3 (modern OpenGL with shaders / programmatic pipeline)
// This needs to be used along with a Platform Binding (e.g. GLFW, SDL, Win32, custom..)
// (Note: We are using GL3W as a helper library to access OpenGL functions since there is no
// standard header to access modern OpenGL functions easily. Alternatives are GLEW, Glad, etc..)

// Implemented features:
//  [X] Renderer: User texture binding. Use 'GLuint' OpenGL texture identifier as void*/ImTextureID.
//  Read the FAQ about ImTextureID in imgui.cpp.

// You can copy and use unmodified imgui_impl_* files in your project. See main.cpp for an example
// of using this. If you are new to dear imgui, read examples/README.txt and read the documentation
// at the top of imgui.cpp. https://github.com/ocornut/imgui

// About GLSL version:
// The 'glsl_version' initialization parameter defaults to "#version 150" if NULL.
// Only override if your GL version doesn't handle this GLSL version. Keep NULL if unsure!

#include "imgui.h"

namespace ImGui
{
namespace ImplOpenGL
{
IMGUI_IMPL_API bool Init(const char* glsl_version = nullptr);
IMGUI_IMPL_API void Shutdown();
IMGUI_IMPL_API void NewFrame();
IMGUI_IMPL_API void RenderDrawData(ImDrawData* draw_data);

// Called by Init/NewFrame/Shutdown
IMGUI_IMPL_API bool CreateFontsTexture();
IMGUI_IMPL_API void DestroyFontsTexture();
IMGUI_IMPL_API bool CreateDeviceObjects();
IMGUI_IMPL_API void DestroyDeviceObjects();
}
}
