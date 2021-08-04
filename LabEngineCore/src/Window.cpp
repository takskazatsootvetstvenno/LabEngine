#include <GL/glew.h>
#include "Window.hpp"

#include <glm/glm.hpp>

#include <GLFW/glfw3.h>

#include "Log.hpp"
#include "Rendering/OpenGL/ShaderProgram.hpp"
#include "Rendering/OpenGL/VertexBuffer.hpp"
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>
namespace LabEngine {

static bool s_GLFW_initialized = false;

GLfloat points[] = {0.0f, 0.5f, 0.0f, 0.5f, -0.5f, 0.0f, -0.5f, -0.5f, 0.0f};

GLfloat colors[] = {1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f};

const char* vertex_shader =
    "#version 460\n"
    "layout(location = 0) in vec3 vertex_position;"
    "layout(location = 1) in vec3 vertex_color;"
    "out vec3 color;"
    "void main() {"
    "   color = vertex_color;"
    "   gl_Position = vec4(vertex_position, 1.0);"
    "}";

const char* fragment_shader =
    "#version 460\n"
    "in vec3 color;"
    "out vec4 frag_color;"
    "void main() {"
    "   frag_color = vec4(color, 1.0);"
    "}";

GLuint vao;
std::unique_ptr<ShaderProgram> p_shader_program;
std::unique_ptr<VertexBuffer> p_points_vbo;
std::unique_ptr<VertexBuffer> p_colors_vbo;

Window::Window(std::string title, const unsigned int width, const unsigned int height)
    : m_data({std::move(title), width, height}) {
    int resultCode = init();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplOpenGL3_Init();
    ImGui_ImplGlfw_InitForOpenGL(m_pWindow, true);
}
Window::~Window() {
    shutdown();
}
void Window::shutdown() {
    glfwDestroyWindow(m_pWindow);
    glfwTerminate();
}
int Window::init() {
    LOG_INFO("Creating window \'" + m_data.m_title + "\' with size: " +
             std::to_string(m_data.m_width) + "|" + std::to_string(m_data.m_height));
    if (!s_GLFW_initialized)
        if (!glfwInit()) {
            LOG_CRIT("Can't initialize GLFW!")
            return -1;
        } else
            s_GLFW_initialized = true;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_pWindow = glfwCreateWindow(m_data.m_width, m_data.m_height, m_data.m_title.c_str(), NULL,
                                 NULL);
    if (!m_pWindow) {
        LOG_CRIT("Can't create Window!" + m_data.m_title + "with size: " +
                 std::to_string(m_data.m_width) + "|" + std::to_string(m_data.m_height))
        glfwTerminate();
        return -2;
    }

    glfwMakeContextCurrent(m_pWindow);
    glfwSwapInterval(0);
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) { LOG_INFO("Can't initialize GLEW!") }

    glfwSetWindowUserPointer(m_pWindow, &m_data);
    glfwSetWindowSizeCallback(m_pWindow, [](GLFWwindow* pWindow, int width, int height) {
        // LOG_INFO("New Size " + std::to_string(width) + "|" + std::to_string(height))

        WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));
        data.m_width = width;
        data.m_height = height;

        EventWindowResize event(width, height);
        data.eventCallbackFn(event);
    });

    glfwSetCursorPosCallback(m_pWindow, [](GLFWwindow* pWindow, double x, double y) {
        WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));

        EventMouseMoved event(x, y);
        data.eventCallbackFn(event);
    });
    glfwSetWindowCloseCallback(m_pWindow, [](GLFWwindow* pWindow) {
        WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));

        EventWindowClose event;
        data.eventCallbackFn(event);
    });

    glfwSetFramebufferSizeCallback(m_pWindow, [](GLFWwindow* pWindow, int width, int height) {
        glViewport(0, 0, width, height);
    });

    p_shader_program = std::make_unique<ShaderProgram>(vertex_shader, fragment_shader);

    if (!p_shader_program->isCompiled()) 
        return false;

    p_points_vbo = std::make_unique<VertexBuffer>(points, sizeof(points));
    p_colors_vbo = std::make_unique<VertexBuffer>(colors, sizeof(colors));

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glEnableVertexAttribArray(0);
    p_points_vbo->bind();
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glEnableVertexAttribArray(1);
    p_colors_vbo->bind();
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    return 0;
}
void Window::on_update() {
    glClearColor(m_background_color[0], m_background_color[1], m_background_color[2],
                 m_background_color[3]);
    glClear(GL_COLOR_BUFFER_BIT);

    p_shader_program->bind();
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize.x = static_cast<float>(get_width());
    io.DisplaySize.y = static_cast<float>(get_height());

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    // ImGui::ShowDemoWindow();
    ImGui::Begin("background selector");
    ImGui::ColorEdit4("Background color", m_background_color);
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(m_pWindow);
    glfwPollEvents();
    // while (!glfwWindowShouldClose(m_pWindow)) {
}
}  // namespace LabEngine
