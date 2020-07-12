#include "InputManager.h"

Input::InputManager::InputManager()
{
}

Input::InputManager::~InputManager()
{
}

void Input::InputManager::PopulateInputMap()
{
	InputMap.emplace(PhysicalKey::A, VirtualKey::A);
	InputMap.emplace(PhysicalKey::B, VirtualKey::B);
	InputMap.emplace(PhysicalKey::C, VirtualKey::C);
	InputMap.emplace(PhysicalKey::D, VirtualKey::D);
	InputMap.emplace(PhysicalKey::E, VirtualKey::E);
	InputMap.emplace(PhysicalKey::F, VirtualKey::F);
	InputMap.emplace(PhysicalKey::G, VirtualKey::G);
	InputMap.emplace(PhysicalKey::H, VirtualKey::H);
	InputMap.emplace(PhysicalKey::I, VirtualKey::I);
	InputMap.emplace(PhysicalKey::J, VirtualKey::J);
	InputMap.emplace(PhysicalKey::K, VirtualKey::K);
	InputMap.emplace(PhysicalKey::L, VirtualKey::L);
	InputMap.emplace(PhysicalKey::M, VirtualKey::M);
	InputMap.emplace(PhysicalKey::N, VirtualKey::N);
	InputMap.emplace(PhysicalKey::O, VirtualKey::O);
	InputMap.emplace(PhysicalKey::P, VirtualKey::P);
	InputMap.emplace(PhysicalKey::Q, VirtualKey::Q);
	InputMap.emplace(PhysicalKey::R, VirtualKey::R);
	InputMap.emplace(PhysicalKey::S, VirtualKey::S);
	InputMap.emplace(PhysicalKey::T, VirtualKey::T);
	InputMap.emplace(PhysicalKey::U, VirtualKey::U);
	InputMap.emplace(PhysicalKey::V, VirtualKey::V);
	InputMap.emplace(PhysicalKey::W, VirtualKey::W);
	InputMap.emplace(PhysicalKey::X, VirtualKey::X);
	InputMap.emplace(PhysicalKey::Y, VirtualKey::Y);
	InputMap.emplace(PhysicalKey::Z, VirtualKey::Z);
	InputMap.emplace(PhysicalKey::Key_1, VirtualKey::Key_1);
	InputMap.emplace(PhysicalKey::Key_2, VirtualKey::Key_2);
	InputMap.emplace(PhysicalKey::Key_3, VirtualKey::Key_3);
	InputMap.emplace(PhysicalKey::Key_4, VirtualKey::Key_4);
	InputMap.emplace(PhysicalKey::Key_5, VirtualKey::Key_5);
	InputMap.emplace(PhysicalKey::Key_6, VirtualKey::Key_6);
	InputMap.emplace(PhysicalKey::Key_7, VirtualKey::Key_7);
	InputMap.emplace(PhysicalKey::Key_8, VirtualKey::Key_8);
	InputMap.emplace(PhysicalKey::Key_9, VirtualKey::Key_9);
	InputMap.emplace(PhysicalKey::Key_0, VirtualKey::Key_0);
	InputMap.emplace(PhysicalKey::F1, VirtualKey::F1);
	InputMap.emplace(PhysicalKey::F2, VirtualKey::F2);
	InputMap.emplace(PhysicalKey::F3, VirtualKey::F3);
	InputMap.emplace(PhysicalKey::F4, VirtualKey::F4);
	InputMap.emplace(PhysicalKey::F5, VirtualKey::F5);
	InputMap.emplace(PhysicalKey::F6, VirtualKey::F6);
	InputMap.emplace(PhysicalKey::F7, VirtualKey::F7);
	InputMap.emplace(PhysicalKey::F8, VirtualKey::F8);
	InputMap.emplace(PhysicalKey::F9, VirtualKey::F9);
	InputMap.emplace(PhysicalKey::F10, VirtualKey::F10);
	InputMap.emplace(PhysicalKey::F11, VirtualKey::F11);
	InputMap.emplace(PhysicalKey::F12, VirtualKey::F12);
	InputMap.emplace(PhysicalKey::Tab, VirtualKey::Tab);
	InputMap.emplace(PhysicalKey::LeftShift, VirtualKey::LeftShift);
	InputMap.emplace(PhysicalKey::LeftControl, VirtualKey::LeftControl);
	InputMap.emplace(PhysicalKey::LeftAlt, VirtualKey::LeftAlt);
	InputMap.emplace(PhysicalKey::RightShift, VirtualKey::RightShift);
	InputMap.emplace(PhysicalKey::RightControl, VirtualKey::RightControl);
	InputMap.emplace(PhysicalKey::RightAlt, VirtualKey::RightAlt);
	InputMap.emplace(PhysicalKey::Enter, VirtualKey::Enter);
	InputMap.emplace(PhysicalKey::Comma, VirtualKey::Comma);
	InputMap.emplace(PhysicalKey::FullStop, VirtualKey::FullStop);
	InputMap.emplace(PhysicalKey::ForwardSlash, VirtualKey::ForwardSlash);
	InputMap.emplace(PhysicalKey::BackSlash, VirtualKey::BackSlash);
	InputMap.emplace(PhysicalKey::Colon, VirtualKey::Colon);
	InputMap.emplace(PhysicalKey::SemiColon, VirtualKey::SemiColon);
	InputMap.emplace(PhysicalKey::Apostrophe, VirtualKey::Apostrophe);
	InputMap.emplace(PhysicalKey::Backspace, VirtualKey::Backspace);
	InputMap.emplace(PhysicalKey::Equal, VirtualKey::Equal);
	InputMap.emplace(PhysicalKey::Hyphen, VirtualKey::Hyphen);
	InputMap.emplace(PhysicalKey::Tilde, VirtualKey::Tilde);
	InputMap.emplace(PhysicalKey::LeftSquareBracket, VirtualKey::LeftSquareBracket);
	InputMap.emplace(PhysicalKey::RightSquareBracket, VirtualKey::RightSquareBracket);
	InputMap.emplace(PhysicalKey::Insert, VirtualKey::Insert);
	InputMap.emplace(PhysicalKey::Home, VirtualKey::Home);
	InputMap.emplace(PhysicalKey::Delete, VirtualKey::Delete);
	InputMap.emplace(PhysicalKey::PageUp, VirtualKey::PageUp);
	InputMap.emplace(PhysicalKey::End, VirtualKey::End);
	InputMap.emplace(PhysicalKey::PageDown, VirtualKey::PageDown);
	InputMap.emplace(PhysicalKey::NumPad0, VirtualKey::NumPad0);
	InputMap.emplace(PhysicalKey::NumPad1, VirtualKey::NumPad1);
	InputMap.emplace(PhysicalKey::NumPad2, VirtualKey::NumPad2);
	InputMap.emplace(PhysicalKey::NumPad3, VirtualKey::NumPad3);
	InputMap.emplace(PhysicalKey::NumPad4, VirtualKey::NumPad4);
	InputMap.emplace(PhysicalKey::NumPad5, VirtualKey::NumPad5);
	InputMap.emplace(PhysicalKey::NumPad6, VirtualKey::NumPad6);
	InputMap.emplace(PhysicalKey::NumPad7, VirtualKey::NumPad7);
	InputMap.emplace(PhysicalKey::NumPad8, VirtualKey::NumPad8);
	InputMap.emplace(PhysicalKey::NumPad9, VirtualKey::NumPad9);
	InputMap.emplace(PhysicalKey::Key_Last, VirtualKey::Key_Last);
}
