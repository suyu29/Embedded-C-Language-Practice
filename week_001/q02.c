#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

#define NULL_PTR                NULL

int read_sensor(uint8_t sensor_id, uint16_t *out_adc)
{
    int8_t retVal = 0;
    /* TODO: 按 README.md 的 Q2 实现；以下只是避免未使用参数警告。 */
    (void)sensor_id;
    if(out_adc != NULL_PTR)
    {
        if((sensor_id == 0x1U) || (sensor_id == 0x2U) || (sensor_id == 0x3U))
        {
            switch(sensor_id)
            {
                case 1:
                    *out_adc = 0;
                break;
                case 2:
                    *out_adc = 2048;
                break;
                case 3:
                    *out_adc = 4095;
                break;
            }
        }
        else 
        {
            retVal = -2;
        }
    }
    else 
    {
        retVal = -1;
    }
    return retVal;
}

int main(void)
{   
    static uint16_t adc_out = 0x123;
    uint16_t adc_buf[0x3] = {0x00};
    int ret;
    /* TODO: 调用 read_sensor，自己检查有效 ID、非法 ID、NULL 和哨兵。 */
    puts("Q2 unfinished: implement read_sensor and add your tests here.");
    ret = read_sensor(0x4U, &adc_out);
    printf("ret val:%d", ret);
    return 0;
}
