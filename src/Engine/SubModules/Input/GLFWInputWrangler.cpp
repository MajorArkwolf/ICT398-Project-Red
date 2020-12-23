#include "GLFWInputWrangler.hpp"

static std::queue<input::GLFWEvent> event_queue_;

void input::GLFWInputWrangler::Init(GLFWwindow* window)
{
	TrackWindow(window);
}

void input::GLFWInputWrangler::TrackWindow(GLFWwindow* window)
{
	glfwSetWindowPosCallback(window, WindowPosCallback);
	glfwSetWindowSizeCallback(window, WindowSizeCallback);
	glfwSetWindowCloseCallback(window, WindowCloseCallback);
	glfwSetWindowRefreshCallback(window, WindowRefreshCallback);
	glfwSetWindowFocusCallback(window, WindowFocusCallback);
	glfwSetWindowIconifyCallback(window, WindowIconifyCallback);
	glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
	glfwSetMouseButtonCallback(window, MouseButtonCallback);
	glfwSetCursorPosCallback(window, CursorPositionCallback);
	glfwSetCursorEnterCallback(window, CursorEnterCallback);
	glfwSetScrollCallback(window, MouseScrollCallback);
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCharCallback(window, CharCallback);
#if GLFW_VERSION_MINOR >= 1
	glfwSetDropCallback(window, FileDropCallback);
#endif
#if GLFW_VERSION_MINOR >= 3
	glfwSetWindowMaximizeCallback(window, WindowMaximiseCallback);
	glfwSetWindowContentScaleCallback(window, WindowContentScaleCallback);
#endif
}

bool input::GLFWInputWrangler::PollEvent(GLFWEvent& event)
{
	if (event_queue_.empty())
	{
		return false;
	}
	//So reference isnt deleted
	auto newEvent = event_queue_.front();
	event = newEvent;
	event_queue_.pop();
	return true;
}

void input::GLFWInputWrangler::WindowPosCallback(GLFWwindow* window, int x, int y)
{
	GLFWEvent event;
	event.type = GLFWEventType::kWindowMoved;
	GLFWEvent::Position pos = { x, y };
	event.data.emplace<GLFWEvent::Position>(pos);
	event.window = window;
	event_queue_.push(event);
}

void input::GLFWInputWrangler::WindowSizeCallback(GLFWwindow* window, int width, int height)
{
	GLFWEvent event;
	event.type = GLFWEventType::kWindowResized;
	GLFWEvent::Size size = { width,height };
	event.data.emplace<GLFWEvent::Size>(size);
	event.window = window;
	event_queue_.push(event);
}

void input::GLFWInputWrangler::WindowCloseCallback(GLFWwindow* window)
{
	GLFWEvent event;
	event.type = GLFWEventType::kWindowClosed;
	event.window = window;
	event_queue_.push(event);
}

void input::GLFWInputWrangler::WindowRefreshCallback(GLFWwindow* window)
{
	GLFWEvent event;
	event.type = GLFWEventType::kWindowRefresh;
	event.window = window;
	event_queue_.push(event);
}

void input::GLFWInputWrangler::WindowFocusCallback(GLFWwindow* window, int focused)
{
	GLFWEvent event;
	if (focused == 1)
	{
		event.type = GLFWEventType::kWindowFocused;
	}
	else
	{
		event.type = GLFWEventType::kWindowDefocused;
	}

	event.window = window;
	event_queue_.push(event);
}

void input::GLFWInputWrangler::WindowIconifyCallback(GLFWwindow* window, int iconified)
{
	GLFWEvent event;
	if (iconified == 1)
	{
		event.type = GLFWEventType::kWindowIconified;
	}
	else
	{
		event.type = GLFWEventType::kWindowUnIconified;
	}
	event.window = window;
	event_queue_.push(event);
}

void input::GLFWInputWrangler::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	GLFWEvent event;
	event.type = GLFWEventType::kFramebufferResized;
	GLFWEvent::Size size = { width,height };
	event.data.emplace<GLFWEvent::Size>(size);
	event.window = window;
	event_queue_.push(event);
}

