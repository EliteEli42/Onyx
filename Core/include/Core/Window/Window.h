#pragma once
#include "Keys.h"
#include <atomic>
#include <functional>

#include <Core/OpenGL/Image.h>

enum class WindowMode : uint8_t
{
	SystemDecorated,   // Standard OS window (titlebar + borders + DWM behavior)
	SystemNoTitleBar,  // No titlebar, but resizable OS border still present
	FullyUndecorated   // No OS decorations at all (borderless popup-style window)
};

class Window;

struct WindowDesc
{
	const char* title = "Window";
	int width = 1280;
	int height = 720;
	WindowMode mode = WindowMode::SystemDecorated;
	bool centered = true;
	bool transparent = false;
	bool darkTitleBar = false;
	bool resizeable = true;
	IVec4 WindowSizeLimits = { -1,-1,-1,-1 };
	Window* sharedContext = nullptr;
};

struct GLFWwindow;

class Window {
public:
	Window() = default;
	~Window();

	bool Create(const WindowDesc& desc);
	void Destroy();

	bool IsCreated() { return mCreated; }

	static void PollEvents();
	void SwapBuffers();

	void RequestClose() { mShouldClose = true; }
	bool ShouldClose() const;

	void RequestMinimize() { mRequestMinimize = true; }
	void RequestMaximize() { mRequestMaximize = true; }
	void RequestRestore() { mRequestRestore = true; }

	void SetActive(bool active = true);

	void Maximize();
	bool IsMaximized() const;
	void Minimize();
	void Restore();

	void Hide();
	void Show();

	bool SetIcon(Image& image);

	void SetFullscreen(bool enabled);
	bool IsFullscreen() const { return mIsFullscreen; }

	IVec2 GetPosition() const { return mPos; }
	IVec2 GetSize() const { return mSize; }

	void SetPosition(IVec2 pos);
	void SetSize(IVec2 size);

	void CenterWindow();

	Image GetScreenshot();

	void SetVSync(bool enabled);
	bool IsVSync() const { return mVSync; }

	void SetTitle(const char* title);
	void SetDarkTitleBar(bool enabled);

	void SetTargetFPS(double fps);
	double GetTargetFPS() const { return mTargetFPS; }

	double GetDeltaTime() const { return mDeltaTime; }
	double GetCurrentFPS() const { return mCurrentFPS; }
	int GetSmoothFPS() const { return mSmoothedFPS; }

	GLFWwindow* GetHandle() const { return mHandle; }

	void SetUserPointer(void* userPointer) { mUserPointer = userPointer; }
	void* GetUserPointer() { return mUserPointer; }

	IVec2 GetMousePosition();
	void SetMousePosition(IVec2 pos);
	void CenterMouse();

	void SetCursorMode(CursorMode mode);
	CursorMode GetCursorMode() const { return mCursorMode; }

	using SizeCallback = std::function<void(Window&, IVec2)>;
	using PositionCallback = std::function<void(Window&, IVec2)>;
	using RefreshCallback = std::function<void(Window&)>;
	using TitleBarHitTestCallback = std::function<bool(Window&, IVec2)>;
	using KeyCallback = std::function<void(Window&, Key, KeyAction)>;
	using MouseButtonCallback = std::function<void(Window&, MouseButton, KeyAction)>;
	using MouseMoveCallback = std::function<void(Window&, IVec2)>;
	using ScrollCallback = std::function<void(Window&, Vec2)>;
	using CharCallback = std::function<void(Window&, uint32_t)>;
	using DropCallback = std::function<void(Window&, int, const char**)>;

	void SetSizeCallback(SizeCallback callback) { mSizeCallback = std::move(callback); }
	void SetPositionCallback(SizeCallback callback) { mPositionCallback = std::move(callback); }
	void SetRefreshCallback(RefreshCallback callback) { mRefreshCallback = std::move(callback); }
	void SetTitleBarHitTestCallback(TitleBarHitTestCallback callback) { mTitleBarHitTestCallback = std::move(callback); }
	void SetKeyCallback(KeyCallback callback) { mKeyCallback = std::move(callback); }
	void SetMouseButtonCallback(MouseButtonCallback callback) { mMouseButtonCallback = std::move(callback); }
	void SetMouseMoveCallback(MouseMoveCallback callback) { mMouseMoveCallback = std::move(callback); }
	void SetScrollCallback(ScrollCallback callback) { mScrollCallback = std::move(callback); }
	void SetCharCallback(CharCallback callback) { mCharCallback = std::move(callback); }
	void SetDropCallback(DropCallback callback) { mDropCallback = std::move(callback); }

private:
	GLFWwindow* mHandle = nullptr;
	void* mUserPointer = nullptr;

	SizeCallback mSizeCallback = nullptr;
	PositionCallback mPositionCallback = nullptr;
	RefreshCallback mRefreshCallback = nullptr;
	TitleBarHitTestCallback mTitleBarHitTestCallback = nullptr;
	KeyCallback mKeyCallback = nullptr;
	MouseButtonCallback mMouseButtonCallback = nullptr;
	MouseMoveCallback mMouseMoveCallback = nullptr;
	ScrollCallback mScrollCallback = nullptr;
	CharCallback mCharCallback = nullptr;
	DropCallback mDropCallback = nullptr;

	double mTargetFPS = 0;
	double mTargetFrameTime = 0;
	double mLastFrameTime = 0.0;

	double mDeltaTime = 0.0;
	double mCurrentFPS = 0.0;
	int mSmoothedFPS = 0;
	int mTitlebarHeight = 30;

	IVec2 mPos = { 0, 0 };
	IVec2 mSize = { 0, 0 };

	IVec2 mWindowedPos = { 100, 100 };
	IVec2 mWindowedSize = { 1280, 720 };

	CursorMode mCursorMode = CursorMode::Normal;

	bool mIsFullscreen = false;
	bool mVSync = true;
	bool mShouldClose = false;
	bool mRequestMinimize = false;
	bool mRequestMaximize = false;
	bool mRequestRestore = false;
	bool mTitleBarHitTestEnabled = false;

	std::atomic<bool> mCreated = false;
	void Watchdog();

	friend class WindowCallback;
};