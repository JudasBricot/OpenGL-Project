#pragma once

#include <string>
#include <functional>

namespace Judas_Engine
{
	enum class EventType {
		None = 0,
		WindowsClosed, WindowsResized,
		KeyPressed, KeyReleased,
		MouseMoved, MouseScrolled, MouseDragged,
		MouseButtonPressed, MouseButtonReleased,
	};

	enum EventCategory {
		None = 0,
		WindowsEventCategory = 1 << 0,
		KeyboardEventCategory = 1 << 1,
		MouseEventCategory = 1 << 2,
		MouseButtonEventCategory = 1 << 3,
		InputEventCategory = 1 << 4
	};

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	class Event
	{
		friend class EventDispatcher;
	public:
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		inline bool IsInCategory(EventCategory category)
		{
			return GetCategoryFlags() & category;
		}

	protected:
		bool m_Handled = false;
	};

	class EventDispatcher
	{
		template <typename T>
		using EventFunc = std::function<bool(T&)>;
	public:

		EventDispatcher(Event& e)
			: m_Event(e)
		{
		}

		template<typename T>
		bool Dispatch(EventFunc<T> function)
		{
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.m_Handled = function(*(T*)&m_Event);
				return true;
			}

			return false;
		}

	private:
		Event& m_Event;
	};
}