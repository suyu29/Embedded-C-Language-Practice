# 第三周：字符串与内存操作

原计划日期：2026-10-26～2026-11-01，可按实际进度完成。共 6 道题和 1 道挑战。

**题目在本 README，答案写在本目录的 `q01.c`～`q06.c`、`challenge.c`。** Q1 是分析题；其他题实现指定函数，在各自 `main()` 中自测。框架没有参考实现。

本周重点：字符串结束符、数组容量、`memcpy`、`memcmp`、`memmove`、重叠内存和失败时的输出保护。

## 作答与编译约定

- 示例输入表示调用前状态，输出表示返回值和调用后状态；不要求编写键盘输入程序。
- 长度、容量、偏移均以**字节**为单位。字符串长度不包括结尾的 `\0`，但存放字符串时需要为它留一个字节。
- 调用者保证非 NULL 指针有效，实际可用空间不小于传入的容量；各题明确可以为 NULL 的情况。
- 状态返回值与通过指针交付的数据不同。除题目允许的原地操作外，输入、输出缓冲区和输出参数对象彼此不重叠。
- 错误或“未找到”时，题目指定的缓冲区和输出参数均保持原值；长度、偏移计算不能因 `size_t` 回绕而误判。
- 使用 C11，不使用动态内存、HAL、RTOS；每个 `.c` 独立编译。`return -99;` 和 `unfinished` 提示是待实现占位。

通过英文入口打开项目，在本周目录编译：

```powershell
cd 'E:\Embedded-C-Practice\week_003'
mingw32-make q02
.\q02.exe
```

`mingw32-make all` 编译七题，`mingw32-make run-q02` 编译并运行 Q2。VS Code 打开对应 `.c` 后按 `Ctrl+Shift+B` 编译、F5 调试。

## Q1. 字符数组和字符串结束符

**难度：简单**　**标签：字符数组、sizeof、字符串长度**　**答题文件：`q01.c`**

### 题目描述

给定：

```c
char text[8] = {'O', 'K', '\0', 'X', 'Y', '\0', '\0', '\0'};
char raw[2] = {'A', 'B'};
```

在 `q01.c` 注释中回答，并使用合法的打印或断点验证：

1. `sizeof(text)` 表示什么？作为 C 字符串时，text 的长度是多少？
2. 第一个 `\0` 后的 `'X'` 是否仍是数组元素？能否通过合法下标读取？
3. `sizeof(raw)` 是多少？为什么不能直接对 raw 调用 `strlen`？
4. 比较 `"OK"` 的前三个字节与 text 的前三个字节，第三个字节是什么？字符串结束后，还能否把整块数组当作普通字节存储？

### 提示与约束

- `strlen` 依赖结束符；不要对没有 `\0` 的数组调用它，也不要实际执行越界读取。
- 此处 `sizeof` 作用于真正的数组；进入接收指针的函数后，不能用指针的 `sizeof` 推导数组容量。
- 本题只要求分析和合法验证，不要求实现通用字符串函数。

## Q2. 在限定范围内测量字符串长度

**难度：简单**　**标签：结束符、容量、输出参数**　**答题文件：`q02.c`**

### 题目描述

在 buf 指向的 capacity 个字节内查找第一个 `\0`。找到时将它之前的字节数写入 out_length，返回 0；范围内没有结束符返回 -2，保留输出原值。

### 函数签名

```c
int bounded_text_length(const char *buf, size_t capacity, size_t *out_length);
```

### 示例 1

```text
输入：buf = ['O','K','\0','X'], capacity = 4, out_length = 99
输出：返回值 = 0, out_length = 2
解释：第一个结束符位于下标 2，其后的 X 不计入字符串长度。
```

### 示例 2

```text
输入：buf = ['A','B'], capacity = 2, out_length = 99
输出：返回值 = -2, out_length = 99
解释：允许检查的范围内没有结束符，不能继续读取下一个字节。
```

### 示例 3

```text
输入：buf = NULL, capacity = 0, out_length = 99
输出：返回值 = -2, out_length = 99
解释：允许空指针配合零容量，但零个字节中找不到结束符。
```

### 提示与约束

