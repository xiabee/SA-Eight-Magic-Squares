## 基于模拟退火算法寻找八幻方问题的解

| 项目     | 内容               |
| -------- | ------------------ |
| 时间     | 2020.11.13         |
| 作者     | xiabee             |
| 主要算法 | 随机算法; 模拟退火 |



#### 0x00 目录结构

```
.
├── Images
│   ├── 00001.pgm
│   ├── 00002.pgm
│   ├── 00003.pgm
│   ├── 00004.pgm
│   ├── 00005.pgm
│   ├── 00006.pgm
│   ├── 00007.pgm
│   ├── 00008.pgm
│   ├── 00009.pgm
│   ├── 00010.pgm
│   ├── 00011.pgm
│   ├── 00012.pgm
│   ├── 00013.pgm
│   ├── 00014.pgm
│   ├── 00015.pgm
│   ├── 00016.pgm
│   └── ...
├── out.cpp
├── out.exe
└── README.md

1 directory, 20 files
```

* 代码文件：`out.cpp`
* 可执行文件：`out.exe`
* 生成`pgm`：`./Image`



#### 0x01 主要函数

* 核心函数：`SA(int **ms)`： 利用模拟退火算法，寻找最优解
* `first_sol_8(int **ms)`：生成初始解
* `tofile(int** ms)`：将合法幻方写入文件



#### 0x02 计算方法

* 记录各行各列各对角线之和的标准差，以此值为标准判断当前解是否接近合法幻方解：当标准差降到0时，当前解即为合法幻方
* 利用模拟退火算法，以标准差为参量，随机计算可能的合法解



#### 0x03 关键退火步骤

* 找到合法解：

```c++
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
```



* 跳出局部困境：在退火`1e3`次之后，若`delta`的值不变，则认为陷入局部困境，重新乱序

```c++
if (0 == sa_conut % (long long)1e3) //进度条
{
    if (delta(ms) == last_delta)
    {
        // cout << "不得行！" << endl;
        shuffle(ms);       // 刚刚不算数，重来
        continue;
    }
    last_delta = delta(ms);
}
```



* 条件转移：遇到更优解，直接转移；否则，以一定概率转移

```c++
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
```

