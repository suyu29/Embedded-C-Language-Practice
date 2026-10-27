# 第五周：二级指针、void * 与 const

原计划日期：2026-11-09～2026-11-15，可按实际进度完成。共 6 道题和 1 道挑战。

**题目在本 README，答案写在本目录的 `q01.c`～`q06.c`、`challenge.c`。** Q1 是分析题；其他题实现指定函数，并在各自 `main()` 中自测。框架没有参考实现。

本周重点：让函数修改调用者的指针、返回数组元素地址、`const` 所限制的对象、`void *` 上下文，以及由指针组成的数组。

## 作答与编译约定

- 示例输入是调用前状态，输出是返回值和调用后状态；数量以元素个数为单位。输出指针指向的变量由调用者事先定义。
- 调用者保证非 NULL 指针、对象类型、对齐、存储期及数组长度正确；不能依靠函数检测一个任意地址是否有效。
- 涉及指针推进时，调用者保证当前位置和剩余范围属于同一数组，或位置是该数组的尾后位置；尾后指针不能解引用。
- 输出参数对象与只读输入、游标及其他输出对象彼此不重叠。题目允许修改的内容会单独说明。
- 返回错误或“未找到”时，相关输出与输入输出参数保持原值。`return -99;` 和 `unfinished` 为待实现占位。
- 使用 C11，不使用动态内存、HAL、RTOS；每个 `.c` 独立编译和自测。

```powershell
cd 'E:\Embedded-C-Practice\week_005'
mingw32-make q02
.\q02.exe
```

`mingw32-make all` 编译七题；VS Code 打开对应 `.c` 后按 `Ctrl+Shift+B` 编译、F5 调试。

## Q1. 分析 const 和指针

**难度：简单**　**标签：const、指针自身、void 指针**　**答题文件：`q01.c`**

### 题目描述

给定：

```c
uint16_t x = 10, y = 20;
const uint16_t *a = &x;
uint16_t *const b = &x;
const uint16_t *const c = &x;
void *raw = &x;
```

在 `q01.c` 的注释中回答，并通过合法语句或断点验证：

1. 对 a，能否执行 `a = &y` 和 `*a = 30`？为什么？
2. 对 b，能否执行 `b = &y` 和 `*b = 30`？为什么？
3. 对 c，上述两种修改分别是否允许？
4. raw 保存了什么？要按 `uint16_t` 访问 x，为什么需要使用匹配的指针类型？`void *` 能否直接解引用？
5. 如果 x 本身声明为 const，是否可以通过强制转换去修改它？

### 提示与约束

- 区分“指针能否改为指向别处”和“能否通过该指针修改所指对象”。
- 不要在程序中实际执行不合法的赋值或对 const 对象的修改；把这些分析写在注释中。
- `void *` 上下文只在指向的真实对象类型和对齐满足访问要求时，才能按相应类型访问。

## Q2. 推进采样指针

**难度：简单到中等**　**标签：二级指针、剩余数量、尾后位置**　**答题文件：`q02.c`**

### 题目描述

`*cursor` 指向当前采样位置，`*remaining` 是从该位置开始的有效元素数。跳过 skip 个元素，并同步更新指针和剩余数量。

### 函数签名

```c
int advance_adc_cursor(const uint16_t **cursor, size_t *remaining, size_t skip);
```

### 示例 1

```text
输入：samples = [100,200,300,400]，cursor 原值为 &samples[1]，
      remaining = 3, skip = 2
输出：返回值 = 0，cursor 新值为 &samples[3]，remaining = 1
解释：跳过 200 和 300，下一次从 400 开始。
```

### 示例 2

```text
输入：samples = [100,200,300,400]，cursor 原值为 &samples[1]，
      remaining = 3, skip = 4
输出：返回值 = -2，cursor 和 remaining 保持原值
解释：要跳过的数量超过剩余有效元素。
```

### 示例 3

```text
输入：cursor 原值为 NULL，remaining = 0, skip = 0
输出：返回值 = 0，cursor 仍为 NULL，remaining = 0
解释：空范围允许不推进，不对 NULL 做指针运算。
```

### 提示与约束

- cursor 或 remaining 为 NULL，或 `*remaining>0` 且 `*cursor=NULL`，返回 -1；再检查 skip，超出剩余数量返回 -2。
- `*remaining=0` 时 `*cursor` 可以为 NULL 或有效数组的尾后指针；skip=0 成功且不需要指针运算。
- skip 等于剩余数量时，非空数组的指针推进至尾后，remaining 变 0；不能解引用尾后指针。
- 成功不修改数组元素；失败时指针变量和剩余数量都不变。两个参数对象彼此独立。

### 补充测试

覆盖 skip=0、1、恰好等于 remaining、大于 remaining、SIZE_MAX，NULL 游标参数、NULL 指针配非零数量以及空范围。

## Q3. 返回匹配元素的地址

**难度：中等**　**标签：输出指针、生命周期、只读数据**　**答题文件：`q03.c`**

### 题目描述

查找 samples 中第一个等于 target 的值。找到时把该元素的地址写入 out_match 并返回 0；未找到返回 1，保留 out_match 原值。