void input::GLFWInputWrangler::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	GLFWEvent event;
	event.type = GLFWEventType::kButtonPressed;
	GLFWEvent::Mouse mouse = { button, mods };

	if (action == GLFW_PRESS)
	{
		event.type = GLFWEventType::kButtonPressed;
	}
	else if (action == GLFW_RELEASE)
	{
		event.type = GLFWEventType::kButtonReleased;
	}


	event.data.emplace<GLFWEvent::Mouse>(mouse);
	event.window = window;
	event_queue_.push(event);
}

void input::GLFWInputWrangler::CursorPositionCallback(GLFWwindow* window, double x, double y)
{
	GLFWEvent event;
	event.type = GLFWEventType::kCursorMoved;
	GLFWEvent::Position pos = { static_cast<int>(x),static_cast<int>(y) };
	event.data.emplace<GLFWEvent::Position>(pos);
	event.window = window;
	event_queue_.push(event);
}

void input::GLFWInputWrangler::CursorEnterCallback(GLFWwindow* window, int entered)
{
	GLFWEvent event;
	event.type = GLFWEventType::kCursorEnteredWindow;
	if (entered == 1)
	{
		event.type = GLFWEventType::kCursorEnteredWindow;
	}
	else
	{
		event.type = GLFWEventType::kCursorLeftWindow;
	}
	event.window = window;
	event_queue_.push(event);
}

void input::GLFWInputWrangler::MouseScrollCallback(GLFWwindow* window, double x, double y)
{
	GLFWEvent event;
	event.type = GLFWEventType::kMouseScrolled;
	GLFWEvent::Scroll scroll = { static_cast<int>(x),static_cast<int>(y) };
	event.data.emplace<GLFWEvent::Scroll>(scroll);
	event.window = window;
	event_queue_.push(event);
}

void input::GLFWInputWrangler::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	GLFWEvent event;
	if( action == GLFW_PRESS)
	{
		event.type = GLFWEventType::kKeyPressed;
	} else if (action == GLFW_RELEASE)
	{
		event.type = GLFWEventType::kKeyReleased;
	}else if (action == GLFW_REPEAT)
	{
		event.type = GLFWEventType::kKeyRepeated;
	}
	GLFWEvent::Keyboard keyboard = { key, scancode, mods};
	event.data.emplace<GLFWEvent::Keyboard>(keyboard);
	event.window = window;
	event_queue_.push(event);
}

void input::GLFWInputWrangler::CharCallback([[maybe_unused]] GLFWwindow* window,[[maybe_unused]] unsigned codepoint)
{
	//Do nothing lol
}

void input::GLFWInputWrangler::MonitorCallback(GLFWwindow* window, int action)
{
	GLFWEvent event;

	if (action == GLFW_CONNECTED)
	{
		event.type = GLFWEventType::kMonitorConnected;
	}
	else if (action == GLFW_DISCONNECTED)
	{
		event.type = GLFWEventType::kMonitorDisconnected;
	}

	event.window = window;
	event_queue_.push(event);
}

void input::GLFWInputWrangler::FileDropCallback([[maybe_unused]] GLFWwindow* window, [[maybe_unused]] int count, [[maybe_unused]] const char** paths)
{
	//do nothing lol
}

void input::GLFWInputWrangler::JoystickCallback([[maybe_unused]] int jid, [[maybe_unused]] int action)
{
	//do nothing lol
}

void input::GLFWInputWrangler::WindowMaximiseCallback(GLFWwindow* window, int maximised)
{
	GLFWEvent event;

	if (maximised == 1)
	{
		event.type = GLFWEventType::kWindowMaximised;
	}
	else
	{
		event.type = GLFWEventType::kWindowUnMaximised;
	}

	event.window = window;
	event_queue_.push(event);
}

void input::GLFWInputWrangler::WindowContentScaleCallback(GLFWwindow* window, float xscale, float yscale)
{
	GLFWEvent event;
	event.type = GLFWEventType::kWindowScaleChanged;
	GLFWEvent::Scale scale = {xscale, yscale};
	event.data.emplace<GLFWEvent::Scale>(scale);

	event.window = window;
	event_queue_.push(event);
}
