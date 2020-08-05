#pragma once

#include <queue>
#include <string>
#include <variant>

#include <glad/glad.h>
#include "GLFW/glfw3.h"

namespace input
{
	struct InputEvent;

	enum class GLFWEventType
	{
		kNone
		, kWindowMoved
		, kWindowResized
		, kWindowClosed
		, kWindowRefresh
		, kWindowFocused
		, kWindowDefocused
		, kWindowIconified
		, kWindowUnIconified
		, kFramebufferResized
		, kButtonPressed
		, kButtonReleased
		, kCursorMoved
		, kCursorEnteredWindow
		, kCursorLeftWindow
		, kMouseScrolled
		, kKeyPressed
		, kKeyReleased
		, kKeyRepeated
		, kMonitorConnected
		, kMonitorDisconnected
		, kFileDropped
		, kWindowMaximised
		, kWindowUnMaximised
		, kWindowScaleChanged
	};

	struct GLFWEvent
	{
		struct Position
		{
			int x = 0;
			int y = 0;
		};
		struct Size
		{
			int width = 0;
			int height = 0;
		};
		struct Scroll
		{
			int x = 0;
			int y = 0;
		};
		struct Keyboard
		{
			int key = 0;
			int scancode = 0;
			int mods = 0;
		};
		struct Mouse
		{
			int button = 0;
			int mods = 0;
		};
		struct File
		{
			std::vector<std::string> paths = {};
		};
		struct Scale
		{
			float x = 0.f;
			float y = 0.f;
		};

		GLFWEventType type = GLFWEventType::kNone;
		std::variant<std::monostate, Position, Size, Scroll, Keyboard, Mouse, File, Scale> data = {};
		GLFWwindow* window = nullptr;
	};

	class GLFWInputWrangler
	{
	public:
		GLFWInputWrangler() = delete;
		GLFWInputWrangler(const GLFWInputWrangler& other) = delete;
		GLFWInputWrangler(const GLFWInputWrangler&& other) = delete;
		GLFWInputWrangler& operator=(const GLFWInputWrangler& rhs) = delete;
		GLFWInputWrangler& operator=(const GLFWInputWrangler&& rhs) = delete;
		~GLFWInputWrangler() = delete;

		static void Init(GLFWwindow* window);
		static void TrackWindow(GLFWwindow* window);
		static bool PollEvent(GLFWEvent& event);


	private:
		static void WindowPosCallback(GLFWwindow* window, int x, int y);
		static void WindowSizeCallback(GLFWwindow* window, int width, int height);
		static void WindowCloseCallback(GLFWwindow* window);
		static void WindowRefreshCallback(GLFWwindow* window);
		static void WindowFocusCallback(GLFWwindow* window, int focused);
		static void WindowIconifyCallback(GLFWwindow* window, int iconified);
		static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
		static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
		static void CursorPositionCallback(GLFWwindow* window, double x, double y);
		static void CursorEnterCallback(GLFWwindow* window, int entered);
		static void MouseScrollCallback(GLFWwindow* window, double x, double y);
		static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void CharCallback(GLFWwindow* window, unsigned codepoint);
		static void MonitorCallback(GLFWwindow* window, int action);
		static void FileDropCallback(GLFWwindow* window, int count, const char** paths);
		static void JoystickCallback(int jid, int action);
		static void WindowMaximiseCallback(GLFWwindow* window, int maximised);
		static void WindowContentScaleCallback(GLFWwindow* window, float xscale, float yscale);


	};
}
