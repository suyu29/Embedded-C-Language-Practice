# 第四周：结构体、联合体与内存对齐

原计划日期：2026-11-02～2026-11-08，可按实际进度完成。共 6 道题和 1 道挑战。

**题目在本 README，答案写在本目录的 `q01.c`～`q06.c`、`challenge.c`。** Q1 是分析题；其他题实现给定函数，并在各自 `main()` 中自测。框架没有参考实现。

本周重点：通过结构体组织数据、成员偏移和填充、`sizeof`、`offsetof`、`_Alignof`，以及带类型标记的联合体。

## 作答与编译约定

- 示例输入是调用前状态，输出是返回值和调用后状态，不要求键盘输入。数量为数组元素个数，偏移与大小以字节为单位。
- 每题的类型定义位于对应的独立 `.c` 文件中；同名类型不需要跨题链接或共享头文件。
- 调用者保证非 NULL 指针指向有效、正确对齐、存储期内的指定类型对象；声明的数组数量真实。输出对象彼此之间及与只读输入不重叠。
- 返回错误或“未找到”时，输出值和结构体**成员值**保持原状。不要依靠填充字节的值来判断结构体是否相等。
- 使用 C11，不使用动态内存、HAL 或 RTOS。`return -99;` 和 `unfinished` 是占位；每个文件有独立 `main()`。

```powershell
cd 'E:\Embedded-C-Practice\week_004'
mingw32-make q02
.\q02.exe
```

`mingw32-make all` 编译七题；VS Code 打开对应 `.c` 后按 `Ctrl+Shift+B` 编译、F5 调试。

## Q1. 观察结构体和联合体的布局

**难度：简单**　**标签：对齐、填充、sizeof、offsetof**　**答题文件：`q01.c`**

### 题目描述

给定：

```c
typedef struct {
    uint8_t id;
    uint32_t tick;
    uint16_t adc;
} DeviceRecord;

typedef union {
    uint32_t word;
    uint8_t bytes[4];
} WordView;
```

在 `q01.c` 注释中回答，结合 `sizeof`、`offsetof` 和 `_Alignof` 在当前编译器上观察：

1. 三个结构体成员的偏移、各自大小及整个结构体的大小是什么？它们之间可能有哪些填充？
2. 为什么整个结构体的大小不一定等于三个成员大小之和？结构体数组中，相邻元素间隔多少字节？
3. 联合体成员是否各自拥有独立的存储空间？联合体大小是否必须恰好等于各成员大小之和？
4. 为什么不能把当前机器的结构体偏移、填充字节或整数的字节顺序当成所有编译器和 MCU 上都相同的协议格式？

### 提示与约束

- 结果需要依据当前工具链实际观察，不能硬编码为固定数值。`offsetof` 定义在 `<stddef.h>`。
- 不读取未初始化数据；不把读取联合体的其他成员当成可移植的整数拆字节方案。可通过 `unsigned char` 指针观察已初始化对象的表示，但不要依赖填充字节的值。
- 本题只需分析和合法观察，不要求构造报文。

## Q2. 找出有效传感器记录的最大值

**难度：简单到中等**　**标签：结构体数组、成员、有效标记**　**答题文件：`q02.c`**

### 题目描述

每条记录包含 id、adc 和 valid。只在 valid 为 1 的记录中查找最大 adc，写入 out_adc 和 out_index；有相同最大值时取**最早的数组下标**。找到返回 0；没有有效记录返回 1。

### 函数签名

```c
typedef struct {
    uint8_t id;
    uint16_t adc;
    uint8_t valid;
} SensorReading;

int max_valid_reading(const SensorReading *records, size_t count,
                      uint16_t *out_adc, size_t *out_index);
```

### 示例 1

```text
输入：records = [{id:1,adc:100,valid:1}, {id:2,adc:900,valid:0},
                {id:3,adc:500,valid:1}], count = 3, out_adc = 9, out_index = 99
输出：返回值 = 0, out_adc = 500, out_index = 2
解释：中间的 900 标记为无效，不能参与比较。
```

### 示例 2

```text
输入：records = NULL, count = 0, out_adc = 9, out_index = 99
输出：返回值 = 1, out_adc = 9, out_index = 99
解释：没有记录，因此没有最大值。
```

### 示例 3

```text
输入：records = [{id:1,adc:100,valid:1}, {id:2,adc:200,valid:2}], count = 2,
      out_adc = 9, out_index = 99
输出：返回值 = -2, out_adc = 9, out_index = 99
解释：valid 只能为 0 或 1，不能在发现非法记录前提交部分结果。
```

### 提示与约束

- 任一输出指针为 NULL，或 count>0 且 records=NULL，返回 -1；count=0 时允许 records=NULL。
- 完整检查所有 valid：存在非 0、1 的值返回 -2；全部合法但没有有效记录返回 1。这两种情况输出均不变。
- adc 使用 `uint16_t` 完整范围，id 仅用于标识，不参与最大值计算；输入记录始终只读。
- 时间复杂度 O(count)，额外空间 O(1)。

