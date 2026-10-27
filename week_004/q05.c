#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

typedef struct {
    uint8_t id;
    uint32_t tick;
    uint16_t adc;
} DeviceRecord;

typedef struct {
    size_t size;
    size_t alignment;
    size_t id_offset;
    size_t tick_offset;
    size_t adc_offset;
    size_t padding_bytes;
} RecordLayout;

int describe_record_layout(RecordLayout *out_layout)
{
    /* TODO: 按 README 的Q5实现。 */
    (void)out_layout;
    return -99;
}

int main(void)
{
    /* TODO: 编写自测，覆盖正常、边界和错误情况。 */
    puts("W04 Q5 unfinished: test actual layout and NULL output.");
    return 0;
}