### 函数签名

```c
int find_adc_pointer(const uint16_t *samples, size_t count,
                     uint16_t target, const uint16_t **out_match);
```

### 示例 1

```text
输入：samples = [100,200,200,300], count = 4, target = 200, out_match 原值 = NULL
输出：返回值 = 0, out_match 新值 = &samples[1]，该地址处的值为 200
解释：有两个 200，只返回第一个的地址。
```

### 示例 2

```text
输入：samples = NULL, count = 0, target = 200, out_match 原值 = NULL
输出：返回值 = 1, out_match 仍为 NULL
解释：空数组中没有匹配项。
```

### 提示与约束

- out_match=NULL，或 count>0 且 samples=NULL，返回 -1；count=0 时允许 samples=NULL。
- 未找到或失败时不修改 `*out_match`；不能返回函数内局部变量的地址。
- 返回的地址指向原数组，其可用时间受原数组存储期限制；不通过结果修改 `const` 输入。
- 时间复杂度 O(count)，额外空间 O(1)。

### 补充测试

覆盖首元素、末元素、重复值、找不到、输入为空、输出原先指向另一个有效对象和指针错误。

## Q4. 从只读上下文读取 ADC

**难度：中等**　**标签：const void *、上下文、类型匹配**　**答题文件：`q04.c`**

### 题目描述

context 实际指向一个 SensorContext。请求的通道与其中 channel 一致时，输出 adc；不一致时返回 1，不修改输出。

### 函数签名

```c
typedef struct {
    uint8_t channel;
    uint16_t adc;
} SensorContext;

int read_context_adc(const void *context, uint8_t requested_channel,
                     uint16_t *out_adc);
```

### 示例 1

```text
输入：context 指向 {channel:1,adc:2048}, requested_channel = 1, out_adc = 99
输出：返回值 = 0, out_adc = 2048
解释：请求通道与上下文通道一致。
```

### 示例 2

```text
输入：context 指向 {channel:1,adc:2048}, requested_channel = 2, out_adc = 99
输出：返回值 = 1, out_adc = 99
解释：两个通道均合法，但不匹配。
```

### 示例 3

```text
输入：context 指向 {channel:3,adc:2048}, requested_channel = 1, out_adc = 99
输出：返回值 = -2, out_adc = 99
解释：本题通道只允许 0、1、2。
```

### 提示与约束

- context 或 out_adc 为 NULL 返回 -1；请求通道或上下文通道大于 2 返回 -2；合法但不匹配返回 1。
- 调用者必须传入真实的、正确对齐的 `SensorContext` 对象，不能把任意字节数组或其他类型假装成该对象；函数无法从 `void *` 自行验证真实类型。
- 用与原对象兼容的只读指针访问；不直接解引用 `void *`，不修改上下文。adc 可以是 `uint16_t` 的任意值。

### 补充测试

覆盖三个合法通道、非法请求通道、非法上下文通道、匹配和不匹配、0 与 65535 以及各指针错误。

## Q5. 从多个通道数组查找最大值

**难度：中等**　**标签：指针数组、二级指针、const**　**答题文件：`q05.c`**

### 题目描述

channels 是由指针组成的数组，lengths 记录各通道的元素数量，channel_count 是通道数。在所有非空通道中找最大值，写入 out_max；没有任何数据时返回 1。

### 函数签名

```c
int max_from_channels(const uint16_t *const *channels, const size_t *lengths,
                      size_t channel_count, uint16_t *out_max);
```

### 示例 1

```text
输入：channels 指向三个通道 [100,500]、[]、[300,700,200]，
      lengths = [2,0,3], channel_count = 3, out_max = 9
输出：返回值 = 0, out_max = 700
解释：第二个通道长度为 0，可以用 NULL 表示。
```

### 示例 2

```text
输入：channels = NULL, lengths = NULL, channel_count = 0, out_max = 9
输出：返回值 = 1, out_max = 9
解释：没有通道，也没有数据。
```

### 示例 3

```text
输入：channels = [指向 [100] 的指针, NULL], lengths = [1,2],
      channel_count = 2, out_max = 9
输出：返回值 = -1, out_max = 9
解释：第二个通道声称有数据，但指针为 NULL。
```

### 提示与约束

- out_max 必须非 NULL；channel_count>0 时 channels、lengths 必须非 NULL；任一 `lengths[i]>0` 的通道指针必须非 NULL。违反返回 -1。
- 全部指针规则检查通过后，没有元素返回 1；找到最大值返回 0。-1 和 1 均保留 out_max。
- 长度为 0 的通道指针可以为 NULL；通道可长短不同。输入数组和所指数据只读。
- `channels` 是指针数组，不是连续的二维数组，不能把 `uint16_t matrix[][3]` 强制转换成它。
- 时间复杂度 O(channel_count + 各通道元素总数)，额外空间 O(1)。

### 补充测试

覆盖全部空通道、单通道、重复最大值、0 与 65535、后面的通道指针无效、零通道及输出指针错误。

## Q6. 修改多个指针所指向的值

**难度：中等偏难**　**标签：指针数组、指针常量、先验证后修改**　**答题文件：`q06.c`**

