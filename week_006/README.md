# 第六周：函数指针、回调与注册机制

原计划日期：2026-11-16～2026-11-22，可按实际进度完成。共 6 道题和 1 道挑战。

**题目在本 README，答案写在本目录的 `q01.c`～`q06.c`、`challenge.c`。** Q1 是分析题；其他题实现指定函数，并在各自 `main()` 中自测。框架没有参考实现。

本周重点：函数指针类型、检查后调用、用户上下文、回调注册和注销，以及按顺序通知多个接收者。

## 作答与编译约定

- 示例输入是调用前状态，输出包括返回值、调用后状态和回调被执行的次数。自测时可以在当前 `.c` 中添加 `static` 测试回调。
- 这是单线程模拟：不使用中断、RTOS、动态内存或全局可变业务状态。回调在当前函数内同步执行，函数返回前完成。
- 回调及其上下文必须在使用期间有效。`void *context` 是传给回调的数据指针，允许为 NULL 的具体规则见各题；函数指针不要转换成 `void *`。
- 每题类型和函数都在对应独立文件中实现；本周题目之间不需要链接，Q5、Q6 可自行初始化本题的槽位。
- `return -99;` 和 `unfinished` 是占位。编译成功只说明框架语法正确，不说明练习已完成。

```powershell
cd 'E:\Embedded-C-Practice\week_006'
mingw32-make q02
.\q02.exe
```

`mingw32-make all` 编译七题；VS Code 打开对应 `.c` 后按 `Ctrl+Shift+B` 编译、F5 调试。

## Q1. 认识函数指针

**难度：简单**　**标签：函数类型、声明、调用**　**答题文件：`q01.c`**

### 题目描述

给定函数指针类型：

```c
typedef uint16_t (*TransformFn)(uint16_t value);
```

在 `q01.c` 注释中回答，并用自定义的简单函数验证：

1. `TransformFn` 指向的函数接收什么参数、返回什么类型？
2. 已有 `TransformFn fn` 时，怎样判断它当前是否可调用？怎样通过它调用函数？
3. 下面两个函数声明中，哪个可以赋给 `TransformFn`？为什么？

```c
uint16_t transform_a(uint16_t value);
void transform_b(uint16_t value);
```

4. 为什么不能通过强制类型转换，把参数或返回类型不兼容的函数当成匹配函数调用？
5. 函数指针与 `void *` 数据指针的用途有什么区别？

### 提示与约束

- 不调用 NULL 函数指针；不实际通过错误类型调用函数。
- 测试函数由你自行编写；本题不要求实现注册表。

## Q2. 调用一个采样转换函数

**难度：简单**　**标签：函数指针、返回值、NULL 检查**　**答题文件：`q02.c`**

### 题目描述

将 input 交给 transform 处理，把回调的返回值写入 out_value。函数自身用返回值报告是否成功。

### 函数签名

```c
typedef uint16_t (*TransformFn)(uint16_t value);

int call_transform(uint16_t input, TransformFn transform, uint16_t *out_value);
```

### 示例 1

```text
输入：input = 20，transform 指向一个对本次输入返回 21 的函数，out_value = 99
输出：返回值 = 0, out_value = 21，transform 调用 1 次
解释：21 是回调计算得到的数据，0 是 call_transform 的状态。
```

### 示例 2

```text
输入：input = 20, transform = NULL, out_value = 99
输出：返回值 = -1, out_value = 99
解释：空函数指针不能调用。
```

### 提示与约束

- transform 或 out_value 为 NULL 返回 -1；检查通过后调用恰好一次，写入结果并返回 0。
- 返回 -1 时不调用回调，不修改有效的 out_value。input 和结果使用 `uint16_t` 完整范围。
- 自测回调类型必须匹配 `TransformFn`；避免用溢出或未定义行为构造测试函数。

### 补充测试

覆盖原值返回、其他转换结果、返回 0 和 65535、两种 NULL，以及确认错误时回调次数不增加。

## Q3. 带上下文通知一次采样

**难度：中等**　**标签：回调、用户数据、参数检查**　**答题文件：`q03.c`**

### 题目描述

通道采样有效时，调用 callback，向它传入 channel、adc 和原样传入的 context。回调可以用 context 找到调用者自己的状态。

### 函数签名

```c
typedef void (*SampleCallback)(uint8_t channel, uint16_t adc, void *context);

int notify_sample(SampleCallback callback, void *context,
                  uint8_t channel, uint16_t adc);
```

### 示例 1

```text
输入：callback 指向计数回调，context 指向初始为 4 的计数对象，
      channel = 1, adc = 2048
输出：返回值 = 0，回调执行 1 次，计数对象变为 5
解释：计数行为是调用者自己编写的回调产生的效果。
```

### 示例 2

