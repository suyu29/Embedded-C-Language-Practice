#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

int consume_uart_line(uint8_t *rx, size_t rx_capacity, size_t *rx_used,
                      char *out, size_t out_capacity, size_t *out_length)
{
    /* TODO: 按 README 的挑战题实现。 */
    (void)rx;
    (void)rx_capacity;
    (void)rx_used;
    (void)out;
    (void)out_capacity;
    (void)out_length;
    return -99;
}

int main(void)
{
    /* TODO: 编写自测，覆盖正常、边界和错误情况。 */
    puts("W03 challenge unfinished: test complete and incomplete UART lines.");
    return 0;
}
