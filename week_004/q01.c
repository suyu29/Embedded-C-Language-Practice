#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

typedef struct {
    uint8_t id;
    uint32_t tick;
    uint16_t adc;
} DeviceRecord;
typedef union {
    uint32_t word;
    uint8_t bytes[4];
} WordView;

int main(void)
{
    DeviceRecord record = {1u, 1000u, 500u};
    WordView view = {.word = 0x12345678u};
    (void)record;
    (void)view;
    /* TODO: 按 README 的 Q1 分析并观察类型布局。 */
    puts("W04 Q1 unfinished: answer the questions and inspect the layout.");
    return 0;
}
