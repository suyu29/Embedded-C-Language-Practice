#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

#define UINT16_MAX          65535
#define NULL_PTR            NULL

int sum_adc_range(const uint16_t *begin, const uint16_t *end,
                  uint32_t *out_sum)
{
    /* TODO: 按 README.md 的 Q4 实现。 */
    (void)begin;
    (void)end;
    int iloop;
    uint32_t keep_sum;
    int32_t distance;
    // 非法参数
    if((out_sum == NULL_PTR) || (begin == NULL_PTR) || (end == NULL_PTR))
    {
        out_sum = &keep_sum;
        return -1;
    }
    distance = (end - begin);
    keep_sum = *out_sum;
    *out_sum = 0;
    // 循环差值次数，累加 
    for(iloop = 0; iloop < distance; iloop++)
    {   
        if (begin[iloop] > UINT32_MAX - *out_sum)
        {
            return -2;
        }
        *out_sum += begin[iloop]; 
        /* 此写法会造成回绕仍然检测不出，应该在运算之前检测 */
        // if(*out_sum <= UINT32_MAX)
        // {
        //     *out_sum = keep_sum;
        //     return -2; 
        // }
    }
    return 0;
}

int main(void)
{
    /* TODO: 使用同一个数组的端点，测试完整/子/空/反向范围和 NULL。 */
    puts("Q4 unfinished: implement sum_adc_range and add your tests here.");
    return 0;
}