```text
输入：callback 指向计数回调，context 指向初始为 4 的计数对象，
      channel = 3, adc = 2048
输出：返回值 = -2，回调执行 0 次，计数对象仍为 4
解释：本题通道只能是 0、1、2。
```

### 提示与约束

- callback=NULL 返回 -1；随后 channel>2 或 adc>4095 返回 -2；通过后调用一次并返回 0。
- context 可以为 NULL，前提是调用者提供的回调能接受 NULL；非 NULL 时对象的真实类型、对齐和存储期由调用者保证。
- 本函数不保存 callback/context，不自行修改 context 指向的对象；回调允许按其约定修改自己的上下文。

### 补充测试

覆盖通道 0、2、3，adc 0、4095、4096，NULL 回调、可接受 NULL 的上下文，以及回调接收到的参数。

## Q4. 注册一个回调

**难度：中等**　**标签：注册、函数指针、上下文生命周期**　**答题文件：`q04.c`**

### 题目描述

一个槽位保存一个采样回调和其上下文。槽位为空时注册；已经有回调时不覆盖旧内容。

### 函数签名

```c
typedef void (*SampleCallback)(uint8_t channel, uint16_t adc, void *context);
typedef struct {
    SampleCallback callback;
    void *context;
} CallbackSlot;

int register_sample_callback(CallbackSlot *slot, SampleCallback callback, void *context);
```

### 示例 1

```text
输入：slot = {callback:NULL, context:NULL}，callback = 回调 A，context = 地址 X
输出：返回值 = 0，slot = {callback:回调 A, context:地址 X}
解释：空槽位保存回调及其对应上下文，不执行回调。
```

### 示例 2

```text
输入：slot = {callback:回调 A, context:地址 X}，callback = 回调 B，context = 地址 Y
输出：返回值 = 1，slot 仍为 {callback:回调 A, context:地址 X}
解释：槽位已占用，不覆盖原来的注册。
```

### 提示与约束

- slot=NULL 或 callback=NULL 返回 -1；槽位已有回调返回 1；成功登记返回 0。
- context 可为 NULL；函数只保存地址，不分配或释放上下文，不调用回调。
- 使用前将槽位初始化为 `CallbackSlot slot = {0};`，之后保证槽位由本题约定的操作维护；失败时成员保持原值。

### 补充测试

覆盖正常注册、重复注册、不同回调尝试占用、NULL 回调、NULL 槽位、NULL 上下文和回调未执行。

## Q5. 注销一个回调

**难度：中等**　**标签：函数指针比较、注销、资源归属**　**答题文件：`q05.c`**

### 题目描述

使用 Q4 相同的 CallbackSlot 和 SampleCallback 类型。如果槽位中保存的回调与 callback 相同，则清空槽位；否则保留原值。

### 函数签名

```c
int unregister_sample_callback(CallbackSlot *slot, SampleCallback callback);
```

### 示例 1

```text
输入：slot = {callback:回调 A, context:地址 X}，callback = 回调 A
输出：返回值 = 0，slot = {callback:NULL, context:NULL}
解释：匹配后清空槽位；不释放地址 X 指向的对象。
```

### 示例 2

```text
输入：slot = {callback:回调 A, context:地址 X}，callback = 回调 B
输出：返回值 = 1，slot 内容不变
解释：槽位没有注册回调 B。
```

### 提示与约束

- slot=NULL 或 callback=NULL 返回 -1；槽位为空或回调不匹配返回 1；匹配并清空返回 0。
- 使用匹配类型的函数指针进行相等比较；不要转换成数据指针或整数比较。
- 本题按回调函数匹配，不额外比较 context；不调用回调，也不释放上下文。
- 在本文件中直接构造测试槽位，不需要链接 Q4。

### 补充测试

覆盖匹配、空槽位、不同回调、NULL 参数、清空后的两个成员和上下文对象仍有效。

## Q6. 派发一次采样回调

**难度：中等**　**标签：已注册回调、输入校验、派发**　**答题文件：`q06.c`**

### 题目描述

使用 Q4 相同的 CallbackSlot 和 SampleCallback 类型，将有效采样派发给槽位中登记的回调。

### 函数签名

```c
int dispatch_sample_callback(const CallbackSlot *slot, uint8_t channel, uint16_t adc);
```

### 示例 1

```text
输入：slot 保存回调 A 和上下文 X，channel = 1, adc = 2000
输出：返回值 = 0，回调 A 用 (1,2000,X) 执行 1 次
解释：参数通过原先登记的上下文一起交给回调。
```

### 示例 2

```text
输入：slot = {callback:NULL, context:NULL}，channel = 1, adc = 2000
输出：返回值 = 1，没有回调被执行
解释：采样有效，但槽位为空。
```

### 示例 3

```text
输入：slot 为空，channel = 3, adc = 2000
输出：返回值 = -2，没有回调被执行
解释：先校验采样参数，再报告空槽位。
```

