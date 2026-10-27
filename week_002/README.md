# 第二周：数组、指针与函数传参

原计划日期：2026-10-19～2026-10-25，可按实际学习进度完成。共 6 道题和 1 道挑战，难度比第一周适当提高。

**题目在本 README，答案写在本目录的 `q01.c`～`q06.c`、`challenge.c`。** Q1 是分析题；其余实现给定函数，并在各自的 `main()` 中编写自测。框架不包含参考实现。

本周重点：数组形参退化、显式长度与容量、连续区间、原地数组操作、二维数组参数、多数组合并，以及 `size_t` 索引的溢出问题。

## 作答与编译约定

- 示例的输入表示参数及调用前状态，输出表示返回值及调用后状态；不需要写键盘输入解析程序。
- 数组中的数值默认十进制。除单独说明外，元素允许为完整 `uint16_t` 范围 `[0,65535]`，本周不额外限制为 12 位 ADC 值。
- 返回值表示状态；数据、索引、数量通过输出参数交付。只有 Q3 使用 `1` 表示“未找到”，不要把它当成返回的索引。
- 使用 C11，不使用动态内存、HAL、RTOS、全局可变业务状态；不用 memcpy、memmove、qsort 等库函数代替本周的数组操作。
- 调用者保证非 NULL 指针有效、存储期和对齐正确，并保证实际数组不小于声明的长度或容量。函数不负责探测任意地址是否有效。
- 带 const 的输入始终只读；失败时数组及输出数量、索引等均保留原值。除 Q4 明确说明的原地操作外，输入、输出数组与输出元数据对象彼此不重叠；不要求函数检测重叠，不通过比较任意地址大小来探测它。
- 每个示例独立初始化数据。`return -99;` 和“unfinished”打印是占位内容；编译成功不代表功能测试通过。
- SIZE_MAX 可用于独立的偏移、删除数量、步长等边界测试；不要把小数组的实际长度谎报成 SIZE_MAX，再要求函数识别存储不足。

通过英文入口打开项目后，在本周目录编译：

```powershell
cd 'E:\Embedded-C-Practice\week_002'
mingw32-make q02
.\q02.exe
```

`mingw32-make all` 编译七个独立程序，`mingw32-make run-q02` 编译并运行 Q2。VS Code 打开对应 `.c`，`Ctrl+Shift+B` 编译，F5 调试；继续使用根目录已有配置。

## Q1. 数组形参与数组指针

**难度：简单**　**标签：sizeof、数组退化、指向数组的指针**　**答题文件：`q01.c`**

### 题目描述

给定 main 中的三个声明：

```c
uint16_t samples[6] = {100, 200, 300, 400, 500, 600};
uint16_t *p = samples;
uint16_t (*whole)[6] = &samples;
```

以及函数参数：

```c
static void inspect_samples(uint16_t samples[6], size_t count);
```

请在 q01.c 注释中回答下面五个问题，并在 main 和 inspect_samples 中验证合法表达式：

1. main 中 `sizeof(samples)`、`sizeof(samples[0])`、`sizeof(samples)/sizeof(samples[0])` 分别表示什么？说明字节数量与元素数量的区别。
2. 进入 inspect_samples 后，`sizeof(samples)` 与 main 中同名表达式含义是否相同？形参中的 `[6]` 能否让函数自动取得实参数组长度，或自动检查长度？
3. main 中 p、whole、`*whole` 的类型分别是什么？`p+1` 与 `whole+1` 分别跨过多少个元素或多少字节，用 sizeof 表达。
4. 为什么 `sizeof(p)/sizeof(*p)` 不能用于推导原数组元素个数？不要把当前机器上的偶然结果当成通用规律。
5. 如果本次只处理 samples 前 3 个元素，应该通过哪个参数表达？数组实际有 6 个元素，是否意味着函数本次必须处理全部 6 个？

### 提示与约束

- 本题是概念分析，不要求实现业务算法。框架中的 inspect_samples 已提供，可以添加打印或断点观察。
- 结果按当前编译器观察，并指出哪些大小取决于平台；不要假定指针恒为 4 字节或 8 字节。
- `whole` 是本周学习的数组指针，后续二维数组题会使用相关类型知识。
- 不解引用数组尾后指针，不读取本次有效范围外的数据。
- 对数组形参直接使用 sizeof 时，编译器可能给出针对退化的提醒；记录并解释这条提醒。其余题的完成版本应编译无警告。

