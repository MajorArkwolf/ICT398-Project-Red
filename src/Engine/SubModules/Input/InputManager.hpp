#pragma once
#include <array>
#include <map>
#include <variant>

#include "GLFWInputWrangler.hpp"
#include "InputEnums.hpp"

namespace input {
using PhysicalKey = VirtualKey;

//An event used within the engine to log inputs
struct InputEvent {
    struct MouseEvent {
        MouseButton button = MouseButton::kNone;
        PhysicalKey mods = PhysicalKey::kNone;
    };

    struct KeyboardEvent {
        PhysicalKey key = PhysicalKey::kKeyLast;
        PhysicalKey mods = PhysicalKey::kKeyLast;
    };

    struct iVector2 {
        int x = 0;
        int y = 0;
    };

    struct dVector2 {
        double x = 0;
        double y = 0;
    };

    InputType type = InputType::kNone;
    using Data = std::variant<std::monostate, MouseEvent, KeyboardEvent, iVector2, dVector2>;
    Data data;
};

/**
	*@class InputManager
	* @brief Class used to read and handle inputs in the engine
	*/
class InputManager {
   public:
    /**
		* Default constructor
		*/
    InputManager();
    InputManager(const InputManager& rhs) = delete;
    InputManager(const InputManager&& rhs) = delete;
    InputManager& operator=(InputManager& rhs) = delete;
    InputManager& operator=(InputManager&& rhs) = delete;
    /**
		* Converts a glfw event into an InputEvent
		* @param event The Glfw event to convert
		* @return the InputEvent that the glfw event was converted into
		*/
    InputEvent ConvertEvent(const GLFWEvent& event);
    /**
		* Recocords the current state of whether keys are pressed or released i nan array
		* @param event The input event to update the array with
		*/
    void RecordKeyStates(const InputEvent& event);
    /**
		* Converts a GLFW key enum into a RedEngine physical key enum
        * @param key The glfw key enum to convert
        * @return The Physical key that was converted
		*/
    PhysicalKey ConvertGLFWKey(int key);

    /**
		* Converts a  RedEngine key into a string
        * @param key The key to convert to string
        * @return The string representation of the key
		*/
    std::string& HashKeyToString(VirtualKey key);

    /**
		* Returns the input map that maps the Physical and Virtual keys together
		*/
    std::map<PhysicalKey, VirtualKey>& GetInputMap();
    /**
		* Returns the array of keystates the logs which keys are currently pressed
		*/
    std::array<bool, static_cast<int>(VirtualKey::kKeyLast)>& GetKeyStates();

    /**
		* Gets the map that maps a key to a string
		*/
    std::map<PhysicalKey, std::string>& GetStringMap();
    ~InputManager();

   private:
    /**
		* Converts a physical key to a virtual key based on the conversion map
        * @param key The physical key to convert
        * @return The virtual key mapped to the physical key
		*/
    VirtualKey ConvertPhysicalToVirtual(PhysicalKey key);

    /**
		* Creates the mapping between virtual and physical keys
		*/
    void PopulateInputMap();
    /**
		* Creates the mapping between physical keys and GLFW keys
		*/
    void PopulateGLFWKeyMap();
    /**
		* Creates the mapping between physical keys and strings
		*/
    void PopulateStringMap();

    //The map containing the relations between physical and virtual keys
    std::map<PhysicalKey, VirtualKey> input_map;
    // The map containing the relations between glfw keys and physical keys
    std::map<int, PhysicalKey> glfw_key_map;
    // The map containing the relation between keys and strings
    std::map<PhysicalKey, std::string> key_string_map;

    //The array containing the state of keys
    std::array<bool, static_cast<int>(VirtualKey::kKeyLast)> key_states = {false};
};

}// namespace input
