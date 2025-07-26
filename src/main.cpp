// KODE ARDUINO BARU
#include "esp_camera.h"
#include "Arduino.h"
#include "WiFi.h"
#include "HTTPClient.h"
#include "base64.h" // Library untuk encoding

// Ganti dengan kredensial WiFi Anda
const char* ssid = "SABOSAPTE";
const char* password = "12345678";

// NANTI GANTI DENGAN URL DARI DEPLOYMENT BARU
String scriptURL = "https://script.google.com/macros/s/AKfycbyKBDdqSweZSknEr6jbdbjDhUd331L_vq-tsQks70DpHYff5P_EL5M5PyA_ZK332kA0/exec";

// Konfigurasi Pin untuk model AI-THINKER (tetap sama)
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

void setup() {
  Serial.begin(115200);
  // Inisialisasi kamera (kode setup tetap sama)
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
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size = FRAMESIZE_UXGA;
  config.jpeg_quality = 12;
  config.fb_count = 1;

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Inisialisasi kamera gagal dengan error 0x%x", err);
    return;
  }
  
  // Koneksi WiFi (kode setup tetap sama)
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nTerhubung ke WiFi!");
  Serial.print("Alamat IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  Serial.println("Mengambil gambar...");
  camera_fb_t * fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Gagal mengambil gambar dari kamera");
    return;
  }

  // Encode gambar ke Base64
  Serial.println("Mengubah gambar ke Base64...");
  String imageFile = base64::encode(fb->buf, fb->len);
  
  // Buat payload JSON
  String payload = "{\"imageData\":\"" + imageFile + "\"}";

  Serial.println("Mengunggah gambar ke Google Drive...");
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(scriptURL);
    // Set content type ke JSON
    http.addHeader("Content-Type", "application/json");

    int httpResponseCode = http.POST(payload);

    if (httpResponseCode > 0) {
      Serial.printf("Kode respons HTTP: %d\n", httpResponseCode);
      String response = http.getString();
      Serial.println("Respons dari server:");
      Serial.println(response);
    } else {
      Serial.printf("Gagal mengirim gambar, kode error: %s\n", http.errorToString(httpResponseCode).c_str());
    }
    http.end();
  }
  
  esp_camera_fb_return(fb);
  Serial.println("Upload selesai. Menunggu...");
  delay(10000);
}