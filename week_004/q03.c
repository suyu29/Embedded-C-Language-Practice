#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

typedef struct {
    uint8_t id;
    uint16_t adc;
    uint8_t status;
} SensorRecord;

int adjust_sensor_adc(SensorRecord *record, int16_t correction)
{
    /* TODO: 按 README 的Q3实现。 */
    (void)record;
    (void)correction;
    return -99;
}

int main(void)
{
    /* TODO: 编写自测，覆盖正常、边界和错误情况。 */
    puts("W04 Q3 unfinished: test signed corrections and ADC boundaries.");
    return 0;
}