## Q2. 有界复制 ADC 子区间

**难度：中等**　**标签：数组切片、长度与容量、错误处理**　**答题文件：`q02.c`**

### 题目描述

给定只读数组 src、源元素数量 src_count，从下标 offset 开始复制 count 个元素到 dst 的开头。成功返回 0，并通过 out_count 写回实际复制数量 count。

源区间必须完全位于 `[0,src_count)` 内；count=0 时允许 offset 等于 src_count。成功只修改 dst 前 count 个元素，其余元素和整个源数组不变。

### 函数签名

```c
int copy_adc_slice(const uint16_t *src, size_t src_count,
                   size_t offset, size_t count,
                   uint16_t *dst, size_t dst_capacity, size_t *out_count);
```

src_count、offset、count、dst_capacity 单位都是元素，不是字节；out_count 指向一个可写的 size_t 对象。

### 示例 1

```text
输入：src = [100,200,300,400,500], src_count = 5, offset = 1, count = 3,
      dst = [999,999,999,999], dst_capacity = 4, out_count = 99
输出：返回值 = 0, dst = [200,300,400,999], out_count = 3
解释：复制源下标 1、2、3，目标第四个元素不变。
```

### 示例 2

```text
输入：src = [100,200,300,400,500], src_count = 5, offset = 4, count = 2,
      dst = [999,999,999,999], dst_capacity = 4, out_count = 99
输出：返回值 = -2, dst = [999,999,999,999], out_count = 99
解释：源数组末尾只剩一个元素，不允许部分复制。
```

### 示例 3

```text
输入：src = [100,200,300,400,500], src_count = 5, offset = 1, count = 3,
      dst = [999,999,999,999], dst_capacity = 2, out_count = 99
输出：返回值 = -3, dst = [999,999,999,999], out_count = 99
解释：实际数组虽有四格，但本次只允许使用两格。
```

### 示例 4

```text
输入：src = NULL, src_count = 0, offset = 0, count = 0,
      dst = NULL, dst_capacity = 0, out_count = 99
输出：返回值 = 0, out_count = 0
解释：合法的空切片，不访问源和目标数组，但要交付复制数量 0。
```

### 提示与约束

- 判定顺序为：指针合法性 → 源切片范围 → 目标容量 → 复制。
- out_count 必须非 NULL；src_count>0 时 src 必须非 NULL；count>0 时 dst 必须非 NULL，否则返回 -1。
- 源区间要求 `offset <= src_count` 且其后的剩余元素足够容纳 count；非法返回 -2。不能因 offset+count 回绕而错误放行。
- 源区间合法但 dst_capacity<count，返回 -3。失败时 dst 和 out_count 均保持原值。
- count=0 仍须检查 offset 和上述指针规则，不忽略源长度。例如有效的 5 元素源数组，offset=6/count=0 仍返回 -2。
- 调用者保证源、目标存储区域不重叠；本题不处理重叠复制。
- 时间复杂度 O(count)，额外空间 O(1)；不使用固定长度中转数组绕过容量检查。

### 补充测试

覆盖 offset=0、恰好复制到源尾、offset=src_count/count=0、offset=SIZE_MAX；out_count 为 NULL；src 为 NULL 但 src_count>0；count>0 且 dst 为 NULL；检查错误优先级、源数组和目标两侧哨兵。

## Q3. 查找首次连续达到阈值的区间

**难度：中等**　**标签：线性扫描、连续区间、输出索引**　**答题文件：`q03.c`**

### 题目描述

设备只有连续 run_length 次采样值均不小于 threshold，才认为满足告警条件。给定 samples 和有效元素数量 count，找到最早满足条件的连续区间起点，将其下标写入 out_start。

找到返回 0；搜索完成但不存在这样的区间，返回 1，保留 out_start 原值；指针错误返回 -1；run_length=0 返回 -2。

### 函数签名

```c
int find_first_run_at_least(const uint16_t *samples, size_t count,
                            uint16_t threshold, size_t run_length,
                            size_t *out_start);
```

