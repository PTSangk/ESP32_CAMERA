/*********
  Rui Santos
  Complete instructions at https://RandomNerdTutorials.com/esp32-cam-projects-ebook/

  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*********/

#include <WiFi.h>
#include "esp_camera.h"

//include system files
#include "fb_gfx.h"
#include "soc/soc.h"          // disable brownout problems
#include "soc/rtc_cntl_reg.h" // disable brownout problems
#include "driver/rtc_io.h"


#include "camera_pin.h"



// Replace with your network credentials
 const char *ssid = "TP-LINK_Test3";
 const char *password = "123456789!";



void startCameraServer();

void setup()
{
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); // disable brownout detector

  /**
    @Motor PIN Setup
  */
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(MTR_PWM, OUTPUT);
  pinMode(LEDStatus, OUTPUT);
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
  digitalWrite(LEDStatus, HIGH);   // turn the LED on (HIGH is the voltage level)
  /**
    @Camera config Setup
  */
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  if (psramFound())
  {
    config.frame_size = FRAMESIZE_VGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  }
  else
  {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }
  // Camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK)
  {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }




  /**
    @WiFi setup
  */
  // Wi-Fi connection
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    digitalWrite(LEDStatus, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(500);                       // wait for a 0.5 second
    digitalWrite(LEDStatus, LOW);    // turn the LED off by making the voltage LOW
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  Serial.print("Camera Stream Ready! Go to: http://");
  Serial.println(WiFi.localIP());

  /**
    @Start streaming web server
  */
  startCameraServer();

}




void loop()
{

}
