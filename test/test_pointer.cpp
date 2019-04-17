#include <enjoycco/coroutine.h>

#include <iostream>

using namespace std;
using namespace enjoyc::co;

Coroutine* resume_co = nullptr;
Coroutine* co = nullptr;

struct GO
{
	void operator()(Function&& f)
	{
		co = new Coroutine(std::move(f));
		if(co->start() == RetCode::ret_already_finished)
		{
			//delete co;
		}
	}
};

void test(Function&& f)
{
		co = new Coroutine(std::move(f));
		if(co->start() == RetCode::ret_already_finished)
		{
			//delete co;
		}
}
void test1()
{
	
	test(
	[](){
		resume_co = CoroutineContext::this_coroutine();
		cout << "resume_co " << resume_co << endl;
		CO_YIELD;
		cout << "co 1 done" << endl;
	});
	cout << " main done 1" << endl;
	
	cout << "resume_co " << resume_co << endl;
	cout << "co " << co << endl;
	resume_co->start();

}

int main()
{
	test1();
}