out_start 是输出索引的地址，下标从 0 开始；run_length 是所需连续元素数量，不是结束下标。

### 示例 1

```text
输入：samples = [100,800,900,950,200,1000], count = 6,
      threshold = 800, run_length = 3, out_start = 99
输出：返回值 = 0, out_start = 1
解释：下标 1、2、3 的三个值都不小于 800。
```

### 示例 2

```text
输入：samples = [900,100,900,900], count = 4,
      threshold = 800, run_length = 3, out_start = 99
输出：返回值 = 1, out_start = 99
解释：不能把被 100 打断的两段拼起来，最长连续满足条件的长度只有 2。
```

### 示例 3

```text
输入：samples = NULL, count = 0, threshold = 800, run_length = 1, out_start = 99
输出：返回值 = 1, out_start = 99
解释：空输入没有符合条件的非空连续区间。
```

### 提示与约束

- 判定顺序：out_start 必须非 NULL，count>0 时 samples 必须非 NULL，否则 -1；随后 run_length=0 返回 -2；最后搜索。
- run_length>count 表示未找到，返回 1，不是参数错误。run_length=SIZE_MAX、count 很小时也应安全返回 1。
- 每个值与阈值的比较为大于或等于，等于阈值也算满足。
- 有多个候选时返回起点下标最小的一个，允许候选区间彼此重叠。
- 只有找到时修改 out_start；返回 1、-1、-2 时均保留原值。输入数组始终只读。
- 时间复杂度 O(count)，额外空间 O(1)，不能对每个起点重新扫描整个候选区间。
- 索引和连续长度计算不能因 size_t 加减回绕而越界。

### 补充测试

验证 run_length=1、恰好等于 count、超过 count、为 0；全数组满足、没有元素满足、恰好等于阈值；多个候选及末尾才出现的候选；所有 NULL 组合。注意返回 1 表示未找到，不表示找到了下标 1。

## Q4. 原地删除采样数组区间

**难度：中等**　**标签：原地移动、逻辑长度、输入输出参数**　**答题文件：`q04.c`**

### 题目描述

samples 是一个容量为 capacity 的数组，只有前 `*inout_count` 个元素有效。从 erase_index 开始删除 erase_count 个有效元素，保留其余有效元素的相对顺序，更新逻辑长度。

成功返回 0。不能缩小物理数组，也不清零腾出的位置；只修改删除位置到新逻辑末尾之前的元素，其他位置保持原值。

### 函数签名

```c
int erase_adc_range(uint16_t *samples, size_t capacity, size_t *inout_count,
                    size_t erase_index, size_t erase_count);
```

capacity 是物理可用容量，不随删除改变；inout_count 指向已初始化的 size_t 对象，调用前是有效数量，调用后接收新有效数量。索引和数量单位均为元素。

### 示例 1

```text
输入：samples = [10,20,30,40,50,99], capacity = 6, inout_count = 5,
      erase_index = 1, erase_count = 2
输出：返回值 = 0, samples = [10,40,50,40,50,99], inout_count = 3
解释：删除 20、30，新的有效序列为前 3 个元素 [10,40,50]。
      后面的旧值不清零，capacity 仍为 6。
```

### 示例 2

```text
输入：samples = [10,20,30,40,50,99], capacity = 6, inout_count = 5,
      erase_index = 0, erase_count = 5
输出：返回值 = 0, samples = [10,20,30,40,50,99], inout_count = 0
解释：删除全部有效元素只需将逻辑长度设为 0，本题不清空数组存储。
```

### 示例 3

```text
输入：samples = [10,20,30,40,50,99], capacity = 6, inout_count = 5,
      erase_index = 4, erase_count = 2
输出：返回值 = -2, samples = [10,20,30,40,50,99], inout_count = 5
解释：删除区间超过逻辑末尾，不能把容量范围内的第六个元素当作有效数据。
```

### 提示与约束

