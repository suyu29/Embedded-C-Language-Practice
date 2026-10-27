#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

typedef struct {
    uint8_t channel;
    uint16_t adc;
} SensorContext;

int read_context_adc(const void *context, uint8_t requested_channel,
                     uint16_t *out_adc)
{
    /* TODO: 按 README 的Q4实现。 */
    (void)context;
    (void)requested_channel;
    (void)out_adc;
    return -99;
}

int main(void)
{
    /* TODO: 编写自测，覆盖正常、边界和错误情况。 */
    puts("W05 Q4 unfinished: test context type, channel and NULL cases.");
    return 0;
}
