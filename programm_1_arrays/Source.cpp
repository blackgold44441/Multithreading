#define _CRT_SECURE_NO_WARNINGS 
#include <iostream> 
#include <thread> 
#include <vector> 
#include <mutex> 
#include <algorithm> 
#include <time.h> 
#include <chrono> 

using namespace std;

mutex product_lock;

int thread_count = 4;
const int array_size = 1e5;
const long long module_product = 1e6 + 1743;

long long array_product;

vector<int> my_array(array_size, 1);

int rand_element()
{
	int l = rand();

	while ((l == 0) || (l % 10 == 0))
	{
		l = rand();
	}

	return l;
}

void thread_function(int left, int right)
{
	long long local_product = 1;

	for (int i = left; i < right; i++)
	{
		local_product *= my_array[i];
		local_product %= module_product;
	}

	lock_guard<mutex> _output_lock(product_lock);
	array_product *= local_product;
	array_product %= module_product;
}

int main()
{
	setlocale(0, "");
	freopen("output1.txt", "w", stdout);

	srand(time(nullptr));
	generate(my_array.begin(), my_array.end(), rand_element);

	vector<thread> ths;

	int thread_count = 1;

	for (int i = 1; i <= 10; i++)
	{
		ths.resize(thread_count);

		int cur_counter = 0;
		int cur_left = 0;
		int cur_right = 0;
		int block_size = array_size / thread_count;

		array_product = 1;

		cout << "Число потоков: " << thread_count << " - ";
		chrono::time_point<chrono::high_resolution_clock> start, end;
		start = chrono::high_resolution_clock::now();

		for (auto &th : ths)
		{
			cur_left = cur_counter*block_size;
			cur_right = cur_left + block_size;
			th = thread(thread_function, cur_left, cur_right);
			cur_counter++;
		}

		for (auto &th : ths)
		{
			th.join();
		}

		cout << "Результат: " << array_product << endl;

		end = chrono::high_resolution_clock::now();
		long long elapsed_seconds = chrono::duration_cast<chrono::milliseconds>
			(end - start).count();

		cout << elapsed_seconds << " милисекунд" << endl;

		thread_count++;
	}

	return 0;
}