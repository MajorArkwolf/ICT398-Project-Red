#pragma once
#include <queue>
#include "InputManager.h"

namespace Input {
	struct InputEvent;
	class RedEventManager
	{
	public:
		RedEventManager() = default;
		RedEventManager(const RedEventManager& other) = delete;
		RedEventManager(const RedEventManager&& other) = delete;
		RedEventManager& operator=(const RedEventManager& rhs) = delete;
		RedEventManager& operator=(const RedEventManager&& rhs) = delete;
		~RedEventManager() = default;

		bool PollEvents(Input::InputEvent& event);
		void AddEventToQueue(Input::InputEvent& event);
	private:
		std::queue<InputEvent> EventQueue;
	};
}