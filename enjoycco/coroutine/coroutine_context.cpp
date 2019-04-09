#include "coroutine_context.h"
#include "coroutine.h"

namespace enjoyc
{
	namespace co
	{
		Coroutine* CoroutineContext::current_ = nullptr;

		void CoroutineContext::this_coroutine_yield()
		{
			if(current_)
				current_->yield();
			else
			{
			}
		}
	}
}
