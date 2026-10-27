#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

typedef void (*SampleCallback)(uint8_t channel, uint16_t adc, void *context);
typedef struct {
    SampleCallback callback;
    void *context;
} CallbackSlot;

int unregister_sample_callback(CallbackSlot *slot, SampleCallback callback)
{
    /* TODO: 按 README 的Q5实现。 */
    (void)slot;
    (void)callback;
    return -99;
}

int main(void)
{
    /* TODO: 编写自测，覆盖正常、边界和错误情况。 */
    puts("W06 Q5 unfinished: test matching, missing and NULL callbacks.");
    return 0;
}