### 补充测试

覆盖全部无效、单条有效、最大值并列、最大值为 0 或 65535、非法 valid 位于最后、count=0 和指针错误。

## Q3. 校准结构体中的 ADC 值

**难度：中等**　**标签：结构体指针、有符号运算、失败保护**　**答题文件：`q03.c`**

### 题目描述

对一条记录的 adc 加上有符号修正值 correction。输入 adc 与修正后的数学结果都必须在 0～4095 内；成功只更新 adc，id 和 status 不变。

### 函数签名

```c
typedef struct {
    uint8_t id;
    uint16_t adc;
    uint8_t status;
} SensorRecord;

int adjust_sensor_adc(SensorRecord *record, int16_t correction);
```

### 示例 1

```text
输入：record = {id:2,adc:1000,status:1}, correction = -200
输出：返回值 = 0, record = {id:2,adc:800,status:1}
解释：校准后为 800，其他成员保持原值。
```

### 示例 2

```text
输入：record = {id:2,adc:100,status:1}, correction = -101
输出：返回值 = -2, record = {id:2,adc:100,status:1}
解释：数学结果 -1 不在允许范围，不能先写回后再判断。
```

### 提示与约束

- record=NULL 返回 -1；输入 adc>4095 或修正结果超范围返回 -2；成功返回 0。
- 使用足够宽的有符号类型表达数学结果，不要先把负修正值转换成 `uint16_t`。
- 失败时所有成员不变；只要求检查 adc，不对 id、status 增加题目之外的限制。

### 补充测试

覆盖结果恰为 0、4095、-1、4096，correction 为 0、INT16_MIN、INT16_MAX，原 adc 非法及 NULL。

## Q4. 使用带标记的联合体判断告警

**难度：中等**　**标签：union、模式标记、成员选择**　**答题文件：`q04.c`**

### 题目描述

mode=0 时使用 fixed 阈值，value 大于 fixed 则告警；mode=1 时使用包含端点的 [low, high] 区间，value 在区间外则告警。将 1（告警）或 0（正常）写入 out_alarm。

### 函数签名

```c
typedef struct {
    uint8_t mode;
    union {
        uint16_t fixed;
        struct { uint16_t low; uint16_t high; } band;
    } limit;
} AlertConfig;

int evaluate_alert(const AlertConfig *config, uint16_t value, uint8_t *out_alarm);
```

### 示例 1

```text
输入：config = {mode:0, fixed:500}, value = 501, out_alarm = 9
输出：返回值 = 0, out_alarm = 1
解释：固定阈值模式下，501 大于 500。
```

### 示例 2

```text
输入：config = {mode:1, low:100, high:300}, value = 100, out_alarm = 9
输出：返回值 = 0, out_alarm = 0
解释：区间包含两端，100 不是越界值。
```

### 示例 3

```text
输入：config = {mode:1, low:300, high:100}, value = 200, out_alarm = 9
输出：返回值 = -2, out_alarm = 9
解释：区间下限不能大于上限。
```

### 提示与约束

- 任一指针为 NULL 返回 -1；mode 不是 0、1，或 mode=1 且 low>high，返回 -2；否则返回 0。
- 调用者根据 mode 正确初始化对应的联合体成员；只读取当前模式对应的成员，不用它观察另一个成员的内存表示。
- 输入只读，失败不修改 out_alarm；阈值和值均允许 `uint16_t` 的完整范围。

### 补充测试

覆盖固定阈值的等于和大于、区间的两个端点和端点外、low=high、非法 mode、倒置区间和 NULL。

## Q5. 描述结构体实际布局

**难度：中等**　**标签：offsetof、对齐、填充字节**　**答题文件：`q05.c`**

### 题目描述

填写当前编译器下 DeviceRecord 的大小、对齐要求、三个成员偏移，以及总填充字节数，写入 out_layout。

### 函数签名

```c
typedef struct {
    uint8_t id;
    uint32_t tick;
    uint16_t adc;
} DeviceRecord;

typedef struct {
    size_t size;
    size_t alignment;
    size_t id_offset;
    size_t tick_offset;
    size_t adc_offset;
    size_t padding_bytes;
} RecordLayout;

int describe_record_layout(RecordLayout *out_layout);
```

### 示例 1

```text
输入：out_layout 指向可写对象
输出：返回值 = 0；各成员反映本编译器的实际布局。
解释：例如某编译器可能得到 size=12、alignment=4、id_offset=0、
      tick_offset=4、adc_offset=8、padding_bytes=5。
      这些数值只是示意，不是所有平台的标准答案。
```

### 示例 2

```text
输入：out_layout = NULL
输出：返回值 = -1
解释：没有可写的输出对象。
```

### 提示与约束

