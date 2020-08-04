#pragma once
#include <queue>
#include "InputManager.hpp"

namespace input {
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

		bool PollEvents(input::InputEvent& event);
		void AddEventToQueue(input::InputEvent& event);
	private:
		std::queue<InputEvent> event_queue_;
	};
}