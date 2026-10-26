#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include "com.h"

#define ENCODE_LEN       2
#define ByteNum     8

int write_be16_at(uint8_t *buffer, size_t length, size_t offset,
                  uint16_t value)
{
    /* TODO: 按 README.md 的 Q6 实现。 */
    (void)buffer;
    (void)length;
    (void)offset;
    (void)value;
    uint16_t tempNum;
    if(buffer != NULL)
    {
         /* 检查偏移与剩余空间 */
        if ((offset > length) || ((length - offset) < ENCODE_LEN))
        {
            return -3;
        }
        buffer[offset] = (value >> ByteNum) & 0xFF;
        buffer[offset+1] = (uint8_t)value;
    }
    else 
    {
        return -1;
    }
    return 0;
}

int main(void)
{
    /* TODO: 自测大端字节、偏移边界、SIZE_MAX、NULL 和未目标字节不变。 */
    puts("Q6 unfinished: implement write_be16_at and add your tests here.");
    return 0;
}
