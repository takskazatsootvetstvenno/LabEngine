#ifndef Window_HPP
#define Window_HPP
#include "Event.hpp"
#include <functional>
#include <string>
struct GLFWwindow;

namespace LabEngine{
	class Window
	{
	public:
		using EventCallbackFn = std::function<void(BaseEvent&)>;

		Window(std::string title, const unsigned int width, const unsigned int height);
		virtual ~Window();
		Window(const Window&) = delete;
		Window(Window&&) = delete;
		Window& operator=(const Window&) = delete;
		Window& operator=(Window&&) = delete;

		virtual void on_update();
		unsigned int get_width() const { return m_data.m_width; }
		unsigned int get_height() const { return m_data.m_height; }

		void set_event_callback(const EventCallbackFn& callback)
		{
			m_data.eventCallbackFn = callback;
		}
	private:
		struct WindowData
		{
			std::string m_title;
			unsigned int m_width;
			unsigned int m_height;
			EventCallbackFn eventCallbackFn;
		};
		int init();
		void shutdown();

		GLFWwindow* m_pWindow = nullptr;
		WindowData m_data;
		float m_background_color[4] = { 0.33f, 0.33f, 0.33f, 0.f };
	};
}
#endif//Window_HPP
