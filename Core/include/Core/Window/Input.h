#pragma once

#include <Core/Window/Keys.h>
#include <Core/Common/MathTypes.h>

#include <string>
#include <queue>
#include <array>

namespace Core {

    class Input {
    public:
        bool IsKeyPressed(Key key) const { return mCurrKeys[(int)key] && !mPrevKeys[(int)key]; }
        bool IsKeyHold(Key key) const { return mCurrKeys[(int)key] && mPrevKeys[(int)key]; }
        bool IsKeyReleased(Key key) const { return !mCurrKeys[(int)key] && mPrevKeys[(int)key]; }

        bool IsMouseButtonPressed(MouseButton button) const { return mCurrMouseButtons[(int)button] && !mPrevMouseButtons[(int)button]; }
        bool IsMouseButtonHold(MouseButton button) const { return mCurrMouseButtons[(int)button] && mPrevMouseButtons[(int)button]; }
        bool IsMouseButtonReleased(MouseButton button) const { return !mCurrMouseButtons[(int)button] && mPrevMouseButtons[(int)button]; }

        IVec2 GetMousePosition() const { return mCurrMousePos; }
        IVec2 GetMouseDelta() const { return mMouseDelta; }

        IVec2 GetScrollDelta() const { return mScrollDelta; }

        unsigned int GetCharPressed() {
            if (mCharQueue.empty()) return 0;
            unsigned int c = mCharQueue.front();
            mCharQueue.pop();
            return c;
        }

        std::string GetCharPressedUTF8() {
            unsigned int cp = GetCharPressed();
            if (cp == 0) return "";
            return CodepointToUTF8(cp);
        }

        char GetCharPressedASCII() {
            unsigned int c = GetCharPressed();
            return (c < 256) ? (char)c : 0;
        }

        bool HasCharPressed() const { return !mCharQueue.empty(); }
        void ClearCharQueue() { mCharQueue = std::queue<unsigned int>(); }

        bool HasDroppedFiles() const { return !mDroppedFiles.empty(); }
        std::string GetNextDroppedFile() {
            if (mDroppedFiles.empty()) return {};
            auto path = std::move(mDroppedFiles.front());
            mDroppedFiles.pop();
            return path;
        }

    private:
        std::array<bool, static_cast<size_t>(Key::Count)> mCurrKeys{ false };
        std::array<bool, static_cast<size_t>(Key::Count)> mPrevKeys{ false };
        std::array<bool, static_cast<size_t>(MouseButton::Count)> mCurrMouseButtons{ false };
        std::array<bool, static_cast<size_t>(MouseButton::Count)> mPrevMouseButtons{ false };

        IVec2 mCurrMousePos = { 0, 0 };
        IVec2 mPrevMousePos = { 0, 0 };
        IVec2 mMouseDelta = { 0, 0 };

        IVec2 mScrollDelta = { 0, 0 };

        std::queue<unsigned int> mCharQueue;
        std::queue<std::string> mDroppedFiles;

        static std::string CodepointToUTF8(unsigned int cp) {
            std::string s;
            if (cp < 0x80) {
                s += (char)cp;
            }
            else if (cp < 0x800) {
                s += (char)(0xC0 | (cp >> 6));
                s += (char)(0x80 | (cp & 0x3F));
            }
            else if (cp < 0x10000) {
                s += (char)(0xE0 | (cp >> 12));
                s += (char)(0x80 | ((cp >> 6) & 0x3F));
                s += (char)(0x80 | (cp & 0x3F));
            }
            else {
                s += (char)(0xF0 | (cp >> 18));
                s += (char)(0x80 | ((cp >> 12) & 0x3F));
                s += (char)(0x80 | ((cp >> 6) & 0x3F));
                s += (char)(0x80 | (cp & 0x3F));
            }
            return s;
        }

        friend class InputCallback;
    };

} // namespace Core