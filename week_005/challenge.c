#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

typedef struct {
    const uint16_t *cursor;
    size_t remaining;
} SampleReader;

int read_next_sample(void *context, const uint16_t **out_sample)
{
    /* TODO: 按 README 的挑战题实现。 */
    (void)context;
    (void)out_sample;
    return -99;
}

int main(void)
{
    /* TODO: 编写自测，覆盖正常、边界和错误情况。 */
    puts("W05 challenge unfinished: test a reader cursor and returned sample addresses.");
    return 0;
}
