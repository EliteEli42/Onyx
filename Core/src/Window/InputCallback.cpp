#include <Window/Input.h>
#include <Window/Window.h>
#include <Window/InputCallback.h>

InputCallback::InputCallback(Input& input)
	:mInput(input)
{

}

void InputCallback::BindWindow(Window& window)
{
	window.SetKeyCallback(
		[this](Window& window, Key key, KeyAction action) {
			KeyCallback(key, action);
		}
	);
	window.SetMouseButtonCallback(
		[this](Window& window, MouseButton button, KeyAction action) {
			MouseButtonCallback(button, action);
		}
	);
	window.SetMouseMoveCallback(
		[this](Window& window, IVec2 pos) {
			MouseMoveCallback(pos);
		}
	);
	window.SetScrollCallback(
		[this](Window& window, IVec2 scroll) {
			ScrollCallback(scroll);
		}
	);
	window.SetCharCallback(
		[this](Window& window, unsigned int codepoint) {
			CharCallback(codepoint);
		}
	);
	window.SetDropCallback(
		[this](Window& window, int count, const char** paths) {
			DropCallback(count, paths);
		}
	);
}

void InputCallback::KeyCallback(Key key, KeyAction action)
{
	if (action == KeyAction::Press || action == KeyAction::Repeat) {
		mInput.mCurrKeys[static_cast<size_t>(key)] = true;
	}
	else if (action == KeyAction::Release) {
		mInput.mCurrKeys[static_cast<size_t>(key)] = false;
	}
}

void InputCallback::MouseButtonCallback(MouseButton button, KeyAction action)
{
	if (action == KeyAction::Press) {
		mInput.mCurrMouseButtons[static_cast<size_t>(button)] = true;
	}
	else if (action == KeyAction::Release) {
		mInput.mCurrMouseButtons[static_cast<size_t>(button)] = false;
	}
}

void InputCallback::MouseMoveCallback(IVec2 pos)
{
	mInput.mCurrMousePos = pos;
}

void InputCallback::ScrollCallback(IVec2 scroll)
{
	mInput.mScrollDelta += scroll;
}

void InputCallback::CharCallback(unsigned int codepoint)
{
	mInput.mCharQueue.push(codepoint);
}

void InputCallback::DropCallback(int count, const char** paths)
{
	for (int i = 0; i < count; ++i) {
		mInput.mDroppedFiles.push(paths[i]);
	}
}

void InputCallback::Reset()
{
	// Keys
	mInput.mCurrKeys.fill(false);
	mInput.mPrevKeys.fill(false);

	// Mouse Buttons
	mInput.mCurrMouseButtons.fill(false);
	mInput.mPrevMouseButtons.fill(false);
}

void InputCallback::Clear()
{
	Reset();

	// Mouse / Scroll
	mInput.mCurrMousePos = { 0, 0 };
	mInput.mPrevMousePos = { 0, 0 };
	mInput.mMouseDelta = { 0, 0 };
	mInput.mScrollDelta = { 0, 0 };

	// Queues
	while (!mInput.mCharQueue.empty()) mInput.mCharQueue.pop();

	while (!mInput.mDroppedFiles.empty()) mInput.mDroppedFiles.pop();
}

void InputCallback::Update() {
	mInput.mPrevKeys = mInput.mCurrKeys;
	mInput.mPrevMouseButtons = mInput.mCurrMouseButtons;
	mInput.mScrollDelta = { 0, 0 };
	mInput.mMouseDelta = mInput.mCurrMousePos - mInput.mPrevMousePos;
	mInput.mPrevMousePos = mInput.mCurrMousePos;
}