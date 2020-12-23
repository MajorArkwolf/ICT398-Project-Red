#pragma once

namespace input
{
	//List of input types
	enum class InputType
	{
		  kNone
		, kWindowMoved
		, kWindowResized
		, kWindowClosed
		, kWindowRefresh
		, kWindowDefocused
		, kWindowIconified
		, kWindowUnIconified
		, kFramebufferResized
		, kButtonPressed
		, kButtonReleased
		, kCursorMoved
		, kCursorEnteredWindow
		, kCursorLeftWindow
		, kMouseScrolled
		, kKeyPressed
		, kKeyReleased
		, kKeyRepeated
		, kMonitorConnected
		, kMonitorDisconnected
		, kFileDropped
		, kWindowMaximised
		, kWindowUnMaximised
		, kWindowScaleChanged
	};

	//List of mouse buttons
	enum class MouseButton
	{
		  kNone
		, kLeft
		, kRight
		, kMiddle
		, kMouse4
		, kMouse5
	};

	//List of keys
	enum class VirtualKey
	{
		kNone,
		A,
		B,
		C,
		D,
		E,
		F,
		G,
		H,
		I,
		J,
		K,
		L,
		M,
		N,
		O,
		P,
		Q,
		R,
		S,
		T,
		U,
		V,
		W,
		X,
		Y,
		Z,
		k1,
		k2,
		k3,
		k4,
		k5,
		k6,
		k7,
		k8,
		k9,
		k0,
		F1,
		F2,
		F3,
		F4,
		F5,
		F6,
		F7,
		F8,
		F9,
		F10,
		F11,
		F12,
		kTab,
		kEscape,
		kLeftShift,
		kLeftControl,
		kLeftAlt,
		kRightShift,
		kRightControl,
		kRightAlt,
		kEnter,
		kComma,
		kFullStop,
		kForwardSlash,
		kBackSlash,
		kColon,
		kSemiColon,
		kApostrophe,
		kBackspace,
		kSpacebar,
		kEqual,
		kHyphen,
		kTilde,
		kLeftSquareBracket,
		kRightSquareBracket,
		kInsert,
		kHome,
		kDelete,
		kPageUp,
		kEnd,
		kPageDown,
		kNumPad0,
		kNumPad1,
		kNumPad2,
		kNumPad3,
		kNumPad4,
		kNumPad5,
		kNumPad6,
		kNumPad7,
		kNumPad8,
		kNumPad9,
		kKeyLast
	};
}
