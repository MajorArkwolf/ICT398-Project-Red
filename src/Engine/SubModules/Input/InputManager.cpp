#include "InputManager.hpp"

#include <variant>

template<class... Ts>
struct overload : Ts... {
  using Ts::operator()...;
};
template<class... Ts>
overload(Ts...)->overload<Ts...>;

input::InputManager::InputManager() {
  PopulateGLFWKeyMap();
  PopulateInputMap();
}

input::InputEvent input::InputManager::ConvertEvent(const GLFWEvent &event) {

  input::InputEvent input_event = {};

  std::visit(overload{
                 [&](std::monostate) {
                   switch (event.type) {
                     case GLFWEventType::kWindowClosed: {
                       input_event.type = InputType::kWindowClosed;
                     } break;
                     case GLFWEventType::kWindowResized: {
                       input_event.type = InputType::kWindowResized;
                     } break;
                   }
                 },
                 [&](input::GLFWEvent::Position position) {
                   InputEvent::iVector2 pos = {position.x, position.y};
                   switch (event.type) {
                     case GLFWEventType::kCursorMoved: {
                       input_event.type = InputType::kCursorMoved;
                     } break;
                     case GLFWEventType::kWindowMoved: {
                       input_event.type = InputType::kWindowMoved;
                     } break;
                   }
                   input_event.data.emplace<InputEvent::iVector2>(pos);
                 },
                 [&](input::GLFWEvent::Size) {
                   switch (event.type) {
                     default:
                       break;
                   }
                 },
                 [&](input::GLFWEvent::Scroll scroll) {
                   switch (event.type) {
                     case GLFWEventType::kMouseScrolled: {
                       input_event.type = InputType::kMouseScrolled;
                     } break;
                     default:
                       break;
                   }
                   InputEvent::iVector2 vec = {scroll.x, scroll.y};
                   input_event.data.emplace<InputEvent::iVector2>(vec);
                 },
                 [&](input::GLFWEvent::Keyboard keyboard) {
                   auto key = ConvertPhysicalToVirtual(ConvertGLFWKey(keyboard.key));
                   auto mod = std::invoke([&]() {
                     switch (keyboard.mods) {
                       case GLFW_MOD_ALT:
                         return PhysicalKey::kLeftAlt;
                       case GLFW_MOD_CONTROL:
                         return PhysicalKey::kLeftControl;
                       case GLFW_MOD_SHIFT:
                         return PhysicalKey::kLeftShift;
                       default:
                         return PhysicalKey::kNone;
                     }
                   });

                   switch (event.type) {
                     case GLFWEventType::kKeyPressed: {
                       input_event.type = InputType::kKeyPressed;
                     } break;
                     case GLFWEventType::kKeyReleased: {
                       input_event.type = InputType::kKeyReleased;
                     } break;
                   };

                   input::InputEvent::KeyboardEvent keyEvent = {key, mod};
                   input_event.data.emplace<input::InputEvent::KeyboardEvent>(keyEvent);
                 },
                 [&](input::GLFWEvent::Mouse mouse) {
                   auto mod = std::invoke([&]() {
                     switch (mouse.mods) {
                       case GLFW_MOD_ALT:
                         return PhysicalKey::kLeftAlt;
                       case GLFW_MOD_CONTROL:
                         return PhysicalKey::kLeftControl;
                       case GLFW_MOD_SHIFT:
                         return PhysicalKey::kLeftShift;
                       default:
                         return PhysicalKey::kNone;
                     }
                   });

                   auto mouse_event = std::invoke([&]() {
                     switch (mouse.button) {
                       case GLFW_MOUSE_BUTTON_LEFT:
                         return MouseButton::kLeft;
                       case GLFW_MOUSE_BUTTON_RIGHT:
                         return MouseButton::kRight;
                       case GLFW_MOUSE_BUTTON_MIDDLE:
                         return MouseButton::kMiddle;
                       default:
                         return MouseButton::kNone;
                     }
                   });
                   switch (event.type) {
                     case GLFWEventType::kButtonPressed: {
                       input_event.type = InputType::kButtonPressed;
                     } break;
                     case GLFWEventType::kButtonReleased: {
                       input_event.type = InputType::kButtonReleased;
                     } break;
                   };
                   InputEvent::MouseEvent m_event = {mouse_event, mod};
                   input_event.data.emplace<InputEvent::MouseEvent>(m_event);
                 },
                 [&](input::GLFWEvent::File) {
                   switch (event.type) {
                     default:
                       break;
                   }
                 },
                 [&](input::GLFWEvent::Scale) {
                   switch (event.type) {
                     default:
                       break;
                   }
                 }},
             event.data);

  return input_event;
}

