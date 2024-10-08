#pragma once

#include "ME_PCH.h"
#include "Mantra/Core.h"
#include "spdlog/fmt/ostr.h"

namespace Mantra {

#define EVENT_CLASS_CATEGORY(category)              \
    virtual int GetCategoryFlags() const override { \
        return category;                            \
    }

class MANTRA_API Event
{
    friend class EventDispatcher;

public:
    enum class EventType {
        None = 0,
        WindowClose,
        WindowResize,
        WindowFocus,
        WindowLostFocus,
        WindowMoved,
        AppTick,
        AppUpdate,
        AppRender,
        KeyPressed,
        KeyTyped,
        KeyReleased,
        MouseButtonPressed,
        MouseButtonReleased,
        MouseMoved,
        MouseScrolled
    };

    enum EventCategory {
        None = 0,
        Application = BIT(0),
        Input = BIT(1),
        Keyboard = BIT(2),
        Mouse = BIT(3),
        MouseButton = BIT(4)
    };

    virtual EventType GetEventType() const = 0;
    virtual const char* GetName() const = 0;
    virtual int GetCategoryFlags() const = 0;
    virtual std::string ToString() const { return GetName(); }

    inline bool IsInCategory(EventCategory category) { return GetCategoryFlags() & category; }

    bool handled = false;
};

class EventDispatcher
{
    template <typename T>
    using EventFn = std::function<bool(T&)>;

public:
    EventDispatcher(Event& event) : mEvent(event) {}

    template <typename T>
    bool Dispatch(EventFn<T> func) {
        if (mEvent.GetEventType() == T::GetStaticType()) {
            mEvent.handled = func(*(T*)&mEvent);
            return true;
        }
        return false;
    }

private:
    Event& mEvent;
};

}  // namespace Mantra

inline std::ostream& operator<<(std::ostream& os, const Mantra::Event& e) {
    return os << e.ToString();
}
template <>
struct fmt::formatter<Mantra::Event> : fmt::ostream_formatter {};