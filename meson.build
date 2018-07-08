project('ImGUI_GLFW_Template', 'cpp',
  default_options : ['c_std=c17', 'cpp_std=c++17'])

inc = include_directories('inc', 'inc/imgui', 'inc/stb')
exe_src = ['src/Main.cpp',
  'src/imgui/imgui.cpp',
  'src/imgui/imgui_draw.cpp',
  'src/imgui/imgui_demo.cpp',
  'src/imgui/ImGui_ImplOpenGL.cpp',
  'src/imgui/ImGui_ImplGlfw.cpp']

dep_glfw3 = dependency('glfw3')
dep_glew = dependency('glew')

exe = executable('ImGUI_GLFW_Template', exe_src, include_directories : inc, dependencies : [dep_glew, dep_glfw3])
