#pragma once

#include "Event.h"

namespace Mantra {

class MANTRA_API KeyEvent : public Event
{
public:
    inline int GetKeyCode() const { return mKeyCode; }

    EVENT_CLASS_CATEGORY(Keyboard | Input)
protected:
    KeyEvent(int keycode) : mKeyCode(keycode) {}

    int mKeyCode;
};

class MANTRA_API KeyPressedEvent : public KeyEvent
{
public:
    KeyPressedEvent(int keycode, int repeatCount) : KeyEvent(keycode), mRepeatCount(repeatCount) {}

    inline int GetRepeatCount() const { return mRepeatCount; }

    std::string ToString() const override {
        std::stringstream ss;
        ss << "KeyPressedEvent: " << mKeyCode << " (" << mRepeatCount << " repeats)";
        return ss.str();
    }

    static EventType GetStaticType() { return EventType::KeyPressed; }
    virtual EventType GetEventType() const override { return GetStaticType(); }
    virtual const char* GetName() const override { return "KeyPressed"; }

private:
    int mRepeatCount;
};

class MANTRA_API KeyReleasedEvent : public KeyEvent
{
public:
    KeyReleasedEvent(int keycode) : KeyEvent(keycode) {}

    std::string ToString() const override {
        std::stringstream ss;
        ss << "KeyReleasedEvent: " << mKeyCode;
        return ss.str();
    }

    static EventType GetStaticType() { return EventType::KeyReleased; }
    virtual EventType GetEventType() const override { return GetStaticType(); }
    virtual const char* GetName() const override { return "KeyReleased"; }
};
}  // namespace Mantra