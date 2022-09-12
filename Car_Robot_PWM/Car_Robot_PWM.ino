/*********

*********/


#include "esp_camera.h"

#define MTR_PWM 15
#define MOTOR_1_PIN_1 14
#define MOTOR_1_PIN_2 2
#define MOTOR_2_PIN_1 13
#define MOTOR_2_PIN_2 12

// Setting PWM properties
#define freq 30000
#define pwmChannel 0
#define resolution 8
int dutyCycle = 200;

void setup()
{
  /**
    @Motor PIN Setup
  */
  pinMode(MOTOR_1_PIN_1, OUTPUT);
  pinMode(MOTOR_1_PIN_2, OUTPUT);
  pinMode(MOTOR_2_PIN_1, OUTPUT);
  pinMode(MOTOR_2_PIN_2, OUTPUT);
  pinMode(MTR_PWM, OUTPUT);

  /**
    @PWM Setup
  */
  // Motor uses PWM Channel 8
  // ledcSetup(ledChannel, freq, resolution);
  ledcSetup(pwmChannel, freq, resolution);
  // ledcAttachPin(ledPin, ledChannel);
  ledcAttachPin(MTR_PWM, pwmChannel);
  // ledcWrite(ledChannel, dutyCycle);
  // ledcWrite(0, 200);

  /**
    @Serial Setup
  */
  Serial.begin(115200);
  // testing
  Serial.print("Testing DC Motor...");
}

void loop()
{

  // Move the DC motor forward at maximum speed
  Serial.println("Moving Forward");
  digitalWrite(MOTOR_1_PIN_1, LOW);
  digitalWrite(MOTOR_1_PIN_2, HIGH);
  delay(2000);

  // Stop the DC motor
  Serial.println("Motor stopped");
  digitalWrite(MOTOR_1_PIN_1, LOW);
  digitalWrite(MOTOR_1_PIN_2, LOW);
  delay(1000);

  // Move DC motor backwards at maximum speed
  Serial.println("Moving Backwards");
  digitalWrite(MOTOR_1_PIN_1, HIGH);
  digitalWrite(MOTOR_1_PIN_2, LOW);
  delay(2000);

  // Stop the DC motor
  Serial.println("Motor stopped");
  digitalWrite(MOTOR_1_PIN_1, LOW);
  digitalWrite(MOTOR_1_PIN_2, LOW);
  delay(1000);

  // Move DC motor forward with increasing speed
  digitalWrite(MOTOR_1_PIN_1, HIGH);
  digitalWrite(MOTOR_1_PIN_2, LOW);
  while (dutyCycle <= 255) {
    ledcWrite(pwmChannel, dutyCycle);
    Serial.print("Forward with duty cycle: ");
    Serial.println(dutyCycle);
    dutyCycle = dutyCycle + 5;
    delay(500);
  }
  dutyCycle = 200;
}
