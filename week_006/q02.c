#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

typedef uint16_t (*TransformFn)(uint16_t value);

int call_transform(uint16_t input, TransformFn transform, uint16_t *out_value)
{
    /* TODO: 按 README 的Q2实现。 */
    (void)input;
    (void)transform;
    (void)out_value;
    return -99;
}

int main(void)
{
    /* TODO: 编写自测，覆盖正常、边界和错误情况。 */
    puts("W06 Q2 unfinished: test a matching callback and NULL cases.");
    return 0;
}
