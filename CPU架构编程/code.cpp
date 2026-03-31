#include <iostream>
#include <windows.h>
#include <vector>
#include <string>

using namespace std;

//时间测试
template <typename Func, typename... Args>//func是函数类型，args是参数列表
double benchmark(const string& testName, int repeat_times, Func func, Args&&... args) {//repeattime在这里定义
    LARGE_INTEGER freq, head, tail;
    QueryPerformanceFrequency(&freq);//windows.h库来进行高精度计时
    QueryPerformanceCounter(&head);


    for (int r = 0; r < repeat_times; ++r) {
        func(std::forward<Args>(args)...);
    }

    QueryPerformanceCounter(&tail);
    

    double avgTimeMs = (tail.QuadPart - head.QuadPart) * 1000.0 / freq.QuadPart / repeat_times;
    
    cout << testName << " | 循环: " << repeat_times 
         << " 次 | 单次均耗时: " << avgTimeMs << " ms" << endl;//输出反馈测试结果
         
    return avgTimeMs;
}
//第一题
//平凡算法
void Trivial(const vector<vector<int>>& A, const vector<int>& x, vector<int>& y, int n) {
    for (int j = 0; j < n; ++j) {
        y[j] = 0; 
        for (int i = 0; i < n; ++i) {
            y[j] += A[i][j] * x[i]; 
        }
    }
}

//Cache优化
void CacheOptimized(const vector<vector<int>>& A, const vector<int>& x, vector<int>& y, int n) {
    for (int j = 0; j < n; ++j) {
        y[j] = 0;
    }
    
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            y[j] += A[i][j] * x[i]; 
        }
    }
}
//第二题
//平凡算法
long long sum_Trivial(const vector<int>& a, int n) {
    long long sum = 0;
    for (int i = 0; i < n; ++i) sum += a[i];
    return sum;
}

//多路求和
long long sum_Optimized(const vector<int>& a, int n) {
    long long sum1 = 0, sum2 = 0, sum3 = 0, sum4 = 0;
    int i = 0;
    for (; i <= n - 4; i += 4) {
        sum1 += a[i]; 
        sum2 += a[i + 1]; 
        sum3 += a[i + 2]; 
        sum4 += a[i + 3];//sum1/2/3/4互不影响
    }
    for (; i < n; ++i) { sum1 += a[i]; }
    return sum1 + sum2 + sum3 + sum4;
}




int main()
{
    //测试数据1
    int n_matrix = 2000;//测试规模
    vector<vector<int>> A(n_matrix, vector<int>(n_matrix, 1)); //矩阵
    vector<int> x(n_matrix, 2);//向量
    vector<int> y(n_matrix, 0);//内积
    
    //测试数据2
    int n_array = 1000000;//测试规模
    vector<int> arr(n_array, 1);


    //测试1
    benchmark("矩阵-平凡算法", 10, Trivial, A, x, y, n_matrix);
    benchmark("矩阵-Cache优化", 10, CacheOptimized, A, x, y, n_matrix);
    
    //测试2
    benchmark("数组-平凡算法", 1000, sum_Trivial, arr, n_array);
    benchmark("数组-多路求和", 1000, sum_Optimized, arr, n_array);

    return 0;
}   
