#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <vector>

#include "ImGui_ImplGlfw.hpp"
#include "ImGui_ImplOpenGL.hpp"
#include "imgui.h"

class AppGL
{
public:
    ////////////////////////////////////////////////// Constants

    const char*  TITLE  = "ImGui GLFW OpenGL template";
    const size_t WIDTH  = 1280;
    const size_t HEIGHT = 720;

    const char* IMGUI_FONT = "./assets/fonts/Roboto-Medium.ttf";


    ////////////////////////////////////////////////// Methods

    /**
     * inits the App (GLFW, ImGui, all kinds of callbacks)
     */
    AppGL()
    : quit(false)
    {
        initGLFW();
        initImGui();
        initGLFWCallbacks();
    }

    ~AppGL()
    {
        destroyImGui();
        destroyGLFW();
    }

    int mainLoop()
    {
        while (!quit && !glfwWindowShouldClose(window))
        {
            int display_w, display_h;
            glfwMakeContextCurrent(window);
            glfwGetFramebufferSize(window, &display_w, &display_h);
            glViewport(0, 0, display_w, display_h);
            glClearColor(0, 0, 0, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            for (const auto& hook : preEventHooks)
                hook();

            glfwPollEvents();

            // Start the ImGui frame
            ImGui::ImplOpenGL::NewFrame();
            ImGui::ImplGlfw::NewFrame();
            ImGui::NewFrame();

            for (const auto& hook : renderHooks)
                hook();

            ImGui::Render();
            ImGui::ImplOpenGL::RenderDrawData(ImGui::GetDrawData());

            glfwMakeContextCurrent(window);
            glfwSwapBuffers(window);

            for (const auto& hook : postRenderHooks)
                hook();
        }
        return 0;
    }

    /**
     * PreEventHooks are executed just after the frame is cleared,
     * and before any input is processed
     */
    size_t addPreEventHook(std::function<void()> hook)
    {
        size_t idx = preEventHooks.size();
        preEventHooks.push_back(hook);
        return idx;
    }

    bool removePreEventHook(size_t idx)
    {
        if (idx < preEventHooks.size())
        {
            preEventHooks[idx] = []() {};
            return true;
        }
        return false;
    }

    /**
     * RenderHooks are executed after events are processed and are intended to be used for the
     * actual rendering
     */
    size_t addRenderHook(std::function<void()> hook)
    {
        size_t idx = renderHooks.size();
        renderHooks.push_back(hook);
        return idx;
    }

    bool removeRenderHook(size_t idx)
    {
        if (idx < renderHooks.size())
        {
            renderHooks[idx] = []() {};
            return true;
        }
        return false;
    }

    /**
     * PostRenderHooks are executed after everything is rendered and the frame is swapped
     */
    size_t addPostRenderHook(std::function<void()> hook)
    {
        size_t idx = postRenderHooks.size();
        postRenderHooks.push_back(hook);
        return idx;
    }

    bool removePostRenderHook(size_t idx)
    {
        if (idx < postRenderHooks.size())
        {
            postRenderHooks[idx] = []() {};
            return true;
        }
        return false;
    }

    /**
     * The following methods are for adding and removing input callbacks
     */

    /**
     * adds a callback, which is executed if not any other previous callback uses mouse buttons
     *  (indicated by the return value of the callbacks)
     *
     *  @return the id/index of the callback, necessary for later optional removal
     */
    size_t addMouseButtonCallback(std::function<bool(GLFWwindow*, int, int, int)> cb)
    {
        size_t idx = mouseButtonCallbacks.size();
        mouseButtonCallbacks.push_back(cb);
        return idx;
    }

    /**
     * removes the callback indicated by the index
     * returns true if it is successfully removed, false otherwise
     */
    bool removeMouseButtonCallback(size_t idx)
    {
        if (idx < mouseButtonCallbacks.size())
        {
            mouseButtonCallbacks[idx] = [](GLFWwindow*, int, int, int) { return false; };
            return true;
        }
        return false;
    }

    /**
     * adds a callback, which is executed if not any other previous callback uses mouse buttons
     *  (indicated by the return value of the callbacks)
     *
     *  @return the id/index of the callback, necessary for later optional removal
     */
    size_t addScrollCallback(std::function<bool(GLFWwindow*, double, double)> cb)
    {
        size_t idx = scrollButtonCallbacks.size();
        scrollButtonCallbacks.push_back(cb);
        return idx;
    }

    /**
     * removes the callback indicated by the index
     * returns true if it is successfully removed, false otherwise
     */
    bool removeScrollCallback(size_t idx)
    {
        if (idx < scrollButtonCallbacks.size())
        {
            scrollButtonCallbacks[idx] = [](GLFWwindow*, double, double) { return false; };
            return true;
        }
        return false;
    }

    /**
     * adds a callback, which is executed if not any other previous callback uses mouse buttons
     *  (indicated by the return value of the callbacks)
     *
     *  @return the id/index of the callback, necessary for later optional removal
     */
    size_t addKeyCallback(std::function<bool(GLFWwindow*, int, int, int, int)> cb)
    {
        size_t idx = keyCallbacks.size();
        keyCallbacks.push_back(cb);
        return idx;
    }

    /**
     * removes the callback indicated by the index
     * returns true if it is successfully removed, false otherwise
     */
    bool removeKeyCallback(size_t idx)
    {
        if (idx < scrollButtonCallbacks.size())
        {
            scrollButtonCallbacks[idx] = [](GLFWwindow*, double, double) { return false; };
            return true;
        }
        return false;
    }

    /**
     * adds a callback, which is executed if not any other previous callback uses mouse buttons
     *  (indicated by the return value of the callbacks)
     *
     *  @return the id/index of the callback, necessary for later optional removal
     */
    size_t addCharCallback(std::function<bool(GLFWwindow*, unsigned int)> cb)
    {
        size_t idx = charCallbacks.size();
        charCallbacks.push_back(cb);
        return idx;
    }

    /**
     * removes the callback indicated by the index
     * returns true if it is successfully removed, false otherwise
     */
    bool removeCharCallback(size_t idx)
    {
        if (idx < charCallbacks.size())
        {
            charCallbacks[idx] = [](GLFWwindow*, unsigned int) { return false; };
            return true;
        }
        return false;
    }


private:
    ////////////////////////////////////////////////// Methods

    static void glfwErrorCallBack(int error, const char* description)
    {
        std::cerr << "Glfw Error " << error << ": " << description << std::endl;
    }

    static void glfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
    {
        ImGui::ImplGlfw::MouseButtonCallback(window, button, action, mods);
        auto _this = static_cast<AppGL*>(glfwGetWindowUserPointer(window));
        if (!ImGui::GetIO().WantCaptureMouse)
        {
            for (const auto& cb : _this->mouseButtonCallbacks)
            {
                if (cb(window, button, action, mods))
                    break;
            }
        }
    }

    static void glfwScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
    {
        ImGui::ImplGlfw::ScrollCallback(window, xoffset, yoffset);
        auto _this = static_cast<AppGL*>(glfwGetWindowUserPointer(window));
        if (!ImGui::GetIO().WantCaptureMouse)
        {
            for (const auto& cb : _this->scrollButtonCallbacks)
            {
                if (cb(window, xoffset, yoffset))
                    break;
            }
        }
    }

    static void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        ImGui::ImplGlfw::KeyCallback(window, key, scancode, action, mods);
        auto _this = static_cast<AppGL*>(glfwGetWindowUserPointer(window));
        if (!ImGui::GetIO().WantCaptureKeyboard)
        {
            for (const auto& cb : _this->keyCallbacks)
            {
                if (cb(window, key, scancode, action, mods))
                    break;
            }
        }
    }

