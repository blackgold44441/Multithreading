#define _CRT_SECURE_NO_WARNINGS 
#include <iostream> 
#include <thread> 
#include <vector> 
#include <mutex> 
#include <time.h> 
#include <algorithm> 
#include <Windows.h> 

using namespace std;

//int first_matrix_str, first_matrix_stl, second_matrix_str, second_matrix_stl;

const int first_matrix_str = 10;
const int first_matrix_stl = 10;
const int second_matrix_str = 10;
const int second_matrix_stl = 10;

vector<vector<int>> first_matrix;
vector<vector<int>> second_matrix;
vector<vector<int>> result_matrix;

long long module_product = 1e3 + 15634;

int rand_element()
{
	int l = rand();

	while ((l == 0) || (l % 10 == 0))
	{
		l = rand();
	}

	return l;
}

void thread_function(int str, int stl)
{
	result_matrix[str][stl] = 0;

	for (int i = 0; i < first_matrix_stl; i++)
	{
		result_matrix[str][stl] += first_matrix[str][i] * second_matrix[i][stl];
		result_matrix[str][stl] %= module_product;
	}
}

int main()
{
	setlocale(0, "");
	freopen("output2.txt", "w", stdout);

	SetConsoleOutputCP(1251);
	srand(time(nullptr));

	int cur_counter = 0;

	/*cout << "Число строк первой матрицы: ";
	cin >> first_matrix_str;
	cout << "Число столбцов первой матрицы: ";
	cin >> first_matrix_stl;
	cout << "Число строк второй матрицы: ";
	cin >> second_matrix_str;
	cout << "Число столбцов второй матрицы: ";
	cin >> second_matrix_stl;*/

	first_matrix.resize(first_matrix_str, vector<int>(first_matrix_stl, 0));
	second_matrix.resize(second_matrix_str, vector<int>(second_matrix_stl, 0));
	result_matrix.resize(first_matrix_str, vector<int>(second_matrix_stl, 0));

	for (int i = 0; i < first_matrix_str; i++)
	{
		generate(first_matrix[i].begin(), first_matrix[i].end(), rand_element);
	}

	for (int i = 0; i < second_matrix_str; i++)
	{
		generate(second_matrix[i].begin(), second_matrix[i].end(), rand_element);
	}

	vector<thread> ths;

	int thread_count = 1;

	for (int i = 1; i <= 10; i++)
	{
		ths.resize(thread_count);

		cout << "Число потоков: " << thread_count << " - ";
		chrono::time_point<chrono::high_resolution_clock> start, end;
		start = chrono::high_resolution_clock::now();

		for (int i = 0; i < first_matrix_str; i++)
		{
			for (int j = 0; j < second_matrix_stl; j++)
			{
				if (cur_counter < thread_count)
				{
					ths[cur_counter] = thread(thread_function, i, j);

					cur_counter++;
				}
				else
				{
					cur_counter = 0;

					for (auto &th : ths)
					{
						th.join();
					}

					ths[cur_counter] = thread(thread_function, i, j);

					cur_counter++;
				}
			}
		}

		for (int i = 0; i < cur_counter; i++)
		{
			ths[i].join();
		}

		cout << "Результат подсчитан" << endl;
		for (int i = 0; i < result_matrix.size(); ++i)
			for (int j = 0; j < result_matrix.size(); ++j)
			{
				{
					cout << result_matrix[i][j] << " ";
				}
			}
		cout << endl;
		end = chrono::high_resolution_clock::now();
		long long elapsed_seconds = chrono::duration_cast<chrono::milliseconds>
			(end - start).count();

		cout << elapsed_seconds << " милисекунд" << endl;

		cur_counter = 0;
		thread_count++;
	}

	return 0;
}
