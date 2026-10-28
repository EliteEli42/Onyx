#pragma once

enum class KeyAction {
	Release,
	Press,
	Repeat
};

enum class Key {
	// Printable keys
	Space,
	Apostrophe,
	Comma,
	Minus,
	Period,
	Slash,
	Num0, Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9,
	Semicolon,
	Equal,
	A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
	LeftBracket,
	Backslash,
	RightBracket,
	GraveAccent,
	World1,
	World2,

	// Function keys
	Escape,
	Enter,
	Tab,
	Backspace,
	Insert,
	Delete,
	Right, Left, Down, Up,
	PageUp, PageDown,
	Home, End,
	CapsLock,
	ScrollLock,
	NumLock,
	PrintScreen,
	Pause,
	F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15, F16, F17, F18, F19, F20, F21, F22, F23, F24,

	// Keypad keys
	KP0, KP1, KP2, KP3, KP4, KP5, KP6, KP7, KP8, KP9,
	KPDecimal,
	KPDivide,
	KPMultiply,
	KPSubtract,
	KPAdd,
	KPEnter,
	KPEqual,

	LeftShift, LeftControl, LeftAlt, LeftSuper,
	RightShift, RightControl, RightAlt, RightSuper,
	Menu,
	
	Unknown,

	Count
};

enum class MouseButton {
	Left,
	Right,
	Middle,
	Button4,
	Button5,
	Button6,
	Button7,
	Button8,

	Unknown,

	Count
};

constexpr const char* KeyNames[static_cast<size_t>(Key::Count)]	 = {
	// Printable
	"Space",
	"Apostrophe",
	"Comma",
	"Minus",
	"Period",
	"Slash",

	"Num0", "Num1", "Num2", "Num3", "Num4",
	"Num5", "Num6", "Num7", "Num8", "Num9",

	"Semicolon",
	"Equal",

	"A", "B", "C", "D", "E", "F", "G", "H", "I", "J",
	"K", "L", "M", "N", "O", "P", "Q", "R", "S", "T",
	"U", "V", "W", "X", "Y", "Z",

	"LeftBracket",
	"Backslash",
	"RightBracket",
	"GraveAccent",
	"World1",
	"World2",

	// Function keys
	"Escape",
	"Enter",
	"Tab",
	"Backspace",
	"Insert",
	"Delete",

	"Right", "Left", "Down", "Up",
	"PageUp", "PageDown",
	"Home", "End",

	"CapsLock",
	"ScrollLock",
	"NumLock",
	"PrintScreen",
	"Pause",

	"F1","F2","F3","F4","F5","F6","F7","F8","F9","F10",
	"F11","F12","F13","F14","F15","F16","F17","F18","F19","F20",
	"F21","F22","F23","F24",

	// Keypad
	"KP0","KP1","KP2","KP3","KP4",
	"KP5","KP6","KP7","KP8","KP9",

	"KPDecimal",
	"KPDivide",
	"KPMultiply",
	"KPSubtract",
	"KPAdd",
	"KPEnter",
	"KPEqual",

	// Modifiers
	"LeftShift",
	"LeftControl",
	"LeftAlt",
	"LeftSuper",

	"RightShift",
	"RightControl",
	"RightAlt",
	"RightSuper",

	"Menu",

	// special
	"Unknown",
};

constexpr const char* KeyToString(Key key) {
	auto idx = static_cast<size_t>(key);
	if (idx < static_cast<size_t>(Key::Count))
		return KeyNames[idx];
	return "Invalid";
}

constexpr const char* MouseNames[static_cast<size_t>(MouseButton::Count)] = {
	"Left",
	"Right",
	"Middle",
	"Button4",
	"Button5",
	"Button6",
	"Button7",
	"Button8",

	"Unknown",
};

constexpr const char* MouseToString(MouseButton button) {
	auto idx = static_cast<size_t>(button);
	if (idx < static_cast<size_t>(MouseButton::Count))
		return MouseNames[idx];
	return "Invalid";
}

enum class CursorMode {
	Normal,
	Hidden,
	Locked
};