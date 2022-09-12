/**

  ESP32-CAM


  @
  @
*/

#include "esp_timer.h"
#include "img_converters.h"
#include "Arduino.h"
#include "esp_http_server.h"


#include "Webserver_Index.h"
#include "Speed_PWM.h"
#include "camera_pin.h"


static const char *_STREAM_CONTENT_TYPE = "multipart/x-mixed-replace;boundary=" PART_BOUNDARY;
static const char *_STREAM_BOUNDARY = "\r\n--" PART_BOUNDARY "\r\n";
static const char *_STREAM_PART = "Content-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n";

httpd_handle_t camera_httpd = NULL;
httpd_handle_t stream_httpd = NULL;


static esp_err_t index_handler(httpd_req_t *req)
{
  httpd_resp_set_type(req, "text/html");//API to set the HTTP content type.
  return httpd_resp_send(req, (const char *)INDEX_HTML, strlen(INDEX_HTML)); //API to send a complete HTTP response
}

static esp_err_t stream_handler(httpd_req_t *req)
{
  camera_fb_t *fb = NULL;
  esp_err_t res = ESP_OK;
  size_t _jpg_buf_len = 0;
  uint8_t *_jpg_buf = NULL;
  char *part_buf[64];

  res = httpd_resp_set_type(req, _STREAM_CONTENT_TYPE);
  if (res != ESP_OK)
  {
    return res;
  }

  while (true)
  {
    fb = esp_camera_fb_get();
    if (!fb)
    {
      Serial.println("Camera capture failed");
      res = ESP_FAIL;
    }
    else
    {
      if (fb->width > 400)
      {
        if (fb->format != PIXFORMAT_JPEG)
        {
          bool jpeg_converted = frame2jpg(fb, 80, &_jpg_buf, &_jpg_buf_len);
          esp_camera_fb_return(fb);
          fb = NULL;
          if (!jpeg_converted)
          {
            Serial.println("JPEG compression failed");
            res = ESP_FAIL;
          }
        }
        else
        {
          _jpg_buf_len = fb->len;
          _jpg_buf = fb->buf;
        }
      }
    }
    if (res == ESP_OK)
    {
      size_t hlen = snprintf((char *)part_buf, 64, _STREAM_PART, _jpg_buf_len);
      res = httpd_resp_send_chunk(req, (const char *)part_buf, hlen);
    }
    if (res == ESP_OK)
    {
      res = httpd_resp_send_chunk(req, (const char *)_jpg_buf, _jpg_buf_len);
    }
    if (res == ESP_OK)
    {
      res = httpd_resp_send_chunk(req, _STREAM_BOUNDARY, strlen(_STREAM_BOUNDARY));
    }
    if (fb)
    {
      esp_camera_fb_return(fb);
      fb = NULL;
      _jpg_buf = NULL;
    }
    else if (_jpg_buf)
    {
      free(_jpg_buf);
      _jpg_buf = NULL;
    }
    if (res != ESP_OK)
    {
      break;
    }
    // Serial.printf("MJPG: %uB\n",(uint32_t)(_jpg_buf_len));
  }
  return res;
}

static esp_err_t cmd_handler(httpd_req_t *req)
{
  char *buf;
  size_t buf_len;
  char variable[32] = {
    0,
  };
  char value[32] = {
    0,
  };

  buf_len = httpd_req_get_url_query_len(req) + 1; //Get Query string length from the request URL.
  if (buf_len > 1)
  {
    buf = (char *)malloc(buf_len);
    if (!buf)
    {
      httpd_resp_send_500(req);
      return ESP_FAIL;
    }
    if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK) //Get Query string from the request URL.
    {
      if (httpd_query_key_value(buf, "var", variable, sizeof(variable)) == ESP_OK &&
          httpd_query_key_value(buf, "val", value, sizeof(value)) == ESP_OK)
          //Helper function to get a URL query tag from a query string of the type param1=val1&param2=val2.
      {
      }
      else
      {
        free(buf);
        httpd_resp_send_404(req);
        return ESP_FAIL;
      }
    }
    else
    {
      free(buf);
      httpd_resp_send_404(req);
      return ESP_FAIL;
    }
    free(buf);
  }
  else
  {
    httpd_resp_send_404(req);
    return ESP_FAIL;
  }

  int val = atoi(value); // sang converts the string argument str to an integer

  /*********
     Start Change ESP32-CAM OV2640 Camera Settings
     HTML code for Rotage picture and filp image by setting
   **********/
  //sensor_t *s = esp_camera_sensor_get();
  //s->set_hmirror(s, 1);        // 0 = disable , 1 = enable Horizontal mirror

  /*
     End Change ESP32-CAM OV2640 Camera Settings
   *********/
  int res = 0;

  if (!strcmp(variable, "speed"))
  { // sang
    if (val > 255)
      val = 255;
    else if (val < 0)
      val = 0;
    speed = val;
    ledcWrite(0, speed);
    Serial.print("Speed: ");
    Serial.println(speed);
  }
  else if ((!strcmp(variable, "car")))
  {
    if (val == 1)
    {
      Serial.println("Forward");
      update_speed();
      digitalWrite(IN1, 0);
      digitalWrite(IN2, 1);
      digitalWrite(IN3, 0);
      digitalWrite(IN4, 1);
    }
    else if (val == 2)
    {
      Serial.println("Left"); // OUT3/4 Motor B
      update_speed();
      digitalWrite(IN1, 0);
      digitalWrite(IN2, 1);
      digitalWrite(IN3, 0);
      digitalWrite(IN4, 0);
    }
    else if (val == 3)
    {
      Serial.println("Right");  // OUT1/2 Motor A
      update_speed();
      digitalWrite(IN1, 0);
      digitalWrite(IN2, 0);
      digitalWrite(IN3, 0);
      digitalWrite(IN4, 1);

    }
    else if (val == 4)
    {
      Serial.println("Backward");
      update_speed();
      digitalWrite(IN1, 1);
      digitalWrite(IN2, 0);
      digitalWrite(IN3, 1);
      digitalWrite(IN4, 0);
    }
    else if (val == 5)
    {
      Serial.println("Stop");
      digitalWrite(IN1, 0);
      digitalWrite(IN2, 0);
      digitalWrite(IN3, 0);
      digitalWrite(IN4, 0);
    }
  }
  else
  {
    res = -1;
  }

  if (res)
  {
    return httpd_resp_send_500(req);
  }

  httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
  return httpd_resp_send(req, NULL, 0);
}

void startCameraServer()
{
  httpd_config_t config = HTTPD_DEFAULT_CONFIG();
  config.server_port = 80;
  httpd_uri_t index_uri = {
    .uri = "/",
    .method = HTTP_GET,
    .handler = index_handler,
    .user_ctx = NULL
  };

  httpd_uri_t cmd_uri = {
    .uri = "/action",
    .method = HTTP_GET,
    .handler = cmd_handler,
    .user_ctx = NULL
  };
  httpd_uri_t stream_uri = {
    .uri = "/stream",
    .method = HTTP_GET,
    .handler = stream_handler,
    .user_ctx = NULL
  };


  if (httpd_start(&camera_httpd, &config) == ESP_OK)
  {
    httpd_register_uri_handler(camera_httpd, &index_uri);
    httpd_register_uri_handler(camera_httpd, &cmd_uri);
  }
  config.server_port += 1;
  config.ctrl_port += 1;
  if (httpd_start(&stream_httpd, &config) == ESP_OK)
  {
    httpd_register_uri_handler(stream_httpd, &stream_uri);
  }
}
