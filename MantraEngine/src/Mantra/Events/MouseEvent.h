#pragma once

#include "Event.h"

namespace Mantra {

class MANTRA_API MouseMovedEvent : public Event
{
public:
    MouseMovedEvent(float x, float y) : mMouseX(x), mMouseY(y) {}

    inline float GetX() const { return mMouseX; }
    inline float GetY() const { return mMouseY; }

    std::string ToString() const override {
        std::stringstream ss;
        ss << "MouseMovedEvent: " << mMouseX << ", " << mMouseY;
        return ss.str();
    }

    static EventType GetStaticType() { return EventType::MouseMoved; }
    virtual EventType GetEventType() const override { return GetStaticType(); }
    virtual const char* GetName() const override { return "MouseMoved"; }

    EVENT_CLASS_CATEGORY(Mouse | Input)
private:
    float mMouseX, mMouseY;
};

class MANTRA_API MouseScrolledEvent : public Event
{
public:
    MouseScrolledEvent(float xOffset, float yOffset) : mXOffset(xOffset), mYOffset(yOffset) {}

    inline float GetXOffset() const { return mXOffset; }
    inline float GetYOffset() const { return mYOffset; }

    std::string ToString() const override {
        std::stringstream ss;
        ss << "MouseScrolledEvent: " << GetXOffset() << ", " << GetYOffset();
        return ss.str();
    }

    static EventType GetStaticType() { return EventType::MouseScrolled; }
    virtual EventType GetEventType() const override { return GetStaticType(); }
    virtual const char* GetName() const override { return "MouseScrolled"; }

    EVENT_CLASS_CATEGORY(Mouse | Input)
private:
    float mXOffset, mYOffset;
};

class MANTRA_API MouseButtonEvent : public Event
{
public:
    inline int GetMouseButton() const { return mButton; }

    EVENT_CLASS_CATEGORY(Mouse | Input)
protected:
    MouseButtonEvent(int button) : mButton(button) {}

    int mButton;
};

class MANTRA_API MouseButtonPressedEvent : public MouseButtonEvent
{
public:
    MouseButtonPressedEvent(int button) : MouseButtonEvent(button) {}

    std::string ToString() const override {
        std::stringstream ss;
        ss << "MouseButtonPressedEvent: " << mButton;
        return ss.str();
    }

    static EventType GetStaticType() { return EventType::MouseButtonPressed; }
    virtual EventType GetEventType() const override { return GetStaticType(); }
    virtual const char* GetName() const override { return "MouseButtonPressed"; }
};

class MANTRA_API MouseButtonReleasedEvent : public MouseButtonEvent
{
public:
    MouseButtonReleasedEvent(int button) : MouseButtonEvent(button) {}

    std::string ToString() const override {
        std::stringstream ss;
        ss << "MouseButtonReleasedEvent: " << mButton;
        return ss.str();
    }

    static EventType GetStaticType() { return EventType::MouseButtonReleased; }
    virtual EventType GetEventType() const override { return GetStaticType(); }
    virtual const char* GetName() const override { return "MouseButtonReleased"; }
};

}  // namespace Mantra