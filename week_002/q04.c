#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

int erase_adc_range(uint16_t *samples, size_t capacity, size_t *inout_count,
                    size_t erase_index, size_t erase_count)
{
    /* TODO: 按 README 的 Q4 实现，区分物理容量和逻辑长度。 */
    (void)samples;
    (void)capacity;
    (void)inout_count;
    (void)erase_index;
    (void)erase_count;
    return -99;
}

int main(void)
{
    /* TODO: 自测中间/首尾/全部删除、零删除、无效区间及未目标位置不变。 */
    puts("W02 Q4 unfinished: implement in-place erase and add your tests.");
    return 0;
}
