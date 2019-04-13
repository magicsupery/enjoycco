#pragma once
#include <assert.h>
#include "coroutine/coroutine.h"

#define GO(FUNCTION)  do{\
	::enjoyc::co::Coroutine(FUNCTION).start(); \
}while(0)

#define CO_YIELD do{\
	::enjoyc::co::CoroutineContext::this_coroutine()->yield(); \
}while(0)

#define ASSERT_IN_COROUTINE do{\
	assert(::enjoyc::co::CoroutineContext::in_coroutine_context()); \
}while(0)
