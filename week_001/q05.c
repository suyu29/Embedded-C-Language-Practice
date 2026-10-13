#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

/* 题目提供的数据格式，不需要你猜测布局或填充字节。 */
typedef struct {
    uint8_t id;
    uint16_t adc;
    uint16_t millivolts;
    uint8_t status;
} SensorData;

int fill_sensor_data(SensorData *out, uint8_t id, uint16_t adc,
                     uint16_t millivolts, uint8_t status)
{
    /* TODO: 按 README.md 的 Q5 实现。 */
    (void)out;
    (void)id;
    (void)adc;
    (void)millivolts;
    (void)status;
    return -99;
}

int main(void)
{
    /* TODO: 测试字段上下界、各字段越界、NULL 和失败时对象不变。 */
    puts("Q5 unfinished: implement fill_sensor_data and add your tests here.");
    return 0;
}
