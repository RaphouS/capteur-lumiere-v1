//---------------------------------------------
// com_esp_now.h pour Rideaux-V2
//---------------------------------------------
#ifndef COM_ESP_NOW_H
#define COM_ESP_NOW_H
#include <esp_now.h>
//#include <WiFi.h>

// MAC address esp32s3 moteurs : 34:85:18:4A:FE:58 maison
// MAC address esp32s3 moteurs : 34:85:18:4A:37:3C bureau
// MAC Address esp32s3 telecommande : 3C:84:27:C0:F3:14 maison
// MAC Address esp32s3 telecommande : 3C:84:27:C0:E7:F4 bureau
// MAC Address esp32c3 supermini : 98:3D:AE:52:85:2C

//---------------------------------------------
// ******* congiguration de ESP-NOW ********

// structure exemple des données envoyées
typedef struct soleil_rideaux{
  long timestamp;
  float luminosite; // en lux
} soleil_rideaux;


void OnDataReceived(const uint8_t * mac, const uint8_t *incomingData, int len);
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);


#endif

