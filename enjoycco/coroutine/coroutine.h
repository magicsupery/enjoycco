#pragma once
#include <boost/context/all.hpp>
#include <iostream>
#include "../fcontext/stack_allocator.h"
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

				static void set_current_coroutine(Coroutine* current);

				static Coroutine* this_coroutine();
		};

		enum class Coroutine_S
		{
			INVALID = 0,
			RUNNING,
			YIELD,
			FINISHED,
			CLOSED,
		};

		using fcontext_t = boost::context::detail::fcontext_t;
		using transfer_t = boost::context::detail::transfer_t;
		typedef void (*Function)(void);
		using DefaultAllocator = StackAllocator<8 * 1024 * 1024, 4 * 1024 * 1024, 1 * 1024 * 1024>;
		class Coroutine: public NonCopyable
		{
			private:
				struct JumpData
				{
					Coroutine* from_co;
					Coroutine* to_co;
					void* data;
				};

			public:
				Coroutine(Function&& fn)
					:fn_(fn),
					from_t_(nullptr),
					to_t_(nullptr)
			{
				sp_ = stack_allocator_.allocate(stack_allocator_.default_stacksize());
				to_t_ = boost::context::detail::make_fcontext(sp_, 
						stack_allocator_.default_stacksize(), 
						&Coroutine::wrapper_function);
			}

			public:
				void start()
				{
					state_ = Coroutine_S::RUNNING;
					int a = 0;
					JumpData jump_data{CoroutineContext::this_coroutine(), this, &a};
					jump_to(to_t_, jump_data);
				}

				void resume()
				{
					start();
				}

				void yield()
				{
					std::cout << __FUNCTION__ << " to context is " << from_t_ << std::endl;
					auto back_transfer_t = boost::context::detail::jump_fcontext(from_t_, nullptr);
					set_from_context(back_transfer_t.fctx);
				}

				void run()
				{
					fn_();
				}	

				void set_state(Coroutine_S s)
				{
					state_ = s;
				}

				void set_from_context(fcontext_t t)
				{

					from_t_ = t;
				}


			protected:
				static void wrapper_function(transfer_t t)
				{

					JumpData* jump_data = reinterpret_cast<JumpData*>(t.data);
					auto co_to = jump_data->to_co;
					co_to->set_from_context(t.fctx);

					co_to->run();
					co_to->set_state(Coroutine_S::FINISHED);
					co_to->yield();
				}

				void jump_to(fcontext_t& to, JumpData& jump_data)
				{
					std::cout << __FUNCTION__ << " from_co is " << jump_data.from_co << " to_co is " << jump_data.to_co <<
						" to context is " << to_t_ << std::endl;
					CoroutineContext::set_current_coroutine(jump_data.to_co);
					auto back_transfer_t = boost::context::detail::jump_fcontext(to, &jump_data);
					to_t_ = back_transfer_t.fctx;
					CoroutineContext::set_current_coroutine(jump_data.from_co);

				}

			private:
				Function fn_;
				Coroutine_S state_;

				fcontext_t from_t_;
				fcontext_t to_t_;
				void* data_;

				DefaultAllocator stack_allocator_;
				void* sp_;
		};


		inline void CoroutineContext::this_coroutine_yield()
		{
			if(current_)
				current_->yield();
			else
			{
			}
		}

		inline void CoroutineContext::set_current_coroutine(Coroutine* current)
		{
			current_ = current;
		}

		inline Coroutine* CoroutineContext::this_coroutine()
		{
			return	current_;
		}
	}
}



