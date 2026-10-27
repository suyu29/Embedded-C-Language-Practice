#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

int max_from_channels(const uint16_t *const *channels, const size_t *lengths,
                      size_t channel_count, uint16_t *out_max)
{
    /* TODO: 按 README 的Q5实现。 */
    (void)channels;
    (void)lengths;
    (void)channel_count;
    (void)out_max;
    return -99;
}

int main(void)
{
    /* TODO: 编写自测，覆盖正常、边界和错误情况。 */
    puts("W05 Q5 unfinished: test multiple channel pointers and empty data.");
    return 0;
}