- 判定顺序：inout_count 为 NULL，或 capacity>0 且 samples 为 NULL，返回 -1；随后检查数量和删除区间，非法返回 -2。
- 调用前必须有 `*inout_count <= capacity`。删除起点不得大于旧逻辑长度，删除数量不得超出起点后的有效元素数量。
- capacity=0 时允许 samples 为 NULL；此时合法逻辑长度只能为 0，合法删除范围只有 index=0/count=0。
- erase_count=0 表示不删除，但仍校验 erase_index；erase_index 等于旧逻辑长度时允许零删除，超过则 -2。
- 失败时整个数组与逻辑长度保持原值。成功时 `[0,erase_index)`、新逻辑长度之后的存储均保留原值，长度只减少 erase_count。
- samples 与 inout_count 对象不得重叠。本题允许在同一个数组内移动元素，不使用动态内存或 memmove。
- 时间复杂度 O(旧逻辑长度)，额外空间 O(1)；区间判断不能因索引加删除数量回绕而失效。

### 补充测试

覆盖删除首元素、末元素、中间、全部、零个；逻辑长度为 0；逻辑长度超过容量；erase_index 或 erase_count 为 SIZE_MAX；NULL 配非零容量；NULL/零容量/零逻辑长度。每次同时检查完整数组和新逻辑长度，不只检查有效前缀。

## Q5. 三通道采样矩阵的列最大值

**难度：中等**　**标签：二维数组、数组指针、只读参数**　**答题文件：`q05.c`**

### 题目描述

DMA 采样缓冲区按“每行一次采样、每列一个通道”组织，固定有 3 个通道。给定 rows 行采样数据，求每个通道的最大值，写入 out_max 的 3 个元素。

成功返回 0。rows=0 时约定三个最大值均为 0，允许 samples 为 NULL。out_max 必须有效，即使没有输入行也要交付三个结果。

### 函数签名

```c
int column_max3(const uint16_t samples[][3], size_t rows,
                 uint16_t out_max[3]);
```

rows 是实际采样行数，不是 uint16_t 元素总数；通道下标固定为 0、1、2。

### 示例 1

```text
输入：samples = [[100,800,300],[150,700,900],[50,900,600]], rows = 3,
      out_max = [999,999,999]
输出：返回值 = 0, out_max = [150,900,900]
解释：每列分别比较，输出的三个位置仍对应通道 0、1、2。
```

### 示例 2

```text
输入：samples = NULL, rows = 0, out_max = [999,999,999]
输出：返回值 = 0, out_max = [0,0,0]
解释：空矩阵采用本题约定值，不解引用 samples。
```

### 示例 3

```text
输入：samples = NULL, rows = 2, out_max = [111,222,333]
输出：返回值 = -1, out_max = [111,222,333]
解释：声称有输入行却没有有效矩阵，输出保持原值。
```

### 提示与约束

- 判定顺序：out_max 为 NULL 返回 -1；rows>0 且 samples 为 NULL 返回 -1；其余情况成功。该题只使用 0、-1。
- 调用者保证 samples 至少有 rows 个连续行，每行恰好 3 个元素；out_max 至少能容纳 3 个 uint16_t，且不与输入重叠。
- 形参中的 out_max[3] 不会自动检查实际容量；提供足够空间是调用者的责任。
- samples 的形参是指向 3 元素一行的数组指针，不是 uint16_t **；不得靠强制类型转换绕过不匹配的行布局。
- 自测输入可声明为 `const uint16_t matrix[][3]`，按 C11 编译并与本题只读接口匹配。
- 不修改输入。失败时三个输出元素全都不变；成功只写这三个元素。
- 时间复杂度 O(rows)，额外空间 O(1)。在注释中解释为何函数还需 rows 参数，以及 samples+1 前进的单位。

### 补充测试

验证单行、各列最大值出现在不同行、重复最大值、全 0、65535、rows=0 且 samples 非 NULL、out_max 为 NULL；用哨兵检查没有写出三元素输出范围。

## Q6. 合并两个有序校准表

**难度：中等**　**标签：多数组传参、双指针、容量校验**　**答题文件：`q06.c`**

### 题目描述

两份只读 Flash 校准阈值表 a、b 已按非递减顺序排列。将全部元素合并到 dst，保持结果非递减，重复值全部保留，并通过 out_count 输出结果数量。

成功返回 0；指针错误返回 -1；任一输入表无序返回 -2；目标容量不足返回 -3。不能通过自动排序修正非法输入，失败时目标及输出数量不变。

### 函数签名

