#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#define NULL_PTR                NULL

int fill_adc_samples(uint16_t *dst, size_t count,
                     uint16_t start, uint16_t step)
{
    int ret = -1;
    int iloop;
    uint16_t tempVal;
    /* TODO: 按 README.md 的 Q3 实现。 */
    (void)dst;
    (void)count;
    (void)start;
    (void)step;
    if(dst != NULL_PTR)
    {
        for(iloop = 0; iloop < count; iloop++)
        {
            if(count == 1)
            {
                tempVal = start;
            }
            else
            {
                tempVal = start + iloop * step;
            }
                
            if((tempVal >= 0) && (tempVal <= 4095))
            {
                *(dst + iloop) = tempVal;
            }
            else 
            {
                ret = -2;
                break;
            }
        }
    }
    // `count == 0` 返回 `0`，允许 NULL
    if((count == 0) && (dst == NULL_PTR))
    {
        ret = 0;
    }
    else
    {
        ret = -1;
    }

    return ret;
}

int main(void)
{
    uint16_t dest[1024] = {0x00};
    /* TODO: 自测生成结果、空数组、NULL、越界及失败时数组不变。 */
    puts("Q3 unfinished: implement fill_adc_samples and add your tests here.");
    fill_adc_samples(dest, 2, 1, 3);
    fill_adc_samples(dest, 4, 100, 25);
    fill_adc_samples(dest, 0, 1, 3);
    return 0;
}
