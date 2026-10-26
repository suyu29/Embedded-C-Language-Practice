#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include "com.h"

/* 题目提供的数据格式，不需要你猜测布局或填充字节。 */
typedef struct {
    uint8_t id;
    uint16_t adc;
    uint16_t millivolts;
    uint8_t status;
} SensorData;

// ID范围
#define ID_MIN              1
#define ID_MAX              3
#define ADC_MIN             0
#define ADC_MAX             4095
#define MILL_MIN             0
#define MILL_MAX             3300
#define STA_MIN             0
#define STA_MAX             3
int fill_sensor_data(SensorData *out, uint8_t id, uint16_t adc,
                     uint16_t millivolts, uint8_t status)
{
    /* TODO: 按 README.md 的 Q5 实现。 */
    (void)out;
    (void)id;
    (void)adc;
    (void)millivolts;
    (void)status;
    if(out == NULL_PTR)
    {
        return -1;
    }
    if((ID_MIN <= id) && (ID_MAX >= id) && (ADC_MIN <= adc) && (ADC_MAX >= adc) &&
            (MILL_MIN <= millivolts) && (MILL_MAX >= millivolts) && (STA_MIN <= status) && (STA_MAX >= status))
    {
        out->id = id;
        out->adc = adc;
        out->millivolts = millivolts;
        out->status = status;
        return 0;
    }
    else 
    {
        return -2;
    }
}

int main(void)
{
    /* TODO: 测试字段上下界、各字段越界、NULL 和失败时对象不变。 */
    puts("Q5 unfinished: implement fill_sensor_data and add your tests here.");
    return 0;
}