```c
int merge_sorted_samples(const uint16_t *a, size_t a_count,
                         const uint16_t *b, size_t b_count,
                         uint16_t *dst, size_t dst_capacity, size_t *out_count);
```

所有数量和容量单位为元素，out_count 指向可写的 size_t 对象。

### 示例 1

```text
输入：a = [100,300,500], a_count = 3, b = [200,300,400], b_count = 3,
      dst = [999,999,999,999,999,999,999,999], dst_capacity = 8, out_count = 99
输出：返回值 = 0, dst = [100,200,300,300,400,500,999,999], out_count = 6
解释：两个 300 都保留，未使用的最后两个目标元素不变。
```

### 示例 2

```text
输入：a = NULL, a_count = 0, b = [10,20], b_count = 2,
      dst = [999,999,999], dst_capacity = 3, out_count = 99
输出：返回值 = 0, dst = [10,20,999], out_count = 2
解释：其中一个输入表为空是合法情况。
```

### 示例 3

```text
输入：a = [300,100], a_count = 2, b = [200], b_count = 1,
      dst = [999,999,999], dst_capacity = 2, out_count = 99
输出：返回值 = -2, dst = [999,999,999], out_count = 99
解释：输入无序优先于容量不足，不能写入任何部分结果。
```

### 提示与约束

- out_count 必须非 NULL；a_count>0 时 a 必须非 NULL；b_count>0 时 b 必须非 NULL；至少有一个输入非空时 dst 必须非 NULL。违反这些规则返回 -1。
- 指针检查通过后，先检查两输入表是否非递减，无序返回 -2；再检查容量，不足返回 -3；全部通过后合并。
- 总数量为 a_count+b_count。容量判断不能因数量相加回绕而通过；若总数量不能用 size_t 表示，也视为容量不足，返回 -3。
- 两输入都为空时允许 a、b、dst 为 NULL，成功写回 out_count=0；out_count 仍必须非 NULL。
- 成功只更新目标的前总数量个元素，其他元素不变。源数组始终只读，目标与源区域不重叠，输出数量对象独立。
- 时间复杂度 O(a_count+b_count)，额外空间 O(1)，不用临时合并数组或排序库。

### 补充测试

验证两表都空、单元素、全部相等、0 和 65535、一个表的所有值均小于另一个表；目标容量恰好足够、少一个元素、为 0；无序元素出现在表尾；所有指针错误及错误优先级。检查源数据、目标后缀和失败时的输出数量。

## 挑战. 提取带步长的 ADC 通道窗口

**难度：中等偏难**　**标签：二维数组、窗口、索引溢出、输出契约**　**答题文件：`challenge.c`**

### 题目描述

给定一个 rows 行、每行 3 通道的只读 ADC 矩阵，从 first_row 开始，每隔 row_stride 行，取出 channel 通道的一个值，连续提取 window_count 个值写到 dst。

所选择的行号按数学意义为 `first_row + i × row_stride`，其中 `0 <= i < window_count`。每个行号都必须小于 rows。成功返回 0，并将实际输出数量写入 out_count。

### 函数签名

```c
int extract_channel_window(const uint16_t samples[][3], size_t rows,
                            size_t first_row, size_t window_count,
                            size_t row_stride, size_t channel,
                            uint16_t *dst, size_t dst_capacity, size_t *out_count);
```

- rows、first_row、row_stride 以行为单位；row_stride=1 表示相邻行，2 表示隔一行，不是 ADC 数值步长或字节偏移。
- channel 是从 0 计数的列下标，只允许 0、1、2。
- window_count、dst_capacity、*out_count 均表示输出元素数量，不是字节数。

下面三个非空示例使用相同矩阵：

```text
samples = [[100,200,300],
           [110,210,310],
           [120,220,320],
           [130,230,330],
           [140,240,340]]
```

### 示例 1

```text
输入：rows = 5, first_row = 0, window_count = 3, row_stride = 2, channel = 1,
      dst = [999,999,999,999], dst_capacity = 4, out_count = 99
输出：返回值 = 0, dst = [200,220,240,999], out_count = 3
解释：选中第 0、2、4 行的通道 1，未目标元素保持原值。
```

### 示例 2