void input::InputManager::RecordKeyStates(const InputEvent &event) {
  if (event.type == InputType::kKeyPressed) {
    auto keyboard = std::get<InputEvent::KeyboardEvent>(event.data);
    key_states[static_cast<int>(keyboard.key)] = true;
  } else if (event.type == InputType::kKeyReleased) {
    auto keyboard = std::get<InputEvent::KeyboardEvent>(event.data);
    key_states[static_cast<int>(keyboard.key)] = false;
  }
}

input::PhysicalKey input::InputManager::ConvertGLFWKey(int key) {
  if (glfw_key_map.find(key) != glfw_key_map.end()) {
    return glfw_key_map.at(key);
  }
  return PhysicalKey::kNone;
}

input::InputManager::~InputManager() {
}

input::VirtualKey input::InputManager::ConvertPhysicalToVirtual(PhysicalKey key) {
  if (input_map.find(key) != input_map.end()) {
    return input_map.at(key);
  }
  return PhysicalKey::kNone;
}

void input::InputManager::PopulateInputMap() {
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

void input::InputManager::PopulateGLFWKeyMap() {
  glfw_key_map.emplace(GLFW_KEY_A, VirtualKey::A);
  glfw_key_map.emplace(GLFW_KEY_B, VirtualKey::B);
  glfw_key_map.emplace(GLFW_KEY_C, VirtualKey::C);
  glfw_key_map.emplace(GLFW_KEY_D, VirtualKey::D);
  glfw_key_map.emplace(GLFW_KEY_E, VirtualKey::E);
  glfw_key_map.emplace(GLFW_KEY_F, VirtualKey::F);
  glfw_key_map.emplace(GLFW_KEY_G, VirtualKey::G);
  glfw_key_map.emplace(GLFW_KEY_H, VirtualKey::H);
  glfw_key_map.emplace(GLFW_KEY_I, VirtualKey::I);
  glfw_key_map.emplace(GLFW_KEY_J, VirtualKey::J);
  glfw_key_map.emplace(GLFW_KEY_K, VirtualKey::K);
  glfw_key_map.emplace(GLFW_KEY_L, VirtualKey::L);
  glfw_key_map.emplace(GLFW_KEY_M, VirtualKey::M);
  glfw_key_map.emplace(GLFW_KEY_N, VirtualKey::N);
  glfw_key_map.emplace(GLFW_KEY_O, VirtualKey::O);
  glfw_key_map.emplace(GLFW_KEY_P, VirtualKey::P);
  glfw_key_map.emplace(GLFW_KEY_Q, VirtualKey::Q);
  glfw_key_map.emplace(GLFW_KEY_R, VirtualKey::R);
  glfw_key_map.emplace(GLFW_KEY_S, VirtualKey::S);
  glfw_key_map.emplace(GLFW_KEY_T, VirtualKey::T);
  glfw_key_map.emplace(GLFW_KEY_U, VirtualKey::U);
  glfw_key_map.emplace(GLFW_KEY_V, VirtualKey::V);
  glfw_key_map.emplace(GLFW_KEY_W, VirtualKey::W);
  glfw_key_map.emplace(GLFW_KEY_X, VirtualKey::X);
  glfw_key_map.emplace(GLFW_KEY_Y, VirtualKey::Y);
  glfw_key_map.emplace(GLFW_KEY_Z, VirtualKey::Z);
  glfw_key_map.emplace(GLFW_KEY_1, VirtualKey::k1);
  glfw_key_map.emplace(GLFW_KEY_2, VirtualKey::k2);
  glfw_key_map.emplace(GLFW_KEY_3, VirtualKey::k3);
  glfw_key_map.emplace(GLFW_KEY_4, VirtualKey::k4);
  glfw_key_map.emplace(GLFW_KEY_5, VirtualKey::k5);
  glfw_key_map.emplace(GLFW_KEY_6, VirtualKey::k6);
  glfw_key_map.emplace(GLFW_KEY_7, VirtualKey::k7);
  glfw_key_map.emplace(GLFW_KEY_8, VirtualKey::k8);
  glfw_key_map.emplace(GLFW_KEY_9, VirtualKey::k9);
  glfw_key_map.emplace(GLFW_KEY_0, VirtualKey::k0);
  glfw_key_map.emplace(GLFW_KEY_F1, VirtualKey::F1);
  glfw_key_map.emplace(GLFW_KEY_F2, VirtualKey::F2);
  glfw_key_map.emplace(GLFW_KEY_F3, VirtualKey::F3);
  glfw_key_map.emplace(GLFW_KEY_F4, VirtualKey::F4);
  glfw_key_map.emplace(GLFW_KEY_F5, VirtualKey::F5);
  glfw_key_map.emplace(GLFW_KEY_F6, VirtualKey::F6);
  glfw_key_map.emplace(GLFW_KEY_F7, VirtualKey::F7);
  glfw_key_map.emplace(GLFW_KEY_F8, VirtualKey::F8);
  glfw_key_map.emplace(GLFW_KEY_F9, VirtualKey::F9);
  glfw_key_map.emplace(GLFW_KEY_F10, VirtualKey::F10);
  glfw_key_map.emplace(GLFW_KEY_F11, VirtualKey::F11);
  glfw_key_map.emplace(GLFW_KEY_F12, VirtualKey::F12);
  glfw_key_map.emplace(GLFW_KEY_TAB, VirtualKey::kTab);
  glfw_key_map.emplace(GLFW_KEY_LEFT_SHIFT, VirtualKey::kLeftShift);
  glfw_key_map.emplace(GLFW_KEY_LEFT_CONTROL, VirtualKey::kLeftControl);
  glfw_key_map.emplace(GLFW_KEY_LEFT_ALT, VirtualKey::kLeftAlt);
  glfw_key_map.emplace(GLFW_KEY_RIGHT_SHIFT, VirtualKey::kRightShift);
  glfw_key_map.emplace(GLFW_KEY_RIGHT_CONTROL, VirtualKey::kRightControl);
  glfw_key_map.emplace(GLFW_KEY_RIGHT_ALT, VirtualKey::kRightAlt);
  glfw_key_map.emplace(GLFW_KEY_ENTER, VirtualKey::kEnter);
  glfw_key_map.emplace(GLFW_KEY_COMMA, VirtualKey::kComma);
  glfw_key_map.emplace(GLFW_KEY_PERIOD, VirtualKey::kFullStop);
  glfw_key_map.emplace(GLFW_KEY_SLASH, VirtualKey::kForwardSlash);
  glfw_key_map.emplace(GLFW_KEY_BACKSLASH, VirtualKey::kBackSlash);
  glfw_key_map.emplace(GLFW_KEY_SEMICOLON, VirtualKey::kColon);
  glfw_key_map.emplace(GLFW_KEY_SEMICOLON, VirtualKey::kSemiColon);
  glfw_key_map.emplace(GLFW_KEY_APOSTROPHE, VirtualKey::kApostrophe);
  glfw_key_map.emplace(GLFW_KEY_BACKSPACE, VirtualKey::kBackspace);
  glfw_key_map.emplace(GLFW_KEY_EQUAL, VirtualKey::kEqual);
  glfw_key_map.emplace(GLFW_KEY_MINUS, VirtualKey::kHyphen);
  glfw_key_map.emplace(GLFW_KEY_GRAVE_ACCENT, VirtualKey::kTilde);
  glfw_key_map.emplace(GLFW_KEY_LEFT_BRACKET, VirtualKey::kLeftSquareBracket);
  glfw_key_map.emplace(GLFW_KEY_RIGHT_BRACKET, VirtualKey::kRightSquareBracket);
  glfw_key_map.emplace(GLFW_KEY_INSERT, VirtualKey::kInsert);
  glfw_key_map.emplace(GLFW_KEY_HOME, VirtualKey::kHome);
  glfw_key_map.emplace(GLFW_KEY_DELETE, VirtualKey::kDelete);
  glfw_key_map.emplace(GLFW_KEY_PAGE_UP, VirtualKey::kPageUp);
  glfw_key_map.emplace(GLFW_KEY_END, VirtualKey::kEnd);
  glfw_key_map.emplace(GLFW_KEY_PAGE_DOWN, VirtualKey::kPageDown);
  glfw_key_map.emplace(GLFW_KEY_LAST, VirtualKey::kKeyLast);
}
