// Tesis Sistema de Seguridad
// 13-marzo-2022

#include <Arduino.h>

#include <WiFi.h>
#include <WiFiMulti.h>
#include <WiFiClientSecure.h>

#include <WebSocketsClient.h>

WiFiMulti WiFiMulti;
WebSocketsClient webSocket;

#define SERIAL_BASE Serial
#define USE_SERIAL Serial2

//////////////////////////////////// Parámetros de configuración

const char* ssid = "ardilluda";
const char* pass = "01971101";

const char* direccion = "157.230.190.233";
const int puerto = 8000;
const char* chat = "/ws/chat/python/";
const String nombreCliente =  "arduino";

//////////////////////////////////// Comunicación serie

#define TAMANIO_BUFFER 256
byte bufferLectura[TAMANIO_BUFFER];
int indiceBuffer = 0;

char bufferLecturaWebsocket[TAMANIO_BUFFER];

char caracterInicio = '#';
char caracterFinal = '&';

void almacenarPuertoSerie2 ();
void leerPuertoSerie2();
String extraerInstruccion(byte*, int, int);
void construirMensaje(String);



//////////////////////////////////// Variables para comunicación

unsigned long contador = 0;
char mensajeAEnviar[150];

//////////////////////////////////// Temporizadores

#define  numeroDeTON 16
struct temporizador {
    byte entrada;
    byte salida;
    byte reset;
    unsigned long tiempo;
    unsigned long tiempoActual;
} TON[numeroDeTON];
struct temporizadorAux {
    byte bandera;
    unsigned long tiempo_Aux1;
    unsigned long tiempo_Aux2;
} TON_Aux[numeroDeTON];

void actualizarTON (int);
void configurar (void);








void setup() {


  TON[0].tiempo = (unsigned long)1 * 5000;




  SERIAL_BASE.begin(115200);
  USE_SERIAL.begin(115200);
  
  USE_SERIAL.setDebugOutput(true);

  USE_SERIAL.println();
  USE_SERIAL.println();
  USE_SERIAL.println();

  for(uint8_t t = 4; t > 0; t--) {
    USE_SERIAL.printf("[SETUP] BOOT WAIT %d...\n", t);
    USE_SERIAL.flush();

    SERIAL_BASE.printf("[SETUP] BOOT WAIT %d...\n", t);
    SERIAL_BASE.flush();

    delay(1000);
  }

  WiFiMulti.addAP(ssid, pass);

  //WiFi.disconnect();
  while(WiFiMulti.run() != WL_CONNECTED) {
    delay(100);
  }

  
  // comunicación con el websocket
  webSocket.begin(direccion, puerto, chat);
  // manejo de eventos
  webSocket.onEvent(webSocketEvent);

  // use HTTP Basic Authorization this is optional remove if not needed
  //webSocket.setAuthorization("user", "Password");

  // nuevo intento si la conexión ha fallado
  webSocket.setReconnectInterval(5000);
}











void loop() {

  //Serial.println ("probando websocket");
  ;
  webSocket.loop();
  //Serial.println ("Dentro de un ciclo");



    
    TON[0].entrada = !TON[0].salida;
    actualizarTON(0);

    if (TON[0].salida) {
        contador++;
        // mensajeAEnviar = "{\"type\": \"chat.message\", \"name\": \"python\", \"text\": \"hola desde arduino %d  \" } ",contador;

        // Serial.print (mensajeAEnviar);
        // sprintf(mensajeAEnviar, "{\"type\": \"chat.message\", \"name\": \"python\", \"text\": \"hola desde arduino %d  \" } ",contador);

        // webSocket.sendTXT(mensajeAEnviar);
    }


     
/////////////////////Codigo para comunicación/////////////////////////
  
    almacenarPuertoSerie2();
    leerPuertoSerie2();


}







void almacenarPuertoSerie2 () {

  while(Serial2.available()) {
      byte caracterLeido = Serial2.read();
      //Serial.write (caracterLeido);
      bufferLectura[indiceBuffer++] =  caracterLeido;

      if (indiceBuffer > int( TAMANIO_BUFFER-10)){
        indiceBuffer = 0;
      }
  }
}




