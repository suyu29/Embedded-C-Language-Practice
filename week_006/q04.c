#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

typedef void (*SampleCallback)(uint8_t channel, uint16_t adc, void *context);
typedef struct {
    SampleCallback callback;
    void *context;
} CallbackSlot;

int register_sample_callback(CallbackSlot *slot, SampleCallback callback, void *context)
{
    /* TODO: 按 README 的Q4实现。 */
    (void)slot;
    (void)callback;
    (void)context;
    return -99;
}

int main(void)
{
    /* TODO: 编写自测，覆盖正常、边界和错误情况。 */
    puts("W06 Q4 unfinished: test empty and occupied slots.");
    return 0;
}
