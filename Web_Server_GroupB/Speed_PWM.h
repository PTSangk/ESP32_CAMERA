/**

  Speed & PWM


  @
  @
*/

#include "camera_pin.h"


/**
  @Speed & PWM
*/

// Define Speed variables
int speed = 200;


unsigned int get_speed(unsigned int sp)
{
  return map(sp, 0, 100, 0, 255);
}


void update_speed()
{
  ledcWrite(pwmChannel, get_speed(speed));
  // Serial.print("Speed: ");
  // Serial.println(speed);
}
