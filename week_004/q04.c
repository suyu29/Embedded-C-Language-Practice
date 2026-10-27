#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

typedef struct {
    uint8_t mode;
    union {
        uint16_t fixed;
        struct { uint16_t low; uint16_t high; } band;
    } limit;
} AlertConfig;

int evaluate_alert(const AlertConfig *config, uint16_t value, uint8_t *out_alarm)
{
    /* TODO: 按 README 的Q4实现。 */
    (void)config;
    (void)value;
    (void)out_alarm;
    return -99;
}

int main(void)
{
    /* TODO: 编写自测，覆盖正常、边界和错误情况。 */
    puts("W04 Q4 unfinished: test both union modes and invalid ranges.");
    return 0;
}
