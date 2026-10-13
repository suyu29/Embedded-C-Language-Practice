#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

int write_be16_at(uint8_t *buffer, size_t length, size_t offset,
                  uint16_t value)
{
    /* TODO: 按 README.md 的 Q6 实现。 */
    (void)buffer;
    (void)length;
    (void)offset;
    (void)value;
    return -99;
}

int main(void)
{
    /* TODO: 自测大端字节、偏移边界、SIZE_MAX、NULL 和未目标字节不变。 */
    puts("Q6 unfinished: implement write_be16_at and add your tests here.");
    return 0;
}
