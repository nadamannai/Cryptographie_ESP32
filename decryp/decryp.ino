#include <WiFi.h>
#include <WiFiUdp.h>
#include "mbedtls/aes.h"


#define KEY "abcdeWpDWHTABCDE" //  clé d'encryptage AES (16 bytes)



// Configuration Wi-Fi
const char* ssid = "Nada1";
const char* password = "Esprj1iot";

// Configuration réseau
const int localPort = 12345;


unsigned char iv[16] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                         0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F }; //clé IV 

WiFiUDP udp; // objet UDP


void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);

  // Connexion Wi-Fi
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnecté au Wi-Fi");

  udp.begin(localPort);
  Serial.println("Récepteur UDP prêt");
}

void loop() {
  int packetSize = udp.parsePacket(); // détecter et récupérer la taille d'un paquet UDP entrant
  if (packetSize) {
    unsigned char encrypted[packetSize];
    udp.read(encrypted, packetSize);  // lire le message encrypté envoyé
    Serial.print("Message chiffré reçu : ");
    for (int i = 0; i < packetSize; i++) {
      Serial.printf("%02X ", encrypted[i]); // afficher chaque octet en hexadécimal
    }
    Serial.println();

    unsigned char decrypted[packetSize]; // definition de la variable du décryptage 
    mbedtls_aes_context aes; // déclaration de la variable pour AES

    // Initialisation AES
    mbedtls_aes_init(&aes);
    mbedtls_aes_setkey_dec(&aes,(const unsigned char*) KEY, 128);

    // Déchiffrement AES-CBC
    mbedtls_aes_crypt_cbc(&aes, MBEDTLS_AES_DECRYPT, packetSize, iv, encrypted, decrypted);
    mbedtls_aes_free(&aes);
 
    // Détectecter la fin du message (string)
    decrypted[16] = '\0';
    
    // Afficher le message déchiffré
    Serial.print("Message déchiffré : ");
    Serial.println((char*)decrypted);
  }
  
}