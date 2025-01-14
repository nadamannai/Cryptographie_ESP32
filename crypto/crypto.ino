#include <WiFi.h>
#include <WiFiUdp.h>
#include "mbedtls/aes.h"
#include <string>
#include <DHT.h>
 

// Ajouter les defines (MACROS)
#define dht_pin 14                // pin du capteur dht
#define DHTTYPE DHT11            // type du capteur dht
#define KEY "abcdeWpDWHTABCDE"  //  clé d'encryptage AES (16 bytes)

// Configuration Wi-Fi
const char* ssid = "Nada1";      // nom du wifi
const char* password = "Esprj1iot"; // mot de passe du wifi

// Configuration réseau
const char* targetIP = "192.168.43.114"; // Adresse IP de l'ESP32 récepteur
const int targetPort = 12345;           // port de l'esp32 recepteur

unsigned char iv[16] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                         0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F }; //clé IV ( for randomness)



float Temperature;
float Humidity;

WiFiUDP udp;  // objet UDP

DHT dht(dht_pin, DHTTYPE); // objet DHT

void setup() {

  Serial.begin(9600);
  WiFi.begin(ssid, password);

  // Connexion Wi-Fi
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnecté au Wi-Fi");
}

void loop() {

  // lire les valeurs de la température et de l'humidité
  Temperature = dht.readTemperature(); 
  Humidity = dht.readHumidity();
   if (isnan(Temperature) || isnan(Humidity)) 
    {
      Serial.println("Failed to read from DHT");
    }
 
  String dplainText = String(Humidity) + "%,"+ String(Temperature) + "°C"; // création d'un text dynamique
     
  const char* plainText = dplainText.c_str(); // convertir le type du text 
  //const char* plainText = "Message secret";
  size_t len = strlen(plainText);

  size_t paddedLen = ((len + 15) / 16) * 16;   // Alignement sur un multiple de 16 bytes pour AES

  //préparer un tableau de texte avec un remplissage (padding)
  unsigned char paddedText[paddedLen];
  memset(paddedText, 0, paddedLen);
  memcpy(paddedText, plainText, len);

  unsigned char encrypted[paddedLen]; // definition de la variable de l'encryptage
  mbedtls_aes_context aes;     // déclaration de la variable pour AES


  // initialisation AES
  mbedtls_aes_init(&aes);
  mbedtls_aes_setkey_enc(&aes, (const unsigned char*)KEY, 128);

  // chiffrement AES-CBC
  mbedtls_aes_crypt_cbc(&aes, MBEDTLS_AES_ENCRYPT, paddedLen, iv, paddedText, encrypted);
  mbedtls_aes_free(&aes);

  // envoi du message chiffré
  udp.beginPacket(targetIP, targetPort);
  udp.write(encrypted, paddedLen);
  udp.endPacket();

  Serial.println("Message chiffré envoyé !");
  delay(5000);
  
 
 
    
}