### 提示与约束

- 依次检查：slot=NULL 返回 -1；channel>2 或 adc>4095 返回 -2；槽位为空返回 1；否则调用一次并返回 0。
- 失败或空槽位时不调用回调。函数自身不修改 slot；回调不得在本次派发过程中修改该槽位。
- 已注册回调和 context 由调用者保证有效；context 可以为 NULL，但回调必须支持该值。
- 在本文件中直接构造测试槽位，不需要链接 Q4 或 Q5。

### 补充测试

覆盖空槽位、有效回调及上下文、参数上下界、非法参数与空槽位同时出现、NULL 槽位和调用次数。

## 挑战. 管理多个采样监听者

**难度：中等偏难**　**标签：注册表、注销、回调顺序、边界**　**答题文件：`challenge.c`**

### 题目描述

实现容量为 4 的监听表。允许注册多个“回调函数 + context”组合，注销指定组合，并按注册顺序向所有监听者派发有效采样。

### 函数签名

```c
#define HUB_CAPACITY 4
typedef void (*SampleCallback)(uint8_t channel, uint16_t adc, void *context);
typedef struct {
    SampleCallback callback;
    void *context;
} Listener;
typedef struct {
    Listener entries[HUB_CAPACITY];
    size_t count;
} EventHub;

int hub_register(EventHub *hub, SampleCallback callback, void *context);
int hub_unregister(EventHub *hub, SampleCallback callback, void *context);
int hub_publish(const EventHub *hub, uint8_t channel, uint16_t adc,
                size_t *out_called);
```

### 示例 1：注册与派发

```text
输入：空 hub 依次注册 (回调 A, 地址 X)、(回调 B, 地址 Y)；
      publish 的 channel = 1, adc = 1000, out_called = 99
输出：两次注册均返回 0；publish 返回 0，依次调用 A、B，out_called = 2
解释：每个回调接收采样参数及其注册时的上下文。
```

### 示例 2：重复注册

```text
输入：hub 已包含 (回调 A, 地址 X)，再次注册相同组合
输出：返回值 = 1，hub 的条目和 count 不变
解释：同一函数和同一个 context 组成的组合不能重复。
```

### 示例 3：注销后保持顺序

```text
输入：hub 按顺序包含 A、B、C 三个不同组合，注销 B
输出：返回值 = 0，活动顺序为 A、C，count = 2
解释：后面的条目前移，释放出的最后一个槽位的 callback 和 context 清空。
```

### 提示与约束

- 初始化使用 `EventHub hub = {0};`；活动条目为 `entries[0..count)`。调用者之后只通过本题接口维护表，不在回调中修改或重新进入同一个 hub。
- 三个函数先检查必须指针：hub 为 NULL 返回 -1；注册或注销时 callback 为 NULL 返回 -1；publish 时 out_called 为 NULL 返回 -1。context 可以为 NULL。
- 指针检查后，若 count>4 或活动条目中存在 NULL callback，返回 -2，不能访问越界条目或执行任何回调。其他伪造的内部状态不要求检测。
- 注册：先检查完全相同的 `(callback, context)`，已存在返回 1；否则已满返回 -3；否则追加并返回 0。同一个回调配不同 context 是不同组合。
- 注销：组合不存在返回 1；存在则保持其他条目的相对顺序，清空原末尾槽位，count 减 1，返回 0。不释放 context。
- 发布：状态检查后，channel>2 或 adc>4095 返回 -2；否则按活动顺序各调用一次，写 `*out_called = count` 并返回 0。空表也是成功，交付 0。
- 返回非 0 时，注册表及有效的 out_called 保持原值；publish 错误时不调用回调。out_called 对象与 hub 存储不重叠。调用者保证回调及 context 存储期有效，回调不修改 hub 或 out_called，不需要处理并发。

### 补充测试

覆盖空表发布、四个槽位恰好用满、满表新组合返回 -3、满表重复组合返回 1、同函数不同 context、注销首项/末项/不存在项、再次注册后的顺序、无效 count、活动条目 NULL、非法采样和 NULL 参数。

## 建议顺序与完成标准

| 顺序 | 练习重点 |
| --- | --- |
| 第 1 天 | Q1：理解函数指针类型和调用 |
| 第 2 天 | Q2：检查并调用一次转换回调 |
| 第 3 天 | Q3：传递上下文和有效采样 |
| 第 4 天 | Q4：注册单个回调 |
| 第 5 天 | Q5：按函数注销并保留资源归属 |
| 第 6 天 | Q6：派发回调和处理空槽位 |
| 第 7 天 | 挑战：多个监听者的顺序、容量和注销 |

完成时应能解释函数指针与数据指针的区别、上下文的生命周期，并通过自测验证回调次数与调用顺序。
