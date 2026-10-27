#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

/* TODO: 在注释中回答本周 README 的 Q1，解释数组参数和指向数组的指针。 */
static void inspect_samples(uint16_t samples[6], size_t count)
{
    /* TODO: 分析 sizeof(samples)、sizeof(*samples) 与 count 的含义。 */
    (void)samples;
    (void)count;
    // sizeof(samples)是表示测量数组的字节数量
    // sizeof(samples[0])表示观察指针类型的字节数
    // sizeof(samples)/sizeof(samples[0]) 数组元素个数
    // 字节数是Byte8位字节，元素数量是表示有多少个元素，最好的例程就是\
        uint16_t类型的数组 uint16_t buf[4]其元素数量是4，字节数量确实8

    // sizeof(samples)` 与 main 中同名表达式含义是否相同但是大小完全不同，\
        不可以自动获取，因为传递进的参数第六个元素
}

int main(void)
{
    uint16_t samples[6] = {100, 200, 300, 400, 500, 600};
    uint16_t *p = samples;
    uint16_t (*whole)[6] = &samples;
    // p是无符号16位指针， whole是指针数组，（*whole）是无符号16位数组\
        p+1跨2个字节，whole+1跨12个字节


    /* TODO: 添加合法的打印和调试观察，验证自己的分析。 */
    (void)p;
    (void)whole;
    inspect_samples(samples, 6);
    puts("W02 Q1 unfinished: explain array parameters and add your checks.");
    return 0;
}
