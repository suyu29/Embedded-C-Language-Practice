#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

int main(void)
{
    uint16_t x = 10u;
    uint16_t y = 20u;
    const uint16_t *a = &x;
    uint16_t *const b = &x;
    const uint16_t *const c = &x;
    void *raw = &x;
    (void)y;
    (void)a;
    (void)b;
    (void)c;
    (void)raw;
    /* TODO: 在注释中分析 const，仅执行合法的验证语句。 */
    puts("W05 Q1 unfinished: answer the questions and add safe checks.");
    return 0;
}
