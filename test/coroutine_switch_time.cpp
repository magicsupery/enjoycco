#include <enjoycco/coroutine.h>

#include <chrono>
#include <iostream>

using namespace std;
using namespace std::chrono;

constexpr int CO_N = 5000000;
int main()
{
	auto start = high_resolution_clock::now();
	
	for(size_t i = 0; i < CO_N; i++)
	{
		enjoyc::co::Coroutine s([](){});
		s.start();
	}
	
	auto end = high_resolution_clock::now();

	cout << "total time is " << duration_cast<milliseconds>(end - start).count() << endl;;
}
