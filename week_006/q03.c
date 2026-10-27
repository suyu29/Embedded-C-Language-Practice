#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

typedef void (*SampleCallback)(uint8_t channel, uint16_t adc, void *context);

int notify_sample(SampleCallback callback, void *context,
                  uint8_t channel, uint16_t adc)
{
    /* TODO: 按 README 的Q3实现。 */
    (void)callback;
    (void)context;
    (void)channel;
    (void)adc;
    return -99;
}

int main(void)
{
    /* TODO: 编写自测，覆盖正常、边界和错误情况。 */
    puts("W06 Q3 unfinished: test callback arguments and invocation count.");
    return 0;
}
