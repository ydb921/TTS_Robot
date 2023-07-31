#include "../Bsp/Show_Class/oled.h"

typedef enum
{
    DisplayDebug,
    Display_1,
    Display_2,
    Display_Sum
} Display_t;

static Display_t DisplayStatus;

void Display(void)
{

    switch (DisplayStatus) {

        case DisplayDebug: {
            OLED_ShowString(0, 0, "J ");
            if (MotorCore.Actual_Angle.Y >= 0) {
                OLED_ShowNum(16, 0, (uint32_t) (MotorCore.Actual_Angle.Y), 3, 16);
                OLED_ShowString(40, 0, " ");
            }
            else {
                OLED_ShowString(16, 0, "-");
                OLED_ShowNum(24, 0, (uint32_t) (abs(MotorCore.Actual_Angle.Y)), 3, 16);
            }
            OLED_ShowString(0, 2, "X ");

            OLED_ShowString(0, 4, "C ");
            break;
        }
        case Display_1: { break; }
        case Display_2: { break; }
        case Display_Sum: { break; }
    }
}
