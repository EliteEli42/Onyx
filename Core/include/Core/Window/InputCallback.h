#pragma once

#include <Common/MathTypes.h>
#include <Window/Keys.h>

class Input;
class Window;

class InputCallback {
public:
	InputCallback(Input& input);
	void BindWindow(Window& window);

	void KeyCallback(Key key, KeyAction action);
	void MouseButtonCallback(MouseButton button, KeyAction action);
	void MouseMoveCallback(IVec2 pos);
	void ScrollCallback(IVec2 scroll);
	void CharCallback(unsigned int codepoint);
	void DropCallback(int count, const char** paths);

	void Update();
	void Reset();
	void Clear();

private:
	Input& mInput;
};