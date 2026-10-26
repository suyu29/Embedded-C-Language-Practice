#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

int merge_sorted_samples(const uint16_t *a, size_t a_count,
                         const uint16_t *b, size_t b_count,
                         uint16_t *dst, size_t dst_capacity, size_t *out_count)
{
    /* TODO: 按 README 的 Q6 实现，合并前确认输入有序和容量足够。 */
    (void)a;
    (void)a_count;
    (void)b;
    (void)b_count;
    (void)dst;
    (void)dst_capacity;
    (void)out_count;
    return -99;
}

int main(void)
{
    /* TODO: 自测重复值、空表、无序输入、容量不足和失败输出不变。 */
    puts("W02 Q6 unfinished: implement sorted merge and add your tests.");
    return 0;
}
