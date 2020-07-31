#include "GLFWInputWrangler.h"

void Input::GLFWInputWrangler::Init(GLFWwindow* window)
{
	TrackWindow(window);
}

void Input::GLFWInputWrangler::TrackWindow(GLFWwindow* window)
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

void Input::GLFWInputWrangler::WindowPosCallback(GLFWwindow* window, int x, int y)
{
	GLFWEvent event;
	event.Type = GLFWEventType::WindowMoved;
	GLFWEvent::Position pos = { x, y };
	event.Data.emplace<GLFWEvent::Position>(pos);
	event.Window = window;
	EventQueue.push(event);
}

void Input::GLFWInputWrangler::WindowSizeCallback(GLFWwindow* window, int width, int height)
{
	GLFWEvent event;
	event.Type = GLFWEventType::WindowResized;
	GLFWEvent::Size size = { width,height };
	event.Data.emplace<GLFWEvent::Size>(size);
	event.Window = window;
	EventQueue.push(event);
}

void Input::GLFWInputWrangler::WindowCloseCallback(GLFWwindow* window)
{
	GLFWEvent event;
	event.Type = GLFWEventType::WindowClosed;
	event.Window = window;
	EventQueue.push(event);
}

void Input::GLFWInputWrangler::WindowRefreshCallback(GLFWwindow* window)
{
	GLFWEvent event;
	event.Type = GLFWEventType::WindowRefresh;
	event.Window = window;
	EventQueue.push(event);
}

void Input::GLFWInputWrangler::WindowFocusCallback(GLFWwindow* window, int focused)
{
	GLFWEvent event;
	if (focused == 1)
	{
		event.Type = GLFWEventType::WindowFocused;
	}
	else
	{
		event.Type = GLFWEventType::WindowDefocused;
	}

	event.Window = window;
	EventQueue.push(event);
}

void Input::GLFWInputWrangler::WindowIconifyCallback(GLFWwindow* window, int iconified)
{
	GLFWEvent event;
	if (iconified == 1)
	{
		event.Type = GLFWEventType::WindowIconified;
	}
	else
	{
		event.Type = GLFWEventType::WindowUnIconified;
	}
	event.Window = window;
	EventQueue.push(event);
}

void Input::GLFWInputWrangler::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	GLFWEvent event;
	event.Type = GLFWEventType::FramebufferResized;
	GLFWEvent::Size size = { width,height };
	event.Data.emplace<GLFWEvent::Size>(size);
	event.Window = window;
	EventQueue.push(event);
}

void Input::GLFWInputWrangler::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	GLFWEvent event;
	event.Type = GLFWEventType::WindowResized;
	GLFWEvent::Mouse mouse = { button, mods };

	if (action == GLFW_PRESS)
	{
		event.Type = GLFWEventType::ButtonPressed;
	}
	else if (action == GLFW_RELEASE)
	{
		event.Type = GLFWEventType::ButtonReleased;
	}


	event.Data.emplace<GLFWEvent::Mouse>(mouse);
	event.Window = window;
	EventQueue.push(event);
}

void Input::GLFWInputWrangler::CursorPositionCallback(GLFWwindow* window, double x, double y)
{
	GLFWEvent event;
	event.Type = GLFWEventType::CursorMoved;
	GLFWEvent::Position pos = { x,y };
	event.Data.emplace<GLFWEvent::Position>(pos);
	event.Window = window;
	EventQueue.push(event);
}

void Input::GLFWInputWrangler::CursorEnterCallback(GLFWwindow* window, int entered)
{
	GLFWEvent event;
	event.Type = GLFWEventType::WindowResized;
	if (entered == 1)
	{
		event.Type = GLFWEventType::CursorEnteredWindow;
	}
	else
	{
		event.Type = GLFWEventType::CursorLeftWindow;
	}
	event.Window = window;
	EventQueue.push(event);
}

void Input::GLFWInputWrangler::MouseScrollCallback(GLFWwindow* window, double x, double y)
{
	GLFWEvent event;
	event.Type = GLFWEventType::MouseScrolled;
	GLFWEvent::Scroll scroll = { x,y };
	event.Data.emplace<GLFWEvent::Scroll>(scroll);
	event.Window = window;
	EventQueue.push(event);
}

void Input::GLFWInputWrangler::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	GLFWEvent event;
	if( action == GLFW_PRESS)
	{
		event.Type = GLFWEventType::KeyPressed;
	} else if (action == GLFW_RELEASE)
	{
		event.Type = GLFWEventType::KeyReleased;
	}else if (action == GLFW_REPEAT)
	{
		event.Type = GLFWEventType::KeyRepeated;
	}
	GLFWEvent::Keyboard keyboard = { key, scancode, mods};
	event.Data.emplace<GLFWEvent::Keyboard>(keyboard);
	event.Window = window;
	EventQueue.push(event);
}

void Input::GLFWInputWrangler::CharCallback([[maybe_unused]] GLFWwindow* window,[[maybe_unused]] unsigned codepoint)
{
	//Do nothing lol
}

void Input::GLFWInputWrangler::MonitorCallback(GLFWwindow* window, int action)
{
	GLFWEvent event;

	if (action == GLFW_CONNECTED)
	{
		event.Type = GLFWEventType::MonitorConnected;
	}
	else if (action == GLFW_DISCONNECTED)
	{
		event.Type = GLFWEventType::MonitorDisconnected;
	}

	event.Window = window;
	EventQueue.push(event);
}

void Input::GLFWInputWrangler::FileDropCallback([[maybe_unused]] GLFWwindow* window, [[maybe_unused]] int count, [[maybe_unused]] const char** paths)
{
	//do nothing lol
}

void Input::GLFWInputWrangler::JoystickCallback([[maybe_unused]] int jid, [[maybe_unused]] int action)
{
	//do nothing lol
}

void Input::GLFWInputWrangler::WindowMaximiseCallback(GLFWwindow* window, int maximised)
{
	GLFWEvent event;

	if (maximised == 1)
	{
		event.Type = GLFWEventType::WindowMaximised;
	}
	else
	{
		event.Type = GLFWEventType::WindowUnMaximised;
	}

	event.Window = window;
	EventQueue.push(event);
}

void Input::GLFWInputWrangler::WindowContentScaleCallback(GLFWwindow* window, float xscale, float yscale)
{
	GLFWEvent event;
	event.Type = GLFWEventType::WindowScaleChanged;
	GLFWEvent::Scale scale = {xscale, yscale};
	event.Data.emplace<GLFWEvent::Scale>(scale);

	event.Window = window;
	EventQueue.push(event);
}
