#pragma once

#include <queue>
#include <string>


#include "GLFW/glfw3.h"

namespace Input
{
	struct InputEvent;

	enum  GLFWEventType
	{
		  None
		, WindowMoved
		, WindowResized
		, WindowClosed
		, WindowRefresh
		, WindowDefocused
		, WindowIconified
		, WindowUnIconified
		, FramebufferResized
		, ButtonPressed
		, ButtonReleased
		, CursorMoved
		, CursorEnteredWindow
		, CursorLeftWindow
		, MouseScrolled
		, KeyPressed
		, KeyReleased
		, KeyRepeated
		, MonitorConnected
		, MonitorDisconnected
		, FileDropped
		, WindowMaximised
		, WindowUnMaximised
		, WindowScaleChanged
	};

	struct GLFWEvent
	{
		GLFWEventType Type;
		union 
		{
			struct Position
			{
				int X = 0;
				int Y = 0;
			};
			struct Size
			{
				int Width = 0;
				int Height = 0;
			};
			struct Scroll
			{
				int X = 0;
				int Y = 0;
			};
			struct Keyboard
			{
				int Key = 0;
				int Scancode = 0;
				int Mods = 0;
			};
			struct Mouse
			{
				int Button = 0;
				int Mods = 0;
			};
			struct File
			{
				std::vector<std::string> Paths;
			};
			struct Scale
			{
				float X = 0.f;
				float Y = 0.f;
			};
		};
	};

	class GLFWEventManager
	{
	public:
		GLFWEventManager() {};
		GLFWEventManager(const GLFWEventManager& other) = delete;
		GLFWEventManager(const GLFWEventManager&& other) = delete;
		GLFWEventManager& operator=(const GLFWEventManager& rhs) = delete;
		GLFWEventManager& operator=(const GLFWEventManager&& rhs) = delete;
		~GLFWEventManager() = delete;

		void Init(GLFWwindow* window);
		void TrackWindow(GLFWwindow* window);


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


		std::queue<Input::InputEvent> EventQueue;
	};
}
