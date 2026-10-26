#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

int find_first_run_at_least(const uint16_t *samples, size_t count,
                            uint16_t threshold, size_t run_length,
                            size_t *out_start)
{
    /* TODO: 按 README 的 Q3 实现，区分找到、未找到和非法参数。 */
    (void)samples;
    (void)count;
    (void)threshold;
    (void)run_length;
    (void)out_start;
    return -99;
}

int main(void)
{
    /* TODO: 自测连续区间、等于阈值、多个候选、空输入及 SIZE_MAX 长度。 */
    puts("W02 Q3 unfinished: implement the run search and add your tests.");
    return 0;
}
