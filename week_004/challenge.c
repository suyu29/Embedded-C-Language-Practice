#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

enum { EVENT_SAMPLE = 1, EVENT_ERROR = 2 };

typedef struct {
    uint8_t id;
    uint16_t last_adc;
    uint16_t error_count;
} DeviceState;

typedef struct {
    uint8_t type;
    union { uint16_t adc; uint8_t error_code; } data;
} DeviceEvent;

int apply_device_event(DeviceState *state, const DeviceEvent *event)
{
    /* TODO: 按 README 的挑战题实现。 */
    (void)state;
    (void)event;
    return -99;
}

int main(void)
{
    /* TODO: 编写自测，覆盖正常、边界和错误情况。 */
    puts("W04 challenge unfinished: test both event types and counter overflow.");
    return 0;
}