void leerPuertoSerie2() {

    //char caracterLeido;

    boolean inicioDeInstruccionEncontrado = false;
    int posicionInicio= 0;
    int posicionFinal = 0;
    byte *cadena;
    String instruccionDetectada = "";

    if (indiceBuffer > 0) {
        for (int i = 0; i < indiceBuffer; i++) {

            cadena += (char) bufferLectura[i];

            if (bufferLectura[i] == caracterInicio ) {
              inicioDeInstruccionEncontrado = true;
              posicionInicio = i;
            }
    
            if (inicioDeInstruccionEncontrado && (bufferLectura[i] == caracterFinal) ) {
                posicionFinal = i;
                instruccionDetectada = extraerInstruccion(bufferLectura, posicionInicio, posicionFinal);
                construirMensaje(instruccionDetectada);
                webSocket.sendTXT(mensajeAEnviar);
                indiceBuffer = 0;
            }
        }
    }
}
String extraerInstruccion(byte* bufferLectura, int posicionInicio, int posicionFinal) {
  
    String inst = "";
    for (int i = posicionInicio; i <= posicionFinal; i++) {
        //Serial.write(bufferLectura[i]);
        inst += (char) bufferLectura[i];
    }
    return inst;
}
void construirMensaje(String mensaje) {
    sprintf(mensajeAEnviar, "{\"type\": \"chat.message\", \"name\": \"%s\", \"text\": \"%s\" } ", nombreCliente, mensaje);
}


void hexdump(const void *mem, uint32_t len, uint8_t cols = 16) {
  const uint8_t* src = (const uint8_t*) mem;
  USE_SERIAL.printf("\n[HEXDUMP] Address: 0x%08X len: 0x%X (%d)", (ptrdiff_t)src, len, len);
  for(uint32_t i = 0; i < len; i++) {
    if(i % cols == 0) {
      USE_SERIAL.printf("\n[0x%08X] 0x%08X: ", (ptrdiff_t)src, i);
    }
    USE_SERIAL.printf("%02X ", *src);
    src++;
  }
  USE_SERIAL.printf("\n");
}







void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  String cadena;

  switch(type) {
    case WStype_DISCONNECTED:
      USE_SERIAL.printf("[WSc] Disconnected!\n");
      break;
    case WStype_CONNECTED:
      USE_SERIAL.printf("[WSc] Connected to url: %s\n", payload);

      SERIAL_BASE.printf("[WSc] Connected to url: %s\n", payload);

      

      // send message to server when Connected
      //webSocket.sendTXT("Connected");
      webSocket.sendTXT("{\"type\": \"chat.message\", \"name\": \"python\", \"text\": \"hola desde arduino\"}");

      
      
      break;
    case WStype_TEXT:
      // USE_SERIAL.printf("[WSc] get text: %s %u\n", payload, length);



      cadena = decodificarJson(payload, length);
      USE_SERIAL.print(cadena);

      // send message to server
      // webSocket.sendTXT("message here");
      break;
    case WStype_BIN:
      USE_SERIAL.printf("[WSc] get binary length: %u\n", length);
      hexdump(payload, length);

      // send data to server
      // webSocket.sendBIN(payload, length);
      break;
    case WStype_ERROR:      
    case WStype_FRAGMENT_TEXT_START:
    case WStype_FRAGMENT_BIN_START:
    case WStype_FRAGMENT:
    case WStype_FRAGMENT_FIN:
      break;
  }

}





String decodificarJson(uint8_t * payload, size_t length) {
    String cadena = "";
    int inicio = 0;
    int fin = 0;
    String aux;

    sprintf(bufferLecturaWebsocket, "[WSc] get text: %s", payload);
    cadena.concat(bufferLecturaWebsocket);

    
    //Serial.print ("Decodificar JSON");

    Serial.println();
    Serial.print (cadena);

    inicio = cadena.indexOf("\"name\": \"") + 9;
    fin = cadena.indexOf("\", \"", inicio);
    Serial.printf ("%d %d,", inicio, fin);

    //Serial.print ( cadena.substring(inicio, fin) );
    aux = cadena.substring(inicio, fin);

    if (aux  == nombreCliente){
        //Serial.println ("Mensaje propio");
        return "";
    }else {
        inicio = cadena.indexOf("\"text\": \"") + 9;
        fin =  cadena.lastIndexOf("\"");
    
        return cadena.substring(inicio, fin);
    }
    return "";
    

}
















//////////////////////////////Temporizadores

void actualizarTON (int i) {
    if (TON[i].entrada){
        if (TON[i].reset) {
            TON[i].salida = false;
            TON_Aux[i].bandera = false;
            TON[i].reset = false;
        }
    
        if (!TON_Aux[i].bandera) {
           TON_Aux[i].bandera = true;
           TON_Aux[i].tiempo_Aux1 = millis ();  
        }
        
        TON_Aux[i].tiempo_Aux2 = millis ();
        TON[i].tiempoActual = TON_Aux[i].tiempo_Aux2 - TON_Aux[i].tiempo_Aux1;
    
        if (TON[i].tiempoActual > TON[i].tiempo) {
            TON[i].salida = true;
        }
    } else {
        TON[i].salida = false;
        TON_Aux[i].bandera = false;
    }
}
