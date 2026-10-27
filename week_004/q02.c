#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

typedef struct {
    uint8_t id;
    uint16_t adc;
    uint8_t valid;
} SensorReading;

int max_valid_reading(const SensorReading *records, size_t count,
                      uint16_t *out_adc, size_t *out_index)
{
    /* TODO: 按 README 的Q2实现。 */
    (void)records;
    (void)count;
    (void)out_adc;
    (void)out_index;
    return -99;
}

int main(void)
{
    /* TODO: 编写自测，覆盖正常、边界和错误情况。 */
    puts("W04 Q2 unfinished: test valid records, ties and invalid flags.");
    return 0;
}
