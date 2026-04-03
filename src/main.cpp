#include <Arduino.h>
#include <WiFi.h>
#include <time.h>
#include <DFRobot_B_LUX_V30B.h>
#include <Wire.h>
#include "help.h"
#include "com_esp_now.h"

// Définition de la règle de fuseau horaire (Exemple pour la France/Paris)
// CET = Central European Time
// CEST = Central European Summer Time
const char* TZ_INFO = "CET-1CEST,M3.5.0,M10.5.0/3";




// Définition de la broche de la LED pour l'ESP32-C3 SuperMini
const int LED_PIN = 8;
#define I2C_SCL_PIN  9
#define I2C_SDA_PIN  8
#define EN_PIN       10


String macAddress;

DFRobot_B_LUX_V30B luxSensor(EN_PIN, I2C_SCL_PIN, I2C_SDA_PIN);

//-----------------------------ESP-NOW-----------------------------
extern uint8_t macTelecommande[];
extern soleil_rideaux soleil;
esp_now_peer_info_t peerInfo;
//---------------------------------------------------------------


void setup() 
{
  Serial.begin(115200); // Initialise la communication série à 115200 bauds
  int net_flag=0;

  
  pinMode(LED_PIN, OUTPUT);
  pinMode(EN_PIN, OUTPUT);
  digitalWrite(EN_PIN, HIGH); // Active le capteur de luminosité (EN_PIN à HIGH)

  //Initialisation de la communication avec le capteur de luminosité
  Wire.begin();
  luxSensor.begin();
  
  Serial.println("Setup complete. Starting LED blink...");
  //macAddress = getMacAddress(); // Appel de la fonction pour récupérer l'adresse MAC

// initialisation ESP-NOW

  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    while(1);
  }

  //registre les fonctions de rappel
  esp_now_register_send_cb(OnDataSent);

  //ajoute le pair
  memcpy(peerInfo.peer_addr, macTelecommande, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  Serial.println("ESP-NOW initialized.");
  soleil.timestamp = 0;
  soleil.luminosite = 0.0f;

  //configuration du fuseau horaire
  setenv("TZ", TZ_INFO, 1);
  tzset();
/*
  while(net_flag==0)
  {
    if(Serial)
    {
      String input = Serial.readStringUntil('\n');
      if (input.startsWith("T")) 
      {
        long pctime = input.substring(1).toInt();
        if (pctime > 1000000) 
        {
          struct timeval tv;
          tv.tv_sec = pctime;
          tv.tv_usec = 0;
          settimeofday(&tv, NULL);
          
          // On renvoie la confirmation au PC immédiatement
          //Serial.println("Synchronisation réussie !");
          
          digitalWrite(LED_PIN, LOW); // Allumer la LED pour indiquer le succès
          net_flag = 1;
          //soleil.timestamp = pctime;
          Serial.print("Timestamp configuré : ");

          // On envoie aussi l'heure calculée pour vérification visuelle côté Python
          struct tm timeinfo;
          if (getLocalTime(&timeinfo)) 
          {
            Serial.print("Heure configurée : ");
            Serial.println(&timeinfo, "%H:%M:%S");
          }
        }
      }
      delay(50);
    }
  }
*/
}

void loop() 
{
  // Régleage de l'horloge et obtention du timestamp actuel
  //if (Serial.available() >0 && net_flag==0) 
  {
    String input = Serial.readStringUntil('\n');
    if (input.startsWith("T")) 
    {
      long pctime = input.substring(1).toInt();
      //Serial.print("Timestamp reçu : ");
      //Serial.println(pctime);
      
      if (pctime > 1000000) 
      {
        struct timeval tv;
        tv.tv_sec = pctime;
        tv.tv_usec = 0;
        settimeofday(&tv, NULL);
        
        // On renvoie la confirmation au PC immédiatement
        Serial.println("Synchronisation réussie !");
        soleil.timestamp = pctime;
        //Serial.print("Timestamp configuré : ");
        
        // On envoie aussi l'heure calculée pour vérification visuelle côté Python
        struct tm timeinfo;
        if (getLocalTime(&timeinfo)) {
          Serial.print("Heure configurée : ");
          Serial.println(&timeinfo, "%H:%M:%S");
        }
      }
    }
  }


  float lux = luxSensor.lightStrengthLux();
  digitalWrite(LED_PIN, LOW);
  delay(500); // Attend 500 ms
  digitalWrite(LED_PIN, HIGH);
  delay(500); // Attend 500 ms

  soleil.luminosite = lux;
  soleil.timestamp = time(NULL);
  Serial.print("Timestamp : ");
  Serial.println(soleil.timestamp);
  Serial.print("Lux: ");
  Serial.println(lux);
  
  //envoie des données
  esp_err_t result = esp_now_send(macTelecommande, (uint8_t *) &soleil, sizeof(soleil));

  delay(5000); // Attend 5 secondes avant la prochaine lecture


}