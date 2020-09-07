#pragma once
#include <map>
#include <variant>
#include <array>
#include "InputEnums.hpp"
#include "GLFWInputWrangler.hpp"

namespace input
{
	using PhysicalKey = VirtualKey;

	struct InputEvent
	{
		struct MouseEvent
		{
			MouseButton button = MouseButton::kNone;
			PhysicalKey mods = PhysicalKey::kNone;
		};

		struct KeyboardEvent
		{
			PhysicalKey key = PhysicalKey::kKeyLast;
			PhysicalKey mods = PhysicalKey::kKeyLast;
		};

		struct iVector2
		{
			int x = 0;
			int y = 0;
		};

		struct dVector2
		{
			double x = 0;
			double y = 0;
		};

		InputType type = InputType::kNone;
		using Data = std::variant<std::monostate, MouseEvent, KeyboardEvent, iVector2, dVector2>;
		Data data;
	};

	class InputManager
	{
	public:
		InputManager();
		InputManager(const InputManager& rhs) = delete;
		InputManager(const InputManager&& rhs) = delete;
		InputManager& operator = (InputManager& rhs) = delete;
		InputManager& operator = (InputManager&& rhs) = delete;
		InputEvent ConvertEvent(const GLFWEvent& event);
		void RecordKeyStates(const InputEvent& event);
		PhysicalKey ConvertGLFWKey(int key);
        std::string& HashKeyToString(VirtualKey key);
        std::map<PhysicalKey, VirtualKey>& GetInputMap();
        std::array<bool, static_cast<int>(VirtualKey::kKeyLast)>& GetKeyStates();
        std::map<PhysicalKey, std::string>& GetStringMap();
		~InputManager();


	private:
        VirtualKey ConvertPhysicalToVirtual(PhysicalKey key);
		void PopulateInputMap();
		void PopulateGLFWKeyMap();
        void PopulateStringMap();
		/**
		 *Maps a physical key to a virtual key
		 */
		std::map<PhysicalKey, VirtualKey> input_map;
		std::map<int, PhysicalKey> glfw_key_map;
        std::map<PhysicalKey, std::string> key_string_map;

		std::array<bool, static_cast<int>(VirtualKey::kKeyLast)> key_states = {false};


	};

}