- out_length 为 NULL，或 capacity>0 且 buf 为 NULL，返回 -1；通过后查找结束符。
- capacity=0 时允许 buf 为 NULL，不解引用 buf，结果为 -2；首字节为 `\0` 时成功交付长度 0。
- 输入只读，仅成功时写 out_length；不要用 `strlen(buf)` 扫描一个未确认有结束符的范围。
- 时间复杂度 O(capacity)，额外空间 O(1)。

### 补充测试

覆盖结束符在首字节、最后一个允许字节、多个结束符、无结束符、零容量及各指针错误；检查输出是否保留原值。

## Q3. 有界复制字符串

**难度：中等**　**标签：源容量、目标容量、结束符**　**答题文件：`q03.c`**

### 题目描述

从 src 的 src_capacity 个字节中查找第一个结束符，将结束符之前的内容和结束符一起复制到 dst。成功返回 0，并将不包含结束符的长度写入 out_length。

### 函数签名

```c
int copy_c_string_bounded(const char *src, size_t src_capacity,
                          char *dst, size_t dst_capacity, size_t *out_length);
```

### 示例 1

```text
输入：src = ['O','K','\0','X'], src_capacity = 4,
      dst = ['?','?','?','?'], dst_capacity = 4, out_length = 99
输出：返回值 = 0, dst = ['O','K','\0','?'], out_length = 2
解释：复制 2 个内容字节和 1 个结束符，目标末尾保持原值。
```

### 示例 2

```text
输入：src = ['O','K','\0'], src_capacity = 3,
      dst = ['?','?'], dst_capacity = 2, out_length = 99
输出：返回值 = -3, dst = ['?','?'], out_length = 99
解释：目标必须容纳结束符，两个字节不够。
```

### 示例 3

```text
输入：src = ['A','B'], src_capacity = 2,
      dst = ['?'], dst_capacity = 1, out_length = 99
输出：返回值 = -2, dst = ['?'], out_length = 99
解释：先发现源范围没有结束符，不因目标也不足而返回 -3。
```

### 提示与约束

- 顺序为指针检查 → 源结束符 → 目标容量 → 复制。out_length 必须非 NULL；对应容量大于 0 时 src、dst 必须非 NULL；违反返回 -1。
- src_capacity=0 时允许 src 为 NULL，但找不到结束符，返回 -2。dst_capacity=0 时允许 dst 为 NULL；若源字符串有效，则容量不足返回 -3。
- 复制前检查源范围和目标容量；失败时 dst 与 out_length 均不变，源始终只读。
- 源、目标及输出对象不重叠；可用循环或 `memcpy`，不得对未确认结束符的源调用 `strlen`、`strcpy`。

### 补充测试

覆盖空字符串、目标恰好放下内容和结束符、目标少一个字节、结束符位于源范围末尾、零容量及指针错误。

## Q4. 比较两个字节块

**难度：中等**　**标签：memcmp、二进制数据、输出状态**　**答题文件：`q04.c`**

### 题目描述

比较 a 和 b 的前 count 个字节，把结果规范化后写入 out_order：a 较小写 -1，相等写 0，a 较大写 1。函数本身成功返回 0。

### 函数签名

```c
int compare_byte_blocks(const uint8_t *a, const uint8_t *b,
                        size_t count, int *out_order);
```

### 示例 1

```text
输入：a = [0x01,0xFF,0x00], b = [0x01,0x00,0xFF], count = 3, out_order = 9
输出：返回值 = 0, out_order = 1
解释：首个不同字节是 0xFF 和 0x00，前者较大；0x00 不会终止比较。
```

### 示例 2

```text
输入：a = NULL, b = NULL, count = 0, out_order = 9
输出：返回值 = 0, out_order = 0
解释：两个零字节区间按本题约定相等，不访问 a 或 b。
```

### 提示与约束

- out_order 为 NULL，或 count>0 时 a、b 任一个为 NULL，返回 -1；失败不修改 out_order。
- count=0 时 a、b 可以为 NULL；不要将 NULL 作为参数传给 `memcmp`，即使长度为 0。
- 可使用 `memcmp`；只依据其返回值的正、零、负判断，不能假设它直接返回 -1 或 1。按字节比较，不是比较多字节整数大小。
- 输入至少各有 count 字节，只读且与 out_order 不重叠。

### 补充测试

覆盖首字节、末字节不同，两个缓冲区相等，包含 0x00 和 0xFF，count=0 与必须非 NULL 的参数错误。

