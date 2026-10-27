#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include "com.h"

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

    size_t iloop;
    uint16_t ValueCnt = 0;

    if((samples == NULL_PTR) || (out_start == NULL_PTR))
    {
        return -1;
    }
    if (run_length == 0)
    {
        return -2;
    }
    
    for(iloop = 0; iloop < count; iloop++)
    {
        if(samples[iloop] >= threshold)
        {
            ValueCnt++;
            if(ValueCnt >= run_length)
            {
                *out_start = (size_t)(iloop - ValueCnt + 1);
                return 0; // 找到返回0
            }
        }
        else
        {
            ValueCnt = 0; // 清零 
        }
    }
    return 1;
}

int main(void)
{
    /* TODO: 自测连续区间、等于阈值、多个候选、空输入及 SIZE_MAX 长度。 */
    puts("W02 Q3 unfinished: implement the run search and add your tests.");
    return 0;
}
