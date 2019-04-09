#pragma once
#include "../utils/noncopyable.h"

namespace enjoyc
{
	namespace co
	{
		class Coroutine;
		class CoroutineContext: public NonCopyable
		{
			private:
				static Coroutine* current_;
			public:
				static void this_coroutine_yield();

				inline static void set_current_coroutine(Coroutine* current)
				{
					current_ = current;
				}

				inline static Coroutine* this_coroutine()
				{
					return current_;
				}
		};

		enum class Coroutine_S
		{
			INVALID = 0,
			READY,
			RUNNING,
			FINISHED,
		};

		enum class RetCode
		{
			ret_success = 0,
			ret_already_running = 1,
			ret_already_finished = 2,
		};
	}
}
