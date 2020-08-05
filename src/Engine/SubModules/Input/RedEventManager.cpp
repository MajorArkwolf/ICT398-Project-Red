#include "RedEventManager.hpp"
#include "InputManager.hpp"

bool input::RedEventManager::PollEvents(input::InputEvent& event)
{
	if (event_queue_.empty())
	{
		return false;
	}
	
	event = event_queue_.front();
	event_queue_.pop();
	return true;

}

void input::RedEventManager::AddEventToQueue(input::InputEvent& event)
{
	event_queue_.emplace(event);
}