### 题目描述

slots 是一个指针数组，每个指针指向一个可写的 `uint16_t` 对象。把值等于 old_value 的对象改为 new_value，并将实际改变的对象数量写入 out_changed。

### 函数签名

```c
int replace_slot_values(uint16_t *const *slots, size_t slot_count,
                        uint16_t old_value, uint16_t new_value, size_t *out_changed);
```

### 示例 1

```text
输入：三个独立对象的值为 [10,20,10]，slots 分别指向它们，
      slot_count = 3, old_value = 10, new_value = 7, out_changed = 99
输出：返回值 = 0，对象值为 [7,20,7], out_changed = 2
解释：只修改两个匹配的目标对象，不改变 slots 中保存的地址。
```

### 示例 2

```text
输入：两个对象原值为 [10,20]，slots = [第一个对象的地址,NULL]，
      slot_count = 2, old_value = 10, new_value = 7, out_changed = 99
输出：返回值 = -1，对象仍为 [10,20], out_changed = 99
解释：发现无效指针后，不能留下第一个对象已被修改的结果。
```

### 示例 3

```text
输入：slots = NULL, slot_count = 0, old_value = 10, new_value = 7, out_changed = 99
输出：返回值 = 0, out_changed = 0
解释：没有需要处理的对象。
```

### 提示与约束

- out_changed=NULL、slot_count>0 且 slots=NULL，或任意条目为 NULL，返回 -1；slot_count=0 允许 slots=NULL。
- 先检查全部指针再修改任何对象。调用者保证所指对象有效且彼此不同，slots 数组和 out_changed 与这些对象不重叠。
- `uint16_t *const *` 限制通过此接口更改 slots 的指针条目，但允许修改各条目指向的值。
- old_value 等于 new_value 时，成功交付 0 个“实际改变的对象”，仍须先检查所有指针。
- 时间复杂度 O(slot_count)，额外空间 O(1)。

### 补充测试

覆盖无匹配、全部匹配、old 与 new 相等、空列表、后一个指针为 NULL，以及目标值和输出数量的失败保护。

## 挑战. 通过通用上下文逐个读取采样

**难度：中等偏难**　**标签：void *、二级指针、const、游标**　**答题文件：`challenge.c`**

### 题目描述

SampleReader 保存当前位置和剩余元素数。通过 `void *` 接收这个读取器；每次调用将当前元素的地址交给调用者，并把读取器推进一个元素。

### 函数签名

```c
typedef struct {
    const uint16_t *cursor;
    size_t remaining;
} SampleReader;

int read_next_sample(void *context, const uint16_t **out_sample);
```

### 示例 1

```text
输入：samples = [100,200,300]，reader = {cursor:&samples[0], remaining:3}，
      context 指向 reader，out_sample 原值 = NULL
输出：返回值 = 0，out_sample 新值 = &samples[0]（值为 100），
      reader = {cursor:&samples[1], remaining:2}
解释：交付原当前位置，再推进到下一个元素。
```

### 示例 2

```text
输入：reader = {cursor:NULL, remaining:0}，out_sample 原值 = NULL
输出：返回值 = 1，reader 和 out_sample 不变
解释：已经没有数据；不解引用 NULL。
```

### 示例 3

```text
输入：reader = {cursor:NULL, remaining:1}，out_sample 原值 = NULL
输出：返回值 = -1，reader 和 out_sample 不变
解释：非空范围不能用 NULL 作为当前位置。
```

### 提示与约束

- context 或 out_sample 为 NULL 返回 -1；将 context 按真实类型访问后，remaining>0 但 cursor=NULL 也返回 -1。
- remaining=0 返回 1；此时 cursor 可以是 NULL 或有效数组尾后指针，不能解引用或对 NULL 做算术运算。
- 成功返回 0：`*out_sample` 接收推进前的地址；cursor 前进 1 个元素，remaining 减 1。读取最后一个元素后 cursor 成为尾后指针。
- 调用者必须传入真实、有效、正确对齐的 `SampleReader`；其 cursor 与 remaining 描述同一数组中的有效范围。out_sample 所指变量与读取器、采样数组不重叠。
- 样本只读，交出的地址在原数组存储期内使用；本题不要求检测已失效的地址。

### 补充测试

连续读取直到结束；单元素；空读取器使用 NULL 或尾后指针；非空却 NULL；context/out_sample 为 NULL；失败时指针和数量不变。

## 建议顺序与完成标准

| 顺序 | 练习重点 |
| --- | --- |
| 第 1 天 | Q1：分辨 const 作用于指针还是数据 |
| 第 2 天 | Q2：通过二级指针推进游标 |
| 第 3 天 | Q3：交付原数组中元素的地址 |
| 第 4 天 | Q4：安全使用只读通用上下文 |
| 第 5 天 | Q5：读取长短不一的指针数组 |
| 第 6 天 | Q6：验证多个目标后再修改 |
| 第 7 天 | 挑战：组合上下文、只读游标和输出指针 |

完成时应能解释指针变量、所指数据、实际类型和生命周期的关系，并用合法边界测试验证每道题。
