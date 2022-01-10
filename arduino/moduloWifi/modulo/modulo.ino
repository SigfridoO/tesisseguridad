// https://esp8266-arduino-spanish.readthedocs.io/es/latest/esp8266wifi/readme.html

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

WiFiUDP Udp;

char* ssid = "ardilluda";
char* pass = "01971101";
unsigned int localPort = 1234;
char packetBuffer[255];
char  ReplyBuffer[] = "Enviando respuesta desde Arduino";       // a string to send back


void setup()
{
  int a = 0;
  Serial.begin(115200);
  Serial.println();

  WiFi.begin(ssid, pass);

  Serial.print("\nConectando");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  printWifiStatus();

  Serial.println("\nIniciando la conexión...");
  a = Udp.begin(localPort);
  Serial.print ("\n fin de begin");
  
}

void loop() {
  
    
  int packetSize = Udp.parsePacket();
  //Serial.print (packetSize);

  if (packetSize) {

    Serial.print("tamaño ");
    Serial.println(packetSize);
    Serial.print("Desde ");

    IPAddress remoteIp = Udp.remoteIP();

    Serial.print(remoteIp);
    Serial.print(", puerto ");
    Serial.println(Udp.remotePort());

    int len = Udp.read(packetBuffer, 255);

    if (len > 0) {
      packetBuffer[len] = 0;
    }

    Serial.println(">>");
    Serial.println(packetBuffer);

    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write(ReplyBuffer);
    Udp.endPacket();

  }  
}

void printWifiStatus() {
  Serial.println();
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  Serial.print("Conectado, dirección IP: ");
  Serial.println(WiFi.localIP());

  long rssi = WiFi.RSSI();
  Serial.print("intensidad de la señal (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