## Q5. 移动接收缓冲区中的字节

**难度：中等**　**标签：memmove、重叠区间、范围检查**　**答题文件：`q05.c`**

### 题目描述

在同一个 buffer 中，将从 src_offset 开始的 count 个字节移动到 dst_offset。允许源和目标重叠；移动结果应与先保存原始源字节再写入目标一致。

### 函数签名

```c
int move_rx_window(uint8_t *buffer, size_t capacity,
                   size_t src_offset, size_t dst_offset, size_t count);
```

### 示例 1

```text
输入：buffer = [1,2,3,4,5,6], capacity = 6, src_offset = 0, dst_offset = 2, count = 4
输出：返回值 = 0, buffer = [1,2,1,2,3,4]
解释：源和目标重叠，目标需要得到移动前的 [1,2,3,4]。
```

### 示例 2

```text
输入：buffer = [1,2,3,4,5,6], capacity = 6, src_offset = 2, dst_offset = 0, count = 4
输出：返回值 = 0, buffer = [3,4,5,6,5,6]
解释：向前移动，目标之外的两个字节不变。
```

### 示例 3

```text
输入：buffer = [1,2,3,4,5,6], capacity = 6, src_offset = 4, dst_offset = 0, count = 3
输出：返回值 = -2, buffer = [1,2,3,4,5,6]
解释：源范围超过容量，不能先移动一部分。
```

### 提示与约束

- capacity>0 且 buffer=NULL 返回 -1；capacity=0 时允许 buffer=NULL。
- 两个 offset 都不得大于 capacity，且各自后面必须有 count 个字节，否则 -2。判断不能依赖可能回绕的 `offset + count`。
- count=0 时仍校验两个 offset；offset=capacity 是合法的零长度位置，禁止对 NULL 做指针运算或调用 `memmove`。
- 成功只改变目标范围。允许使用 `memmove`，不能对重叠区域使用 `memcpy`。

### 补充测试

覆盖向前、向后重叠，相同源和目标，不重叠，零长度、尾后位置、SIZE_MAX 偏移或数量，以及错误时整个数组保持不变。

## Q6. 有界追加字符串

**难度：中等偏难**　**标签：字符串追加、容量、错误顺序**　**答题文件：`q06.c`**

### 题目描述

在各自容量范围内确认 dst、src 都是以 `\0` 结束的字符串，将 src 的内容追加到 dst 原有内容后，并写入新字符串长度，不包括结束符。

### 函数签名

```c
int append_text(char *dst, size_t dst_capacity,
                const char *src, size_t src_capacity, size_t *out_length);
```

### 示例 1

```text
输入：dst = ['A','T','\0','#','#','#','#','#'], dst_capacity = 8,
      src = ['+','O','K','\0'], src_capacity = 4, out_length = 99
输出：返回值 = 0, dst = ['A','T','+','O','K','\0','#','#'], out_length = 5
解释：新长度为 5；结束符及其之前的字节被写入，其余目标字节保留。
```

### 示例 2

```text
输入：dst = ['A','\0','?'], dst_capacity = 3,
      src = ['B','C','\0'], src_capacity = 3, out_length = 99
输出：返回值 = -3, dst = ['A','\0','?'], out_length = 99
解释：追加后需要 4 字节，包括结束符；容量只有 3。
```

### 示例 3

```text
输入：dst = ['A','B'], dst_capacity = 2,
      src = ['X','\0'], src_capacity = 2, out_length = 99
输出：返回值 = -2, dst = ['A','B'], out_length = 99
解释：目标范围中没有原字符串的结束符。
```

### 提示与约束

- 顺序为指针检查 → 分别检查两个结束符 → 检查目标容量 → 追加。out_length 必须非 NULL；对应容量大于 0 时指针必须非 NULL，否则 -1。
- 零容量允许对应指针为 NULL，但该范围中没有结束符，返回 -2；结束符缺失优先于容量不足的 -3。
- 目标容量必须容纳原内容、追加内容和一个结束符。运算须避免 `size_t` 回绕；失败时整个目标和 out_length 不变。
- 源、目标及输出对象不重叠。可以在完成检查后使用 `memcpy`，不能用未做边界检查的 `strcat`。

### 补充测试

覆盖空源、空目标、两者均为空、恰好放下、少一个字节、源或目标缺少结束符，以及 NULL 与零容量组合。

