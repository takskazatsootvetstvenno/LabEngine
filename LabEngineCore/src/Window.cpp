#include <GL/glew.h>
#include "Window.hpp"

#include <glm/glm.hpp>

#include <GLFW/glfw3.h>

#include "Log.hpp"
#include "Rendering/OpenGL/ShaderProgram.hpp"
#include "Rendering/OpenGL/VertexBuffer.hpp"
#include "Rendering/OpenGL/VertexArray.hpp"
#include "Rendering/OpenGL/IndexBuffer.hpp"
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>
namespace LabEngine {

static bool s_GLFW_initialized = false;

GLfloat points[] = {
    0.0f, 0.5f, 0.0f,
    0.5f, -0.5f, 0.0f, 
    -0.5f, -0.5f, 0.0f
};

GLfloat colors[] = {1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f};

GLfloat positions_colors[] = {
 0.0f, 0.5f, 0.0f,   1.0f, 1.0f, 0.0f,
0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 1.0f,
-0.5f, -0.5f, 0.0f,   1.0f, 1.0f, 1.0f,
-0.5f, 0.5f, 0.0f,   0.0f, 1.0f, 1.0f
};

int indices[] = { 0, 1, 2, 3, 0, 2 };

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

std::unique_ptr<VertexBuffer> p_positions_colors_vbo;
std::unique_ptr<VertexArray> p_vao_1buffer;
std::unique_ptr<IndexBuffer> p_index_buffer;
Window::Window(std::string title, const unsigned int width, const unsigned int height)
    : m_data({std::move(title), width, height}) {
    int resultCode = init();
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
    glfwSetWindowSizeCallback(m_pWindow, 
        [](GLFWwindow* pWindow, int width, int height) 
        {
        WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));
        data.m_width = width;
        data.m_height = height;

        EventWindowResize event(width, height);
        data.eventCallbackFn(event);
        }
    );

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

    BufferLayout buffer_layout_1vec3
    {
        ShaderDataType::Float3
    };

    BufferLayout buffer_layout_2vec3
    {
        ShaderDataType::Float3,
        ShaderDataType::Float3
    };
    p_positions_colors_vbo = std::make_unique<VertexBuffer>(positions_colors, sizeof(positions_colors), buffer_layout_2vec3);
    p_index_buffer = std::make_unique<IndexBuffer>(indices, sizeof(indices) / sizeof(GLuint));
    p_vao_1buffer = std::make_unique<VertexArray>();

    p_vao_1buffer->add_vertex_buffer(*p_positions_colors_vbo);
    p_vao_1buffer->set_index_buffer(*p_index_buffer);
    return 0;
}
void Window::on_update() {
    
    glClearColor(m_background_color[0], m_background_color[1], m_background_color[2],
                 m_background_color[3]);
    glClear(GL_COLOR_BUFFER_BIT);
    p_shader_program->bind();
    p_vao_1buffer->bind();
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glfwSwapBuffers(m_pWindow);
    glfwPollEvents();
}
}  // namespace LabEngine
