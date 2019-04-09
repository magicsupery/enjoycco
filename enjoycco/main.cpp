#include "coroutine/coroutine.h"

#include <iostream>

using namespace std;
using namespace enjoyc::co;

void func2()
{

	cout << "func2 1" << endl;	

	CoroutineContext::this_coroutine_yield();
	cout << "func2 2" << endl;
}
void func()
{
	cout << "func 1" << endl;	
	
	CoroutineContext::this_coroutine_yield();
	
	Coroutine s(func2);

	s.start();

	cout << "func 2" << endl;

	s.resume();

}

void test1()
{
	Coroutine s(func);
	s.start();
	
	cout << "main done 1" << endl;
	s.resume();

	cout << "main done 2" << endl;
}

void test2()
{
	
}
int main()
{
	test1();	
}
