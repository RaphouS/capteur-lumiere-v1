//---------------------------------------------
// com_esp_now.cpp pour Rideaux-V2
//---------------------------------------------

#include <Arduino.h>
#include <esp_now.h>
#include <cstring>
#include "com_esp_now.h"

soleil_rideaux soleil ;

uint8_t senderMac[6];
//adresse MAC de la maison
//uint8_t macMoteur[] = {0x34, 0x85, 0x18, 0x4A, 0xFE, 0x58}; // Moteurs maison
//uint8_t macTelecommande[] = {0x3C, 0x84, 0x27, 0xC0, 0xF3, 0x14}; // Télécommande maison 

// adresse MAC du bureau
//uint8_t macMoteur[] = {0x34, 0x85, 0x18, 0x4A, 0x37, 0x3C}; // Moteurs bureau
//uint8_t macTelecommande[] = {0x3C, 0x84, 0x27, 0xC0, 0xE7, 0xF4}; // Télécommande bureau
//uint8_t macSupermini[] = {0x98, 0x3D, 0xAE, 0x52, 0x85, 0x2C}; // Supermini

// adresse MAC pour les test IUT
uint8_t macMoteur[] = {0x34, 0x85, 0x18, 0x4A, 0x37, 0x3C}; // Moteurs bureau
// mac de la télécommande iut : 2cbcbbb5bd84
uint8_t macTelecommande[] = {0x2C, 0xBC, 0xBB, 0xB5, 0xBD, 0x84}; // Telecommande IUT
// mac supermini-iut : 50787d8fe580
uint8_t macSupermini[] = {0x50, 0x78, 0x7D, 0x8F, 0xE5, 0x80};


void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Last Packet Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

/*
// fonction de rappel appelée lors de la réception des données
void OnDataReceived(const uint8_t * mac, const uint8_t *incomingData, int len) 
{

  char macStr[18];
  Serial.print("Bytes received from: ");
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  Serial.println(macStr);
  memcpy(senderMac, mac, 6);

  if(memcmp(senderMac, macTelecommande, 6) == 0) 
  { // adresse MAC de la télécommande
    memcpy(&mvt_rideaux, incomingData, sizeof(mvt_rideaux));
    Serial.print("Rideau actif: ");
    Serial.println(mvt_rideaux.rideau_actif);
    Serial.print("Action: ");
    Serial.println(mvt_rideaux.action);
    Serial.print("Position rideau droit: ");
    Serial.println(mvt_rideaux.position_rideau_droit);
    Serial.print("Position rideau centre: ");
    Serial.println(mvt_rideaux.position_rideau_centre);
    Serial.print("Position rideau gauche: ");
    Serial.println(mvt_rideaux.position_rideau_gauche);
    Serial.print("Orientation lame rideau droit: ");
    Serial.println(mvt_rideaux.orientation_rideau_droit);
    Serial.print("Orientation lame rideau centre: ");
    Serial.println(mvt_rideaux.orientation_rideau_centre);
    Serial.print("Orientation lame rideau gauche: ");
    Serial.println(mvt_rideaux.orientation_rideau_gauche);

  }

  if(memcmp(senderMac, macMoteur, 6) == 0) 
  { // adresse MAC de l'ESP32 des moteurs
    memcpy(&pos_rideaux, incomingData, sizeof(pos_rideaux));
    Serial.print("Position rideau droit: ");
    Serial.println(pos_rideaux.position_rideau_droit);
    Serial.print("Position rideau centre: ");
    Serial.println(pos_rideaux.position_rideau_centre);
    Serial.print("Position rideau gauche: ");
    Serial.println(pos_rideaux.position_rideau_gauche);
    Serial.print("Orientation lame rideau droit: ");
    Serial.println(pos_rideaux.orientation_rideau_droit);
    Serial.print("Orientation lame rideau centre: ");
    Serial.println(pos_rideaux.orientation_rideau_centre);
    Serial.print("Orientation lame rideau gauche: ");
    Serial.println(pos_rideaux.orientation_rideau_gauche);
  }
}   
*/