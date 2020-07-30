#pragma once
#include <map>
#include <variant>
#include "InputEnums.h"

namespace Input
{
	using PhysicalKey = VirtualKey;
	
	struct InputEvent
	{
		struct MouseEvent
		{
			MouseButton Button = MouseButton::None;
			PhysicalKey Mods = PhysicalKey::None;
		};

		struct KeyboardEvent
		{
			PhysicalKey Key = PhysicalKey::Key_Last;
			PhysicalKey Mods = PhysicalKey::Key_Last;
		};
		
		struct Position
		{
			int X = 0;
			int Y = 0;
		};
		
		InputType Type = InputType::None;
		std::variant<std::monostate, MouseEvent, KeyboardEvent, Position> Input;
	};


	class InputManager
	{
	public:
		InputManager();
		InputManager(const InputManager& rhs) = delete;
		InputManager(const InputManager&& rhs) = delete;
		InputManager& operator = (InputManager& rhs) = delete;
		InputManager& operator = (InputManager&& rhs) = delete;
		~InputManager();

		
	private:

		void PopulateInputMap();
		/**
		 *Maps a physical key to a virtual key
		 */
		std::map<PhysicalKey, VirtualKey> InputMap;

		VirtualKey KeyState[static_cast<int>(VirtualKey::Key_Last)];


	};

}

