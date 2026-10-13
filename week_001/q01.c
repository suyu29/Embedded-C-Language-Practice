#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

/* TODO: 在这里逐项回答 README.md 的 Q1，不要运行非法指针表达式。 */

int main(void)
{
    /* TODO: 写入题目中的合法代码，打印结果或用 GDB 验证自己的推导。 */
    // adc[1]是一个数组， &adc[1]是一个指针， p也是一个指针，&p是一个指针的地址，*p是uint16_t类型
    // copy == 200u， 数组四个元素分别是100， 200 ，300，400 ； *p == 200u；copy获取的是值会跟着p变化，\
        \因为其获取的是这个指针指向的内存的值，也就是数组第二个元素的值
    puts("Q1 unfinished: answer the pointer questions and add your checks.");
    return 0;
}
