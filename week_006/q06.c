#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

typedef void (*SampleCallback)(uint8_t channel, uint16_t adc, void *context);
typedef struct {
    SampleCallback callback;
    void *context;
} CallbackSlot;

int dispatch_sample_callback(const CallbackSlot *slot, uint8_t channel, uint16_t adc)
{
    /* TODO: 按 README 的Q6实现。 */
    (void)slot;
    (void)channel;
    (void)adc;
    return -99;
}

int main(void)
{
    /* TODO: 编写自测，覆盖正常、边界和错误情况。 */
    puts("W06 Q6 unfinished: test valid callbacks, empty slots and invalid data.");
    return 0;
}
