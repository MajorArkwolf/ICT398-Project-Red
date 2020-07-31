#include "RedEventManager.hpp"
#include "InputManager.h"

bool Input::RedEventManager::PollEvents(Input::InputEvent& event)
{
	if (EventQueue.empty())
	{
		return false;
	}
	
	event = EventQueue.front();
	EventQueue.pop();
	return true;

}

void Input::RedEventManager::AddEventToQueue(Input::InputEvent& event)
{
	EventQueue.emplace(event);
}