- 输出指针非 NULL 返回 0，NULL 返回 -1。大小和偏移以字节为单位。
- 用 `sizeof`、`_Alignof`、`offsetof` 计算；padding_bytes 包括成员之间和结构体末尾的填充。
- 不硬编码示意数值，不要求使用编译器特有的 packed 属性。

### 补充测试

在本机打印结果，检查成员偏移符合实际、结构体大小至少容纳所有成员，且总填充等于结构体大小减去各成员大小之和；另外检查 NULL。

## Q6. 按成员比较两条记录

**难度：中等**　**标签：结构体比较、填充、可移植性**　**答题文件：`q06.c`**

### 题目描述

使用 Q5 相同定义的 DeviceRecord，逐一比较 id、tick、adc。全部相等时把 1 写入 out_equal，否则写 0。

### 函数签名

```c
int record_fields_equal(const DeviceRecord *a, const DeviceRecord *b,
                        uint8_t *out_equal);
```

### 示例 1

```text
输入：a = {id:1,tick:1000,adc:500}, b = {id:1,tick:1000,adc:500}, out_equal = 9
输出：返回值 = 0, out_equal = 1
解释：三个成员值相同；不依赖它们对象表示中的填充字节。
```

### 示例 2

```text
输入：a = {id:1,tick:1000,adc:500}, b = {id:1,tick:1001,adc:500}, out_equal = 9
输出：返回值 = 0, out_equal = 0
解释：tick 不同。
```

### 提示与约束

- 任一指针为 NULL 返回 -1，失败时 out_equal 不变；否则返回 0。
- 两条记录只读；不得使用 `memcmp` 比较整个结构体，因为填充字节不属于题目要求的成员值。
- 调用者保证两条记录的成员已初始化；不要求构造或读取未初始化的填充字节。

### 补充测试

分别仅改变 id、tick、adc；全部相同；a 与 b 指向同一条记录；每个指针依次为 NULL。

## 挑战. 根据事件更新设备状态

**难度：中等偏难**　**标签：结构体、带标记联合体、边界检查**　**答题文件：`challenge.c`**

### 题目描述

设备状态包含最新 ADC 值和错误次数。事件类型为 EVENT_SAMPLE 时更新 ADC；类型为 EVENT_ERROR 时将错误次数加 1。根据类型只读取联合体中对应的数据。

### 函数签名

```c
enum { EVENT_SAMPLE = 1, EVENT_ERROR = 2 };

typedef struct {
    uint8_t id;
    uint16_t last_adc;
    uint16_t error_count;
} DeviceState;

typedef struct {
    uint8_t type;
    union { uint16_t adc; uint8_t error_code; } data;
} DeviceEvent;

int apply_device_event(DeviceState *state, const DeviceEvent *event);
```

### 示例 1

```text
输入：state = {id:1,last_adc:100,error_count:2},
      event = {type:EVENT_SAMPLE, adc:2048}
输出：返回值 = 0, state = {id:1,last_adc:2048,error_count:2}
解释：采样事件只修改最新 ADC。
```

### 示例 2

```text
输入：state = {id:1,last_adc:100,error_count:2},
      event = {type:EVENT_ERROR, error_code:2}
输出：返回值 = 0, state = {id:1,last_adc:100,error_count:3}
解释：有效错误事件只将错误次数加 1。
```

### 示例 3

```text
输入：state = {id:1,last_adc:100,error_count:65535},
      event = {type:EVENT_ERROR, error_code:1}
输出：返回值 = -3，state 的所有成员不变
解释：错误次数已达到 UINT16_MAX，不能回绕为 0。
```

### 提示与约束

- state 或 event 为 NULL 返回 -1。
- EVENT_SAMPLE 的 adc 必须为 0～4095；EVENT_ERROR 的 error_code 必须为 1～3；未知类型或对应数据非法返回 -2。
- 事件合法后，若要增加的 error_count 已为 UINT16_MAX，返回 -3。先判断再修改，任一失败时 state 所有成员不变。
- 调用者按事件类型初始化联合体的对应成员，state 和 event 不重叠；不额外限制已有 id 或 last_adc。
- 本题是单线程模拟，不要求中断、硬件寄存器或报文编码。

### 补充测试

覆盖采样 0、4095、4096，错误码 0、1、3、4，非法类型，error_count 为 65534 和 65535，以及 NULL 和错误优先级。

## 建议顺序与完成标准

| 顺序 | 练习重点 |
| --- | --- |
| 第 1 天 | Q1：观察真实布局与联合体共享存储 |
| 第 2 天 | Q2：结构体数组和成员读取 |
| 第 3 天 | Q3：安全更新单个成员 |
| 第 4 天 | Q4：按标记读取联合体 |
| 第 5 天 | Q5：计算大小、偏移和填充 |
| 第 6 天 | Q6：按成员比较，认识填充 |
| 第 7 天 | 挑战：组合事件、数据校验和状态更新 |

完成时应能解释平台布局和协议格式的区别，编译无警告，并测试正常、非法数据及成员保持原值的情况。
