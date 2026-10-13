#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

int read_sensor(uint8_t sensor_id, uint16_t *out_adc)
{
    /* TODO: 按 README.md 的 Q2 实现；以下只是避免未使用参数警告。 */
    (void)sensor_id;
    (void)out_adc;
    return -99;
}

int main(void)
{
    /* TODO: 调用 read_sensor，自己检查有效 ID、非法 ID、NULL 和哨兵。 */
    puts("Q2 unfinished: implement read_sensor and add your tests here.");
    return 0;
}