```text
输入：rows = 5, first_row = 3, window_count = 2, row_stride = 2, channel = 0,
      dst = [999,999,999,999], dst_capacity = 4, out_count = 99
输出：返回值 = -2, dst = [999,999,999,999], out_count = 99
解释：第二个行号为 5，已越界；不能先把第 3 行的数据写进目标。
```

### 示例 3

```text
输入：rows = 5, first_row = 0, window_count = 3, row_stride = 2, channel = 1,
      dst = [999,999,999,999], dst_capacity = 2, out_count = 99
输出：返回值 = -3, dst = [999,999,999,999], out_count = 99
解释：窗口本身有效，但目标本次只允许写入两个元素。
```

### 示例 4

```text
输入：samples = NULL, rows = 0, first_row = 0, window_count = 0,
      row_stride = 1, channel = 2, dst = NULL, dst_capacity = 0, out_count = 99
输出：返回值 = 0, out_count = 0
解释：有效的空窗口，不读取矩阵，不写入目标数组。
```

### 提示与约束

- 判定顺序：指针规则 → 通道、步长与完整窗口范围 → 目标容量 → 写入。
- out_count 必须非 NULL；rows>0 时 samples 必须非 NULL；window_count>0 时 dst 必须非 NULL，否则返回 -1。
- channel 必须小于 3，row_stride 必须至少为 1。first_row 不能大于 rows；window_count>0 时，所有选择的行号还必须严格小于 rows。违反任一条件返回 -2。
- window_count=0 时允许 first_row=rows、dst=NULL，忽略目标容量；但仍校验上述通道、步长、起始位置和指针规则，不能把空窗口当成忽略全部参数。
- 完整窗口有效但 dst_capacity<window_count，返回 -3。窗口越界同时容量不足时，返回 -2。
- 计算行号不能发生 size_t 乘法或加法回绕后误判为合法；验证完成前不能形成越界的行指针。
- 调用者保证矩阵至少有 rows 个连续的三元素行，目标至少有声明的容量；只读输入、目标、输出数量对象彼此不重叠。
- 成功只写前 window_count 个目标元素，并令 *out_count=window_count；源矩阵及目标后缀不变。返回 -1、-2、-3 时，目标和输出数量均保持原值。
- 时间复杂度 O(window_count)，额外空间 O(1)，不复制整张采样矩阵。本文件独立编译，需要辅助函数时自行编写为 static。

### 补充测试

下表除明确列出的参数外，沿用挑战示例 1；“空窗口”表示 window_count=0。

| 场景                                                              | 预期                             |
| --------------------------------------------------------------- | ------------------------------ |
| 示例矩阵，first_row=4/window_count=1/row_stride=SIZE_MAX/channel=2   | 成功，仅输出 340；只选一行，不因未使用的下一步跨度而报错 |
| 示例矩阵，first_row=4/window_count=2/row_stride=SIZE_MAX             | -2，不能让行号回绕成 3 后继续读取            |
| 示例矩阵，first_row=5/window_count=0/row_stride=1/channel=0，dst=NULL | 成功，输出数量 0                      |
| 空窗口但 channel=3 或 row_stride=0                                   | -2，输出数量保留原值                    |
| first_row=SIZE_MAX                                              | -2，输出保持原值                      |
| 分别令三个通道被选择，stride=1，窗口恰好结束于最后一行                                 | 数据正确，行列索引没有混淆                  |
| 任一必须非 NULL 的参数为 NULL                                            | -1，其他有效输出保持原值                  |

## 建议顺序与完成标准

| 顺序    | 练习重点                       |
| ----- | -------------------------- |
| 第 1 天 | Q1：在调用者与函数内分别观察 sizeof 和类型 |
| 第 2 天 | Q2：切片范围、容量及错误优先级           |
| 第 3 天 | Q3：连续区间与线性扫描               |
| 第 4 天 | Q4：原地移动与逻辑长度               |
| 第 5 天 | Q5：二维数组参数与每行跨度             |
| 第 6 天 | Q6：多数组归并与失败前验证             |
| 第 7 天 | 挑战，补充哨兵与大索引测试并审查           |

完成时应能解释参数单位、数组形参退化、调用者责任，以及为什么失败不会留下部分结果。Q2～挑战题编译无警告，main 覆盖正常、空输入、错误优先级和边界用例；不要把只打印“unfinished”的框架当作已完成答案。
