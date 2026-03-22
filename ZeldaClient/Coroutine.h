#pragma once
#include <coroutine>
#include <exception>
#include <optional>
#include <map>
#include <type_traits>
#include <thread>
#include "TimeController.h"

#define CO_WAIT co_await std::suspend_always{}

class EventLoop 
{
public:
	using Clock = std::chrono::steady_clock;
	using TimePoint = Clock::time_point;

	static  void run()
	{
		while (!tasks.empty()) 
		{
			auto now = Clock::now();
			if (tasks.top().time <= now) 
			{
				auto task = tasks.top().task;
				tasks.pop();
				task();
			}
		}
	}

	static void schedule(std::function<void()> task, size_t milliseconds)
	{
		TimePoint time = Clock::now() + std::chrono::milliseconds(milliseconds);
		tasks.push({ time, task });
	}

private:
	struct TimedTask 
	{
		TimePoint time;
		std::function<void()> task;

		bool operator>(const TimedTask& other) const 
		{
			return time > other.time;
		}
	};

	static std::priority_queue<TimedTask, std::vector<TimedTask>, std::greater<>> tasks;
};

struct WaitForSeconds;

struct WaitForSeconds
{
	WaitForSeconds(size_t _duration) : duration_(_duration)
	{}

	bool await_ready() const noexcept
	{
		// 코루틴이 준비가 됐는지 체크
		// true는 await_resume를 호출하고 코루틴을 재개함
		// false는 await_suspend를 호출하고 코루틴을 종료함
		return false;
	}

	// 코루틴을 중단하고 호출자에게 돌아감
	// void await_suspend(std::coroutine_handle<> _handle) const {}

	// 반환 값에 따라 결정됨
	// true 코루틴을 중단하고 호출자에게 돌아감
	// false 코루틴을 재개함
	// bool await_suspend(std::coroutine_handle<> _handle) const { return true; }

	// 반환하는 핸들로 제어권을 넘김
	std::coroutine_handle<> await_suspend(std::coroutine_handle<> _handle) const
	{
		static_cast<void>(_handle);
		return std::noop_coroutine(); // 호출자로 돌아감.
	}

	void await_resume() const noexcept
	{}

private:
	size_t duration_;
};
//struct WaitForSecondsRealtime
//{
//	WaitForSecondsRealtime(std::chrono::milliseconds _duration) : duration_(_duration) 
//	{}
//	WaitForSecondsRealtime(long long _duration) : duration_(std::chrono::milliseconds(_duration))
//	{}
//
//	bool await_ready() const noexcept
//	{
//		return duration_.count() <= 0;
//	}
//	void await_suspend(std::coroutine_handle<> _handle) const
//	{
//		std::thread([_handle, this]()
//			{
//				std::this_thread::sleep_for(duration_);
//				_handle.resume();
//			}
//		).detach();
//	}
//	void await_resume() const noexcept {}
//
//private:
//	std::chrono::milliseconds duration_;
//};

struct ICoroutine
{
    virtual bool done() const = 0;
	virtual std::coroutine_handle<> get_handle() const = 0;
    virtual ~ICoroutine() = default;
};

class CoroutineBase : public ICoroutine
{
public:
    struct promise_type_base
	{
        std::exception_ptr exception_;

		std::suspend_always initial_suspend()
		{ 
			return {}; 
		}
		std::suspend_always final_suspend() noexcept
		{ 
			return {}; 
		}
        void unhandled_exception() 
		{ 
			exception_ = std::current_exception();
		}
    };

private:
};

template <typename = void>
struct Coroutine;

template<>
struct Coroutine<void> : public CoroutineBase
{
	struct promise_type;
	using handle_type = std::coroutine_handle<promise_type>;

	struct promise_type : public CoroutineBase::promise_type_base
	{
		Coroutine<void> get_return_object()
		{
			auto handle = handle_type::from_promise(*this);
			return Coroutine<void>(handle);
		}

		void return_void() {}
	};

	void operator()() const
	{
		if (!handler_.done())
		{
			handler_.resume();
		}

		if (handler_.promise().exception_)
		{
			std::rethrow_exception(handler_.promise().exception_);
		}
	}

	bool done() const override
	{
		return handler_.done();
	}

	std::coroutine_handle<> get_handle() const override
	{
		return handler_;
	}

	~Coroutine() override
	{
		if (handler_) {
			handler_.destroy();
		}
	}
	constexpr Coroutine<void>(handle_type handler) : handler_(handler) {}
	constexpr Coroutine<void>(const Coroutine&) = delete;
	Coroutine<void>& operator=(const Coroutine&) = delete;
	Coroutine(Coroutine&& other) noexcept : handler_(other.handler_)
	{
		other.handler_ = nullptr;
	}
	Coroutine& operator=(Coroutine&& other) noexcept
	{
		if (this != &other)
		{
			if (handler_)
				handler_.destroy();

			handler_ = other.handler_;
			other.handler_ = nullptr;
		}
		return *this;
	}

protected:
	handle_type handler_;
};

template<typename _Ty>
struct Coroutine : public CoroutineBase
{
	struct promise_type;
	using handle_type = std::coroutine_handle<promise_type>;

    struct promise_type : public CoroutineBase::promise_type_base
	{
		std::optional<_Ty> value_;

        Coroutine get_return_object()
		{
			auto handle = handle_type::from_promise(*this);
			return Coroutine(handle);
		}

        template<std::convertible_to<_Ty> From>
        auto yield_value(const From& from) 
        {
            this->value_ = from;
            return std::suspend_always{};
        }

        template<std::convertible_to<_Ty> From>
        auto yield_value(From&& from) noexcept
    	{
            this->value_ = std::forward<From>(from);
            return std::suspend_always{};
        }

		template<std::convertible_to<_Ty> From>
		void return_value(const From& from)
		{
			this->value_ = from;
		}

        template<std::convertible_to<_Ty> From>
        void return_value(From&& from) noexcept
    	{
            this->value_ = std::forward<From>(from);
        }
    };

	void operator()()
	{
		handler_.promise().value_ = std::nullopt;

		if (!handler_.done())
		{
			handler_.resume();
		}

		if (handler_.promise().exception_)
		{
			std::rethrow_exception(handler_.promise().exception_);
		}
	}

	bool done() const override
	{
		return handler_.done();
	}

	std::optional<_Ty> get_value()
	{
		return handler_.promise().value_;
	}

	std::coroutine_handle<> get_handle() const override
	{
		return handler_;
	}

	~Coroutine() override
	{
		if (handler_) {
			handler_.destroy();
		}
	}
	constexpr Coroutine(handle_type handler) : handler_(handler) {}
    Coroutine(const Coroutine&) = delete;
    Coroutine& operator=(const Coroutine&) = delete;
	Coroutine(Coroutine&& other) noexcept : handler_(other.handler_)
	{
		other.handler_ = nullptr;
	}
	Coroutine& operator=(Coroutine&& other) noexcept
	{
		if (this != &other) 
		{
			if (handler_)
				handler_.destroy();

			handler_ = other.handler_;
			other.handler_ = nullptr;
		}
		return *this;
	}

protected:
	handle_type handler_;
};
