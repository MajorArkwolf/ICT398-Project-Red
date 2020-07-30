#include "RedEventManager.hpp"
#include "InputManager.h"

bool RedEventManager::PollEvents(Input::InputEvent& event)
{
	if (EventQueue.empty())
	{
		return false;
	}
	
	event = EventQueue.front();
	EventQueue.pop();
	return true;

}

void RedEventManager::AddEventToQueue(Input::InputEvent& event)
{
	EventQueue.emplace(event);
}
