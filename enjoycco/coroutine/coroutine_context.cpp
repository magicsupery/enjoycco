#include "coroutine_context.h"
#include "coroutine.h"

namespace enjoyc
{
	namespace co
	{
		Coroutine* CoroutineContext::current_ = nullptr;

	}
}
