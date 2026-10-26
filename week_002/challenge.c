#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

int extract_channel_window(const uint16_t samples[][3], size_t rows,
                            size_t first_row, size_t window_count,
                            size_t row_stride, size_t channel,
                            uint16_t *dst, size_t dst_capacity, size_t *out_count)
{
    /* TODO: 按 README 的挑战题实现，先验证完整窗口再修改输出。 */
    (void)samples;
    (void)rows;
    (void)first_row;
    (void)window_count;
    (void)row_stride;
    (void)channel;
    (void)dst;
    (void)dst_capacity;
    (void)out_count;
    return -99;
}

int main(void)
{
    /* TODO: 自测三个通道、带步长窗口、空窗口、越界、容量和 SIZE_MAX 步长。 */
    puts("W02 challenge unfinished: implement window extraction and add your tests.");
    return 0;
}
