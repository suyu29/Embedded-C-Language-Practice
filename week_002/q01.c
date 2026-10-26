#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

/* TODO: 在注释中回答本周 README 的 Q1，解释数组参数和指向数组的指针。 */
static void inspect_samples(uint16_t samples[6], size_t count)
{
    /* TODO: 分析 sizeof(samples)、sizeof(*samples) 与 count 的含义。 */
    (void)samples;
    (void)count;
}

int main(void)
{
    uint16_t samples[6] = {100, 200, 300, 400, 500, 600};
    uint16_t *p = samples;
    uint16_t (*whole)[6] = &samples;

    /* TODO: 添加合法的打印和调试观察，验证自己的分析。 */
    (void)p;
    (void)whole;
    inspect_samples(samples, 6);
    puts("W02 Q1 unfinished: explain array parameters and add your checks.");
    return 0;
}
