#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

/* 每题独立编译，本文件自行提供数据格式。 */
typedef struct {
    uint8_t id;
    uint16_t adc;
    uint16_t millivolts;
    uint8_t status;
} SensorData;

int encode_sensor_frame(const SensorData *sensor, uint8_t *buffer,
                        size_t capacity, size_t *out_written)
{
    /* TODO: 按 README.md 的挑战题实现。 */
    (void)sensor;
    (void)buffer;
    (void)capacity;
    (void)out_written;
    return -99;
}

int main(void)
{
    /* TODO: 自测标准报文、字段边界、容量、NULL、哨兵及失败输出不变。 */
    puts("Challenge unfinished: implement encoding and add your tests here.");
    return 0;
}
