#include "InputManager.hpp"

template<class... Ts> struct overload : Ts... { using Ts::operator()...; };
template<class... Ts> overload(Ts...)->overload<Ts...>;

input::InputManager::InputManager()
{
}
 
input::InputEvent input::InputManager::ConvertEvent(const GLFWEvent& event)
{
	input::InputEvent input_event;
	std::variant<std::monostate, Position, Size, Scroll, Keyboard, Mouse, File, Scale> data = {};
	std::visit(overload(
		[](const std::monostate& i) {; },
		[](const GLFWEvent::Position& keyboard) {; },
		[](const GLFWEvent::Size& keyboard) {; },
		[](const GLFWEvent::Scroll& keyboard) {; },
		[](const GLFWEvent::Keyboard& keyboard) {;  },
		[](const GLFWEvent::Mouse& keyboard) {; },
		[](const GLFWEvent::File& keyboard) {; },
		[](const GLFWEvent::Scale& keyboard) {; }
		),event.data);
}

void input::InputManager::RecordKeyStates(const InputEvent& event)
{
	if (event.type == InputType::kKeyPressed)
	{
		auto keyboard = std::get<InputEvent::KeyboardEvent>(event.data);
		key_states[static_cast<int>(keyboard.key)] = true;
	}
	else if (event.type == InputType::kKeyReleased)
	{
		auto keyboard = std::get<InputEvent::KeyboardEvent>(event.data);
		key_states[static_cast<int>(keyboard.key)] = false;
	}

}

input::InputManager::~InputManager()
{
}

