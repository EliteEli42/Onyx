#include <Core/Log/Log.h>
#include <Core/Debug/Debug.h>

using namespace Core;

int main() {
	Log::SetLevel(LogLevel::Trace);

	Log::Trace("This is a trace message.");
	Log::Debug("This is a debug message.");
	Log::Info("This is an info message.");
	Log::Warn("This is a warning message.");
	Log::Error("This is an error message.");
	Log::Fatal("This is a fatal message.");

	DEBUG_BREAK();

	CHECK(false, "This is a check failure.");

	ASSERT(false, "This is an assertion failure.");

	return 0;
}
