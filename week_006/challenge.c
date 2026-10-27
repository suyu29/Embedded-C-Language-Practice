#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#define HUB_CAPACITY 4

typedef void (*SampleCallback)(uint8_t channel, uint16_t adc, void *context);
typedef struct {
    SampleCallback callback;
    void *context;
} Listener;
typedef struct {
    Listener entries[HUB_CAPACITY];
    size_t count;
} EventHub;

int hub_register(EventHub *hub, SampleCallback callback, void *context)
{
    /* TODO: 按 README 的挑战题实现。 */
    (void)hub;
    (void)callback;
    (void)context;
    return -99;
}

int hub_unregister(EventHub *hub, SampleCallback callback, void *context)
{
    /* TODO: 按 README 的挑战题实现。 */
    (void)hub;
    (void)callback;
    (void)context;
    return -99;
}

int hub_publish(const EventHub *hub, uint8_t channel, uint16_t adc,
                size_t *out_called)
{
    /* TODO: 按 README 的挑战题实现。 */
    (void)hub;
    (void)channel;
    (void)adc;
    (void)out_called;
    return -99;
}

int main(void)
{
    /* TODO: 编写自测，覆盖正常、边界和错误情况。 */
    puts("W06 challenge unfinished: test registration, removal, capacity and callback order.");
    return 0;
}
