#pragma once

template<typename F>
class ScopeGuard
{
public:
    explicit ScopeGuard(F f) : mFunc(std::move(f)) {}
    ~ScopeGuard() { if (mActive) mFunc(); }
    void Dismiss() { mActive = false; }

    ScopeGuard(const ScopeGuard&) = delete;
    ScopeGuard& operator=(const ScopeGuard&) = delete;

private:
    F mFunc;
    bool mActive = true;
};