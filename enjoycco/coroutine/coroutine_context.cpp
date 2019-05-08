#include "coroutine_context.h"
#include "coroutine.h"

namespace enjoyc
{
	namespace co
	{
		thread_local Coroutine* CoroutineContext::current_ = nullptr;

	}
}
