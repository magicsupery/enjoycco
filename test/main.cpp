#include <enjoycco/coroutine.h>

#include <iostream>

using namespace std;
using namespace enjoyc::co;

void func2()
{
	ASSERT_IN_COROUTINE;
	cout << "func2 1" << endl;
	CO_YIELD;
	cout << "func2 2" << endl;
}
void func()
{
	cout << "func 1" << endl;
	CO_YIELD;
	cout << "func 2" << endl;

}

void test1()
{
	Coroutine s(func);
	s.start();

	cout << "main done 1" << endl;
	s.start();

	cout << "main done 2" << endl;

	s.start();

	cout << "main done 3" << endl;
}

void test2()
{

}
int main()
{
	test1();
}
