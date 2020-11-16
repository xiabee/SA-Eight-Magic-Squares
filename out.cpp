/*
*  八幻方问题 利用模拟退火算法，寻找八幻方尽可能多的解
*  @ auther:    xiabee
*  @ time:  2020/11/13
*  @ version:   0.3
*/

#include <iostream>
#include <ctime>
#include <fstream>
#include <cmath>
#include <cstdio>
#include <algorithm>
#include <direct.h>
#include <io.h>

const int ORDER = 8;
using namespace std;
int file_num = 0;
// 记录文件总数

int max(int** ms,int current_row) //  矩阵列的最大值
{
    int maxn = 0;
    for (int cnt = 0; cnt < ORDER; cnt++)
        if (ms[cnt][current_row] > maxn)
            maxn = ms[cnt][current_row];
    return maxn;
}

int min(int** ms, int current_row) // 矩阵列的最小值
{
    int minn = 0xffff;
    for (int cnt = 0; cnt < ORDER; cnt++)
        if (ms[cnt][current_row] < minn)
            minn = ms[cnt][current_row];
    return minn;
}

int rand_int()  // 创造[0, ORDER)内的随机数
{
    return rand() % ORDER;
}

int **init(int ORDER)    //初始化一个n阶幻方
{
    //ORDER:阶数; magic_square: 幻方指针
    int **magic_square = new int* [ORDER];//动态申请二维数组nxm

    for (int cnt = 0; cnt < ORDER; cnt++)
        magic_square[cnt] = new int[ORDER];
    // 申请内存

    for (int line = 0; line < ORDER; line++)
        for (int column = 0; column < ORDER; column++)
            magic_square[line][column] = line * ORDER + column + 1;
    // 初始化，按顺序填入
    return magic_square;
}

void shuffle(int** ms) // 乱序
{
    int pos1, pos2;
    for(int cnt_line=0;cnt_line<ORDER;cnt_line++)
        for (int cnt_column = 0; cnt_column < ORDER; cnt_column++)
        {
            pos1 = rand_int();
            pos2 = rand_int();
            // 生成随机点
            swap(ms[cnt_line][cnt_column], ms[pos1][pos2]);
            // 交换当前点与随机点，产生乱序
        }
}

void exc(int** ms, int x1, int y1, int x2, int y2) // 交换两个点的位置
{
    swap(ms[x1][y1], ms[x2][y2]);
}

int sum(int **ms) // 矩阵总和
{
    int total = 0;
    for (int cnt_line = 0; cnt_line < ORDER; cnt_line++)
        for (int cnt_column = 0; cnt_column < ORDER; cnt_column++)
            total += ms[cnt_line][cnt_column];
    return total;
}

int sum_column(int** ms,int column)  //列之和
{
    int total = 0;
    for (int cnt_line = 0; cnt_line < ORDER; cnt_line++)
            total += ms[cnt_line][column];
    return total;
}

int sum_line(int** ms, int line) //行之和
{
    int total = 0;
        for (int cnt_column = 0; cnt_column < ORDER; cnt_column++)
            total += ms[line][cnt_column];
    return total;
}

int sum_diagonal(int** ms, bool x)  // 对角线之和
{
    int sum = 0;
    if (x)
        for (int cnt = 0; cnt < ORDER; cnt++)
            sum += ms[cnt][cnt];
    else
        for (int cnt = 0; cnt < ORDER; cnt++)
            sum += ms[cnt][ORDER - cnt - 1];
    return sum;
}

int delta(int** ms) // 给定矩阵的各行各列标准差的整数倍. 用于判断当前状态是否接近正确解 (当标准差为零时，认为找到幻方)
{
    int ssum = sum(ms) * 2 + sum_diagonal(ms, 0) + sum_diagonal(ms, 1); //各行、各列、双对角线之和
    int ave = ssum / (ORDER * 2 + 2); // 平均值
    int deviation = 0;  //标准差的 N 倍
    for (int cnt = 0; cnt < ORDER; cnt++)
        deviation += abs(sum_line(ms, cnt) - ave) + abs(sum_column(ms, cnt) - ave);
    deviation += abs(sum_diagonal(ms, 0) - ave) + abs(sum_diagonal(ms, 1) - ave);
    return deviation;
}

void output(int** ms) // 打印幻方
{
    cout << "8" << endl;
    for (int cnt_line = 0; cnt_line < ORDER; cnt_line++)
    {
        for (int cnt_column = 0; cnt_column < ORDER; cnt_column++)
        {
            if (ms[cnt_line][cnt_column] < 10)
                cout << "0" << ms[cnt_line][cnt_column] << " ";
            else
                cout << ms[cnt_line][cnt_column] << " ";
        }   // 对齐输出
        cout << endl;
    }
    for (int cnt = 0; cnt < ORDER; cnt++)
    {
        cout << cnt + 1 << " ";
        // 输出行号
        if (min(ms, cnt) < 10)
            cout << "0";
        cout << min(ms, cnt);
        cout << " ";
        // 输出最小值

        if (max(ms, cnt) < 10)
            cout << "0";
        cout << max(ms, cnt) << " 9" << endl;
        // 输出最大值、总行
    }
    cout << endl;
}