    static void glfwCharCallback(GLFWwindow* window, unsigned int c)
    {
        ImGui::ImplGlfw::CharCallback(window, c);
        auto _this = static_cast<AppGL*>(glfwGetWindowUserPointer(window));
        if (!ImGui::GetIO().WantCaptureKeyboard)
        {
            for (const auto& cb : _this->charCallbacks)
            {
                if (cb(window, c))
                    break;
            }
        }
    }

    void initGLFW()
    {
        // Setup window
        glfwSetErrorCallback(glfwErrorCallBack);
        if (!glfwInit())
            exit(EXIT_FAILURE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
        window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, NULL, NULL);
        glfwSetWindowUserPointer(window, this);

        glfwMakeContextCurrent(window);
        glfwSwapInterval(1);  // Enable vsync
        glewInit();
    }

    void initGLFWCallbacks()
    {
        glfwSetMouseButtonCallback(window, glfwMouseButtonCallback);
        glfwSetScrollCallback(window, glfwScrollCallback);
        glfwSetKeyCallback(window, glfwKeyCallback);
        glfwSetCharCallback(window, glfwCharCallback);
    }

    void initImGui()
    {
        // Setup Dear ImGui binding
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
        // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

        // init the frontend for ImGui (GLFW with OpenGL)
        ImGui::ImplGlfw::InitForOpenGL(window, false);
        ImGui::ImplOpenGL::Init();

        // Setup style
        ImGui::StyleColorsLight();

        io.Fonts->AddFontFromFileTTF(IMGUI_FONT, 16.0f);
    }

    void destroyGLFW()
    {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void destroyImGui()
    {
        ImGui::ImplOpenGL::Shutdown();
        ImGui::ImplGlfw::Shutdown();
        ImGui::DestroyContext();
    }


    ////////////////////////////////////////////////// Attributes

    GLFWwindow* window;

    bool quit;

    /**
     * callbacks for any input data
     *
     * the return value (bool) describes, if the callback is using the input,
     * and therefore blocks any further callbacks if it returns true
     *
     * if ImGui uses either the mouse or the keyboard, no other callbacks are executed
     */
    std::vector<std::function<bool(GLFWwindow*, int, int, int)>>      mouseButtonCallbacks;
    std::vector<std::function<bool(GLFWwindow*, double, double)>>     scrollButtonCallbacks;
    std::vector<std::function<bool(GLFWwindow*, int, int, int, int)>> keyCallbacks;
    std::vector<std::function<bool(GLFWwindow*, unsigned int)>>       charCallbacks;

    /**
     * functions that are executed in each frame life-cycle
     */
    std::vector<std::function<void()>> preEventHooks;
    std::vector<std::function<void()>> renderHooks;
    std::vector<std::function<void()>> postRenderHooks;
};