void input::InputManager::PopulateInputMap()
{
	input_map.emplace(PhysicalKey::A, VirtualKey::A);
	input_map.emplace(PhysicalKey::B, VirtualKey::B);
	input_map.emplace(PhysicalKey::C, VirtualKey::C);
	input_map.emplace(PhysicalKey::D, VirtualKey::D);
	input_map.emplace(PhysicalKey::E, VirtualKey::E);
	input_map.emplace(PhysicalKey::F, VirtualKey::F);
	input_map.emplace(PhysicalKey::G, VirtualKey::G);
	input_map.emplace(PhysicalKey::H, VirtualKey::H);
	input_map.emplace(PhysicalKey::I, VirtualKey::I);
	input_map.emplace(PhysicalKey::J, VirtualKey::J);
	input_map.emplace(PhysicalKey::K, VirtualKey::K);
	input_map.emplace(PhysicalKey::L, VirtualKey::L);
	input_map.emplace(PhysicalKey::M, VirtualKey::M);
	input_map.emplace(PhysicalKey::N, VirtualKey::N);
	input_map.emplace(PhysicalKey::O, VirtualKey::O);
	input_map.emplace(PhysicalKey::P, VirtualKey::P);
	input_map.emplace(PhysicalKey::Q, VirtualKey::Q);
	input_map.emplace(PhysicalKey::R, VirtualKey::R);
	input_map.emplace(PhysicalKey::S, VirtualKey::S);
	input_map.emplace(PhysicalKey::T, VirtualKey::T);
	input_map.emplace(PhysicalKey::U, VirtualKey::U);
	input_map.emplace(PhysicalKey::V, VirtualKey::V);
	input_map.emplace(PhysicalKey::W, VirtualKey::W);
	input_map.emplace(PhysicalKey::X, VirtualKey::X);
	input_map.emplace(PhysicalKey::Y, VirtualKey::Y);
	input_map.emplace(PhysicalKey::Z, VirtualKey::Z);
	input_map.emplace(PhysicalKey::k1, VirtualKey::k1);
	input_map.emplace(PhysicalKey::k2, VirtualKey::k2);
	input_map.emplace(PhysicalKey::k3, VirtualKey::k3);
	input_map.emplace(PhysicalKey::k4, VirtualKey::k4);
	input_map.emplace(PhysicalKey::k5, VirtualKey::k5);
	input_map.emplace(PhysicalKey::k6, VirtualKey::k6);
	input_map.emplace(PhysicalKey::k7, VirtualKey::k7);
	input_map.emplace(PhysicalKey::k8, VirtualKey::k8);
	input_map.emplace(PhysicalKey::k9, VirtualKey::k9);
	input_map.emplace(PhysicalKey::k0, VirtualKey::k0);
	input_map.emplace(PhysicalKey::F1, VirtualKey::F1);
	input_map.emplace(PhysicalKey::F2, VirtualKey::F2);
	input_map.emplace(PhysicalKey::F3, VirtualKey::F3);
	input_map.emplace(PhysicalKey::F4, VirtualKey::F4);
	input_map.emplace(PhysicalKey::F5, VirtualKey::F5);
	input_map.emplace(PhysicalKey::F6, VirtualKey::F6);
	input_map.emplace(PhysicalKey::F7, VirtualKey::F7);
	input_map.emplace(PhysicalKey::F8, VirtualKey::F8);
	input_map.emplace(PhysicalKey::F9, VirtualKey::F9);
	input_map.emplace(PhysicalKey::F10, VirtualKey::F10);
	input_map.emplace(PhysicalKey::F11, VirtualKey::F11);
	input_map.emplace(PhysicalKey::F12, VirtualKey::F12);
	input_map.emplace(PhysicalKey::kTab, VirtualKey::kTab);
	input_map.emplace(PhysicalKey::kLeftShift, VirtualKey::kLeftShift);
	input_map.emplace(PhysicalKey::kLeftControl, VirtualKey::kLeftControl);
	input_map.emplace(PhysicalKey::kLeftAlt, VirtualKey::kLeftAlt);
	input_map.emplace(PhysicalKey::kRightShift, VirtualKey::kRightShift);
	input_map.emplace(PhysicalKey::kRightControl, VirtualKey::kRightControl);
	input_map.emplace(PhysicalKey::kRightAlt, VirtualKey::kRightAlt);
	input_map.emplace(PhysicalKey::kEnter, VirtualKey::kEnter);
	input_map.emplace(PhysicalKey::kComma, VirtualKey::kComma);
	input_map.emplace(PhysicalKey::kFullStop, VirtualKey::kFullStop);
	input_map.emplace(PhysicalKey::kForwardSlash, VirtualKey::kForwardSlash);
	input_map.emplace(PhysicalKey::kBackSlash, VirtualKey::kBackSlash);
	input_map.emplace(PhysicalKey::kColon, VirtualKey::kColon);
	input_map.emplace(PhysicalKey::kSemiColon, VirtualKey::kSemiColon);
	input_map.emplace(PhysicalKey::kApostrophe, VirtualKey::kApostrophe);
	input_map.emplace(PhysicalKey::kBackspace, VirtualKey::kBackspace);
	input_map.emplace(PhysicalKey::kEqual, VirtualKey::kEqual);
	input_map.emplace(PhysicalKey::kHyphen, VirtualKey::kHyphen);
	input_map.emplace(PhysicalKey::kTilde, VirtualKey::kTilde);
	input_map.emplace(PhysicalKey::kLeftSquareBracket, VirtualKey::kLeftSquareBracket);
	input_map.emplace(PhysicalKey::kRightSquareBracket, VirtualKey::kRightSquareBracket);
	input_map.emplace(PhysicalKey::kInsert, VirtualKey::kInsert);
	input_map.emplace(PhysicalKey::kHome, VirtualKey::kHome);
	input_map.emplace(PhysicalKey::kDelete, VirtualKey::kDelete);
	input_map.emplace(PhysicalKey::kPageUp, VirtualKey::kPageUp);
	input_map.emplace(PhysicalKey::kEnd, VirtualKey::kEnd);
	input_map.emplace(PhysicalKey::kPageDown, VirtualKey::kPageDown);
	input_map.emplace(PhysicalKey::kNumPad0, VirtualKey::kNumPad0);
	input_map.emplace(PhysicalKey::kNumPad1, VirtualKey::kNumPad1);
	input_map.emplace(PhysicalKey::kNumPad2, VirtualKey::kNumPad2);
	input_map.emplace(PhysicalKey::kNumPad3, VirtualKey::kNumPad3);
	input_map.emplace(PhysicalKey::kNumPad4, VirtualKey::kNumPad4);
	input_map.emplace(PhysicalKey::kNumPad5, VirtualKey::kNumPad5);
	input_map.emplace(PhysicalKey::kNumPad6, VirtualKey::kNumPad6);
	input_map.emplace(PhysicalKey::kNumPad7, VirtualKey::kNumPad7);
	input_map.emplace(PhysicalKey::kNumPad8, VirtualKey::kNumPad8);
	input_map.emplace(PhysicalKey::kNumPad9, VirtualKey::kNumPad9);
	input_map.emplace(PhysicalKey::kKeyLast, VirtualKey::kKeyLast);
}
