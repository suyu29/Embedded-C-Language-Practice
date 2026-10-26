#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

int column_max3(const uint16_t samples[][3], size_t rows,
                 uint16_t out_max[3])
{
    /* TODO: 按 README 的 Q5 实现，保持输入矩阵只读。 */
    (void)samples;
    (void)rows;
    (void)out_max;
    return -99;
}

int main(void)
{
    /* TODO: 用 const uint16_t matrix[][3] 构造输入，验证每列最大值和空矩阵。 */
    puts("W02 Q5 unfinished: implement column_max3 and add your tests.");
    return 0;
}
