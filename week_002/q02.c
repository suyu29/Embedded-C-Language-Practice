#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include "com.h"

int copy_adc_slice(const uint16_t *src, size_t src_count,
                   size_t offset, size_t count,
                   uint16_t *dst, size_t dst_capacity, size_t *out_count)
{
    /* TODO: 按 README 的 Q2 实现，失败时保留目标数组和输出数量。 */
    (void)src;
    (void)src_count;
    (void)offset;
    (void)count;
    (void)dst;
    (void)dst_capacity;
    (void)out_count;
    uint8_t retVal;

    if((src != NULL_PTR) && (dst != NULL_PTR) && (out_count != NULL_PTR))
    {
        // 确保有足够的位置写入
        if((src_count - count) > offset)
        {

        }
        else
        {
            
        }
    }
    {
       retVal = -1; 
    }
    return retVal;
}

int main(void)
{
    /* TODO: 自测切片、空操作、索引与容量边界、NULL、哨兵和错误优先级。 */
    puts("W02 Q2 unfinished: implement copy_adc_slice and add your tests.");
    return 0;
}
