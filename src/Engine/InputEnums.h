#pragma once

namespace Input
{
	enum class InputType
	{
		  None
		, WindowMoved
		, WindowResized
		, WindowClosed
		, WindowRefresh
		, WindowDefocused
		, WindowIconified
		, WindowUnIconified
		, FramebufferResized
		, ButtonPressed
		, ButtonReleased
		, CursorMoved
		, CursorEnteredWindow
		, CursorLeftWindow
		, MouseScrolled
		, KeyPressed
		, KeyReleased
		, KeyRepeated
		, MonitorConnected
		, MonitorDisconnected
		, FileDropped
		, WindowMaximised
		, WindowUnMaximised
		, WindowScaleChanged
	};

	enum class MouseButton
	{
		  None
		, Left
		, Right
		, Middle
		, Mouse4
		, Mouse5
	};

	enum class VirtualKey
	{
		None,
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
		Key_1,
		Key_2,
		Key_3,
		Key_4,
		Key_5,
		Key_6,
		Key_7,
		Key_8,
		Key_9,
		Key_0,
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
		Tab,
		LeftShift,
		LeftControl,
		LeftAlt,
		RightShift,
		RightControl,
		RightAlt,
		Enter,
		Comma,
		FullStop,
		ForwardSlash,
		BackSlash,
		Colon,
		SemiColon,
		Apostrophe,
		Backspace,
		Equal,
		Hyphen,
		Tilde,
		LeftSquareBracket,
		RightSquareBracket,
		Insert,
		Home,
		Delete,
		PageUp,
		End,
		PageDown,
		NumPad0,
		NumPad1,
		NumPad2,
		NumPad3,
		NumPad4,
		NumPad5,
		NumPad6,
		NumPad7,
		NumPad8,
		NumPad9,
		Key_Last
	};
}