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

	/**
	* @brief A struct use to represent possible information received from GLFW input callbacks
	*/
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
		using Data = std::variant<std::monostate, Position, Size, Scroll, Keyboard, Mouse, File, Scale>;
		Data data = {};
		GLFWwindow* window = nullptr;
	};

	/**
	* @class GLFWInputWrangler
	* @brief A class used to convert GLFW inputs into something more usable by our engine
	*/
	class GLFWInputWrangler
	{
	public:
		//
		GLFWInputWrangler() = delete;
		GLFWInputWrangler(const GLFWInputWrangler& other) = delete;
		GLFWInputWrangler(const GLFWInputWrangler&& other) = delete;
		GLFWInputWrangler& operator=(const GLFWInputWrangler& rhs) = delete;
		GLFWInputWrangler& operator=(const GLFWInputWrangler&& rhs) = delete;
		~GLFWInputWrangler() = delete;

		/**
		* 
		* @param window The GLFW window context
		*/
		static void Init(GLFWwindow* window);
        /**
		* Sets the GLFW callbacks form the window context to the other functions in this class
		* @param window The GLFW window context
		*/
		static void TrackWindow(GLFWwindow* window);
        /**
		* Turns the passed in event reference to the newest event in the event queue, returns true if there are still events left
		* @param window The GLFW window context
		*/
		static bool PollEvent(GLFWEvent& event);


	private:
        /**
		* Call back for window posistion changes
		* @param window The GLFW window context
		* @param x X position of the window
		* @param y Y position of the window
		*/
		static void WindowPosCallback(GLFWwindow* window, int x, int y);
     /**
		* Callback for changing the window size
		* @width Width of the wnidow
		* @height height of the window
		* @param window The GLFW window context
		*/
		static void WindowSizeCallback(GLFWwindow* window, int width, int height);
        /**
		* Callback for closing the window
		* @param window The GLFW window context
		*/
		static void WindowCloseCallback(GLFWwindow* window);
        /**
		* Callback for window refresh
		* @param window The GLFW window context
		*/
		static void WindowRefreshCallback(GLFWwindow* window);
        /**
		* Callback for window focus
		* @param focused
		* @param window The GLFW window context
		*/
		static void WindowFocusCallback(GLFWwindow* window, int focused);
        /**
		* Callback for the window iconification
		* @param iconified
		* @param window The GLFW window context
		*/
		static void WindowIconifyCallback(GLFWwindow* window, int iconified);
        /**
		* Callback for the framebuffer
		* @param width Width of the framebuffer
		* @param height Height of the framebuffer
		* @param window The GLFW window context
		*/
		static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
        /**
		* Callback for the mouse button
		* @param button The button that was actioned
		* @param action The type of actioned that occured
		* @param mods The modifier buttons that were active
		* @param window The GLFW window context
		*/
		static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
        /**
		* The cursor position callback
		* @param x The X position of the cursor
		* @param y The Y position of the cursor
		* @param window The GLFW window context
		*/
		static void CursorPositionCallback(GLFWwindow* window, double x, double y);
        /**
		* The cursor entering window callback
		* @param entered
		* @param window The GLFW window context
		*/
		static void CursorEnterCallback(GLFWwindow* window, int entered);
        /**
		* Mouse scroll callback
		* @param x The amount the mouse was scrolled left and right
		* @param y The amount the mouse was scrolled up and down
		* @param window The GLFW window context
		*/
		static void MouseScrollCallback(GLFWwindow* window, double x, double y);
        /**
		* The keyboard callback function
		* @param key The key that was actioned
		* @param scancode The scancode that was actioned
		* @param action The action that occured, e.g. press or release
		* @param mods The modifier keys that were active
		* @param window The GLFW window context
		*/
		static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        /**
		* 
		* @param window The GLFW window context
		*/
		static void CharCallback(GLFWwindow* window, unsigned codepoint);
        /**
		* 
		* @param window The GLFW window context
		*/
		static void MonitorCallback(GLFWwindow* window, int action);
        /**
		* 
		* @param window The GLFW window context
		*/
		static void FileDropCallback(GLFWwindow* window, int count, const char** paths);
        /**
		* 
		* @param window The GLFW window context
		*/
		static void JoystickCallback(int jid, int action);
        /**
		* 
		* @param window The GLFW window context
		*/
		static void WindowMaximiseCallback(GLFWwindow* window, int maximised);
        /**
		* 
		* @param window The GLFW window context
		*/
		static void WindowContentScaleCallback(GLFWwindow* window, float xscale, float yscale);


	};
}
