#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

typedef struct {
    uint8_t id;
    uint32_t tick;
    uint16_t adc;
} DeviceRecord;

int record_fields_equal(const DeviceRecord *a, const DeviceRecord *b,
                        uint8_t *out_equal)
{
    /* TODO: 按 README 的Q6实现。 */
    (void)a;
    (void)b;
    (void)out_equal;
    return -99;
}

int main(void)
{
    /* TODO: 编写自测，覆盖正常、边界和错误情况。 */
    puts("W04 Q6 unfinished: test individual fields and NULL pointers.");
    return 0;
}