## 挑战. 取出一条 UART 文本行

**难度：中等偏难**　**标签：缓冲区、CRLF、memmove、失败保护**　**答题文件：`challenge.c`**

### 题目描述

rx 的前 `*rx_used` 个字节是已收到的数据。寻找第一个连续的 `\r\n`，把它之前的字节复制到 out，并加上 `\0`，然后从 rx 中移除这一行和分隔符，将余下有效字节前移。成功时更新 `*rx_used` 和 `*out_length`（不含结束符）。

没有完整分隔符时返回 1，等待更多数据，不修改任何输出。可按字节搜索，不要求解析协议字段。

### 函数签名

```c
int consume_uart_line(uint8_t *rx, size_t rx_capacity, size_t *rx_used,
                      char *out, size_t out_capacity, size_t *out_length);
```

### 示例 1

```text
输入：rx = ['O','K','\r','\n','A','B',0,0], rx_capacity = 8, rx_used = 6,
      out = ['?','?','?','?'], out_capacity = 4, out_length = 99
输出：返回值 = 0, rx = ['A','B','\r','\n','A','B',0,0], rx_used = 2,
      out = ['O','K','\0','?'], out_length = 2
解释：仅前 2 个 rx 字节仍有效；其余旧存储不清零。
```

### 示例 2

```text
输入：rx = ['O','K','\r'], rx_capacity = 3, rx_used = 3,
      out = ['?','?','?'], out_capacity = 3, out_length = 99
输出：返回值 = 1，rx、rx_used、out 和 out_length 均不变
解释：最后只有 \r，还没有收到随后的 \n。
```

### 示例 3

```text
输入：rx = ['O','K','\r','\n'], rx_capacity = 4, rx_used = 4,
      out = ['?','?'], out_capacity = 2, out_length = 99
输出：返回值 = -3，rx、rx_used、out 和 out_length 均不变
解释：输出必须容纳 O、K 和结束符，共 3 字节。
```

### 提示与约束

- 检查顺序：必须的指针 → `*rx_used <= rx_capacity` → 查找首个 CRLF → 检查该行内容 → 输出容量 → 修改数据。
- rx_used、out_length 必须非 NULL；rx_capacity>0 时 rx 必须非 NULL；out_capacity>0 时 out 必须非 NULL，否则 -1。容量为 0 时对应缓冲区允许为 NULL。
- `*rx_used > rx_capacity` 返回 -2。若尚无完整 CRLF 返回 1；对已找到的第一行，若分隔符之前含有字节 0，也返回 -2，因为它不能完整表示为 C 字符串。分隔符之后的数据不参与本次内容检查。
- 行内容按字节原样复制，不做字符编码转换；本题不校验内容是否为 UTF-8 或其他特定编码。
- 完整行有效但 out_capacity 不足以容纳内容与 `\0`，返回 -3；空行需要 1 字节。所有失败和返回 1 的情况均保持整个 rx、out 和两个数量不变。
- 成功只写 out 的内容及结束符；rx 只修改前移后的有效前缀，旧尾部不清零。两个缓冲区和两个数量对象彼此不重叠。
- 前移可能重叠，可使用 `memmove`；不允许对重叠区域使用 `memcpy`。时间复杂度 O(原 rx_used)，额外空间 O(1)。

### 补充测试

覆盖空输入、空行、只有 `\r`、多个行分隔符、末尾恰好是分隔符、行内含 0、分隔符之后含 0、目标恰好足够或不足、无效 used 和各种指针错误。检查移动后的有效长度和未清除的尾部。

## 建议顺序与完成标准

| 顺序 | 练习重点 |
| --- | --- |
| 第 1 天 | Q1：区分字节数组、结束符和字符串长度 |
| 第 2 天 | Q2：限制扫描范围 |
| 第 3 天 | Q3：复制内容与结束符 |
| 第 4 天 | Q4：字节比较与返回值含义 |
| 第 5 天 | Q5：重叠移动和边界 |
| 第 6 天 | Q6：追加前验证完整输入和容量 |
| 第 7 天 | 挑战：处理不完整数据、移动余量和失败保护 |

完成时应能解释 `memcpy`、`memcmp`、`memmove` 的适用场景，编译无警告，并在 `main()` 覆盖正常、空输入和边界情况。
