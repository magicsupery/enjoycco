#pragma once
#include <boost/context/all.hpp>
#include <iostream>
#include "../fcontext/stack_allocator.h"
#include "../utils/noncopyable.h"
namespace enjoyc
{
	namespace co
	{
		
		class Coroutinue;
		struct CoroutinueContext: public NonCopyable
		{
			static Coroutinue* current_;
		};

		Coroutinue* CoroutinueContext::current_ = nullptr;
		enum class Coroutinue_S
		{
			INVALID = 0,
			RUNNING,
			YIELD,
			FINISHED,
			CLOSED,
		};
		
		using fcontext_t = boost::context::detail::fcontext_t;
		using transfer_t = boost::context::detail::transfer_t;
		//using Function = std::function<void(transfer_t)>;
		typedef void (*Function)(void);
		using DefaultAllocator = StackAllocator<8 * 1024 * 1024, 4 * 1024 * 1024, 1 * 1024 * 1024>;
		class Coroutinue: public NonCopyable
		{
			struct JumpData
			{
				Coroutinue* from_co;
				Coroutinue* to_co;
				void* data;
			};

			public:
				Coroutinue(Function&& fn)
					:fn_(fn),
					from_t_(nullptr),
					to_t_(nullptr)
				{
					sp_ = stack_allocator_.allocate(stack_allocator_.default_stacksize());
					to_t_ = boost::context::detail::make_fcontext(sp_, 
							stack_allocator_.default_stacksize(), 
							&Coroutinue::wrapper_function);
				}
			
			public:
				void start()
				{
					state_ = Coroutinue_S::RUNNING;
					int a = 0;
					JumpData jump_data{CoroutinueContext::current_, this, &a};
					jump_to(to_t_, jump_data);
				}
	
				void resume()
				{
					start();
				}
	
				void yield()
				{
					std::cout << __FUNCTION__ << " from context is " << from_t_ << std::endl;
					int i = 0;
					
					auto back_transfer_t = boost::context::detail::jump_fcontext(from_t_, &i);
					
					set_from_context(back_transfer_t.fctx);
				}
				
				void run()
				{
					fn_();
				}	
				
				void jump_to(fcontext_t& to, JumpData& jump_data)
				{
					std::cout << __FUNCTION__ << " to context is " << to_t_ << std::endl;
					auto back_transfer_t = boost::context::detail::jump_fcontext(to, &jump_data);
					to_t_ = back_transfer_t.fctx;
					CoroutinueContext::current_ = jump_data.from_co;

					
				}
				void set_state(Coroutinue_S s)
				{
					state_ = s;
				}
				
				void set_from_context(fcontext_t t)
				{
					
					std::cout << __FUNCTION__ << " set from context is " << t << std::endl;
					from_t_ = t;
				}


			protected:
				static void wrapper_function(transfer_t t)
				{
					
					std::cout << __FUNCTION__ << " 0" << std::endl;
					JumpData* jump_data = reinterpret_cast<JumpData*>(t.data);
					auto co_to = jump_data->to_co;
					CoroutinueContext::current_ = co_to;
					co_to->set_from_context(t.fctx);

					std::cout << __FUNCTION__ << " 1" << std::endl;
					co_to->run();

					std::cout << __FUNCTION__ << " 2" << std::endl;
					co_to->set_state(Coroutinue_S::FINISHED);

					co_to->yield();
				}
				
			private:
				Function fn_;
				Coroutinue_S state_;
				
				fcontext_t from_t_;
				fcontext_t to_t_;
				void* data_;
				
				DefaultAllocator stack_allocator_;
				void* sp_;
		};


	}
}