void first_sol_8(int **ms) // 找出八阶幻方的第一个解
{
    if (ORDER != 8)
    {
        cout << "Error! Order must be 8!" << endl;
        exit(-1);
    } //只有八阶幻方可以使用这个函数

    // 这里可以直接写数字，但是不知道其他阶数的幻方能不能找到解，就先以变量代替，便于维护
    for (int cnt = 0; cnt < ORDER / 2; cnt++)
    {
        exc(ms, cnt, cnt, ORDER - cnt -1, ORDER - cnt -1);
        exc(ms, cnt, ORDER - cnt -1, ORDER - cnt -1, cnt);
    }// 主对角线交换

    for (int cnt = 0; cnt < ORDER / 2; cnt++)
    {
        exc(ms, cnt, ORDER / 2 - cnt - 1, ORDER - cnt -1, cnt + ORDER / 2);
        exc(ms, ORDER / 2 + cnt, cnt, ORDER / 2 - cnt - 1, ORDER - cnt - 1);
    } // 副对角线交换 (四个4*4矩阵对角线交换)
}

void release(int** ms) // 释放内存空间
{
    for (int cnt = 0; cnt < ORDER; cnt++)
        free(ms[cnt]);
    free(ms);
}

void tofile(int** ms) // 将合法幻方写入.pgm文件
{
    file_num++;
    if (file_num > 99999)
    {
        cout << "Error! Overflow!";
        exit(-1);
    }
        
    char file_name[10];
    sprintf_s(file_name, "%05d%s", file_num, ".pgm"); // 0位补齐
    string pwd = "./Images/";   // 写入文件夹
    if (0 != _access(pwd.c_str(), 0))
    {
        // if this folder not exist, create a new one.
        int s = _mkdir(pwd.c_str());   // 返回 0 表示创建成功，-1 表示失败
    }
    pwd += file_name;

    cout << pwd << endl;
    ofstream outfile(pwd);

    outfile << "P2" << endl;    // 文件头标识符
    outfile << "08 08" << endl; // 图像宽度 高度
    outfile << ORDER * ORDER << endl;   // 灰度的最大值
    
    for (int cnt_line = 0; cnt_line < ORDER; cnt_line++)
    {
        for (int cnt_column = 0; cnt_column < ORDER; cnt_column++)
        {
            if (ms[cnt_line][cnt_column] < 10)
                outfile << "0" << ms[cnt_line][cnt_column] << " ";
            else
                outfile << ms[cnt_line][cnt_column] << " ";
        }   // 对齐输出
        outfile << endl;
    }
    outfile.close();
}

void SA(int** ms) // simulated annealing 模拟退火算法，寻找可能存在的八幻方
{
    double T = 1e20;			//开始温度
    double T_min = 1e-8;		//结束温度
    double per = 0;				//状态转移概率
    double lame = 0.99999;		//退火系数
    int current_delta, next_delta, del;		//分别记录当前状态、下一时刻状态、状态差值

    long long sa_conut = 0;		//记录退火次数
    int last_delta = 0;
    int sol_cnt = 0;
    clock_t startTime = clock();

    while (1)
    {
        T *= lame;
        sa_conut++;

        if (0 == delta(ms)) // 找到解
        {
            tofile(ms);
            // 写入 .pgm 文件中
            sol_cnt++;
            cout << endl;
            cout << "Find " << sol_cnt << ": " << endl;
            clock_t endTime = clock();
            cout << "Calculation time: " << ((double)endTime - (double)startTime) / CLOCKS_PER_SEC << "s" << endl << endl;
            startTime = clock();

            output(ms);
            shuffle(ms); // 找到解后重新乱序
            continue;
        }

        if (0 == sa_conut % (long long)1e3) //进度条
        {
            
            // cout << "Iterations times: " << sa_conut << " delta: " << delta(ms) << endl;
            if (delta(ms) == last_delta)
            {
                // cout << "不得行！" << endl;
                shuffle(ms);       // 刚刚不算数，重来
                continue;
            }
            last_delta = delta(ms);
        }

        int x1 = rand_int();
        int y1 = rand_int();
        int x2 = rand_int();
        int y2 = rand_int();
        while (x1 == x2 && y1 == y2)
        {
            x2 = rand_int();
            y2 = rand_int();
        } // 制造两个不同行不同列的随机点对

        current_delta = delta(ms);
        exc(ms, x1, y1, x2, y2);
        next_delta = delta(ms);
        del = next_delta - current_delta; // 计算两次状态的差值
        if (del < 0)
            continue;   // 如果遇到更佳状态，直接转移
        else
        {
            per = exp(-del / T);	//计算转移概率
            double r = (double)(rand() % 1000) / 1000;		//制造随机值
            if (r < per)
                continue;
            // 非更佳状态，以一定概率转移
            else
                exc(ms, x1, y1, x2, y2);	// 回溯
        }
    }
}

int main()
{
    srand((int)time(NULL));
    // 设置时间种子

    int** mm = init(ORDER);
    // 申请内存空间，命名为mm

    first_sol_8(mm);
    // 找出第一个解

    SA(mm);
    release(mm);

    return 0;
}