#pragma once

#include "Event.h"

namespace Mantra {

class MANTRA_API WindowResizeEvent : public Event
{
public:
    WindowResizeEvent(unsigned int width, unsigned int height) : mWidth(width), mHeight(height) {}

    inline unsigned int GetWidth() const { return mWidth; }
    inline unsigned int GetHeight() const { return mHeight; }

    std::string ToString() const override {
        std::stringstream ss;
        ss << "WindowResizeEvent: " << mWidth << ", " << mHeight;
        return ss.str();
    }

    static EventType GetStaticType() { return EventType::WindowResize; }
    virtual EventType GetEventType() const override { return GetStaticType(); }
    virtual const char* GetName() const override { return "WindowResize"; }

    EVENT_CLASS_CATEGORY(Application)
private:
    unsigned int mWidth, mHeight;
};  // namespace Mantra

class MANTRA_API WindowCloseEvent : public Event
{
public:
    WindowCloseEvent() {}

    static EventType GetStaticType() { return EventType::WindowClose; }
    virtual EventType GetEventType() const override { return GetStaticType(); }
    virtual const char* GetName() const override { return "WindowClose"; }
    EVENT_CLASS_CATEGORY(Application)
};  // namespace Mantra

class MANTRA_API AppTickEvent : public Event
{
public:
    AppTickEvent() {}

    static EventType GetStaticType() { return EventType::AppTick; }
    virtual EventType GetEventType() const override { return GetStaticType(); }
    virtual const char* GetName() const override { return "AppTick"; }

    EVENT_CLASS_CATEGORY(Application)
};

class MANTRA_API AppUpdateEvent : public Event
{
public:
    AppUpdateEvent() {}

    static EventType GetStaticType() { return EventType::AppUpdate; }
    virtual EventType GetEventType() const override { return GetStaticType(); }
    virtual const char* GetName() const override { return "AppUpdate"; }

    EVENT_CLASS_CATEGORY(Application)
};

class MANTRA_API AppRenderEvent : public Event
{
public:
    AppRenderEvent() {}

    static EventType GetStaticType() { return EventType::AppRender; }
    virtual EventType GetEventType() const override { return GetStaticType(); }
    virtual const char* GetName() const override { return "AppRender"; }

    EVENT_CLASS_CATEGORY(Application)
};
}  // namespace Mantra