#include "Application.hpp"
#include "Log.hpp"
#include "Window.hpp"
//#define STB_IMAGE_IMPLEMENTATION
/////#include "stb_image.h"

#include <iostream>
namespace LabEngine {

Application::Application() {
    LOG_INFO("Starting Application")
}
Application::~Application() {
    LOG_INFO("Closing Application")
}

int Application::start(unsigned int window_width, unsigned int window_height,
                              const char* title) {

    m_pWindow = std::make_unique<Window>(title, window_width, window_height);

    m_event_dispather.add_event_listener <EventMouseMoved>(
        [](EventMouseMoved& event)
        {
            LOG_INFO("[MouseMoved] Mouse moved to " + std::to_string(event.x) +" x " + std::to_string(event.y))
        }
    );
     m_event_dispather.add_event_listener <EventWindowResize>(
        [](EventWindowResize& event)
        {
            LOG_INFO(" [Resized] Changing size to " + std::to_string(event.width) + " x " + std::to_string(event.height))
        }
    );
     m_event_dispather.add_event_listener <EventWindowClose>(
        [&](EventWindowClose& event)
        {
             LOG_INFO(" [WindowClose]")
             m_bCloseWindow = true;
        }
    );
    m_pWindow->set_event_callback(
        [&](BaseEvent& event)
        {
            m_event_dispather.dispatch(event);
        }
    );
    while (!m_bCloseWindow)
    {
        m_pWindow->on_update();
        on_update();
    }
    m_pWindow = nullptr;
    return 0;
}

}  // namespace LabEngine
