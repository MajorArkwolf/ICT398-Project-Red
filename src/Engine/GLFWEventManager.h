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
		

	private:
		std::queue<Input::InputEvent> EventQueue;
	};
}
