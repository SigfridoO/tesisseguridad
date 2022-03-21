///////////////////Codigo de comunicacion////////////////////////////



// Instrucciones envidas desde android a arduino
#define ACTIVACION_MANUAL "1B11"                  //-> activar bandera M[4]         ->
#define DESACTIVACION_MANUAL "1B11"               //-> desactivar bandera M[4]
#define SENIAL_GPS_ACTIVA "1B11"                  //-> activar gps M[4]
#define SENIAL_GPS_DESACTIVA "1B11"               //-> desactivar bandera M[4]

#define ENVIAR_SENIALES "1212"                    //                                -> REGRESAR EL ESTADO DE LAS SEÑALES POR EL PUERTO SERIE

#define RESPUESTA_ENVIAR_SENIALES "1434"          //                                -> construye la respuesta #1434000000111000000&


#define TAMANIO_BUFFER 100
byte bufferLectura[TAMANIO_BUFFER];
int indiceBuffer = 0;

#define TAMANIO_BUFFER_2 256
byte bufferLectura2[TAMANIO_BUFFER_2];
int indiceBuffer2 = 0;

char caracterInicio = '#';
char caracterFinal = '&';

void almacenarPuertoSerie ();
void leerPuertoSerie();

String extraerInstruccion(byte* , int , int);
void seleccionarInstruccion (String);

void almacenarPuertoSerie2 ();
void leerPuertoSerie2();
//////////////////////////////Funciones de usuario//////////////


int senialGPS0();
int senialGPS1();
int activManual();
int desactivManual();
/*
char P1A(String);
char P1B(String);
char P2A(String);
char P2B(String);
char P3A(String);
char P3B(String);
char P4A(String);
char P4B(String);
char P5F(String);*/

String extraerInstruccion(byte*, int, int);

void actualizarPines();
void apagarAlarma();
char caracterLeido();

///////////////////Codigo para declarar pines de entrada y salida//////
int di_0=5;
int di_1=6;
int di_2=7;
int di_3=8;
int di_4=9;
int di_5=10;

int do_0=11;
int do_1=12;
int do_2=13;
//[]
byte X[8];
byte Y[8];
byte M[48];


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


int contador = 0;


void setup() {
  // Definir pines de entrada y salida

  pinMode(di_0,INPUT);
  pinMode(di_1,INPUT);
  pinMode(di_2,INPUT);
  pinMode(di_3,INPUT);
  pinMode(di_4,INPUT);
  pinMode(di_5,INPUT);

  pinMode(do_0, OUTPUT);
  pinMode(do_1, OUTPUT);
  pinMode(do_2, OUTPUT);


  TON[0].tiempo = (unsigned long)1 * 2000;
  

String envioMensaje= "#1A";


/////////////////////Codigo para inicializar puertos de comunicación
   
//M[1]=Identificación de entrada 1 --> Respuesta (%=Abierto y *Cerrado)
M[2]=0; // 0=Usuario fuera, 1=Usuario dentro --> Señal GPS ()
//M[3]=0; // 1=Encendido módo manual, 0= Apagado modo manual --> Señal de encenddido manual --> Petisión #_%1A24&
//I0=Interruptor electromágnetico del zaguan --> *
//M4=Encendido del sistema --> Fisico #_%1A24&
//M[5]=0; //Mensaje de activación a la aplicación
//M[6]=Activación de alarma 
//M[7]=Mensaje de que se activo la alarma
//M[8]=Llamada de emergencia
//M[9]=Botón para activación de alarma programada
//M[10]=0; // Activación por alarma programada por horario, 1=Encendido y 0=Apagado
//Q[0]=Activación de cerradura electromagnetica
//M[11]=Botón de activación manual
M[12]=0; //Botón de desactivación manual


  Serial.begin(9600);
  Serial2.begin(115200);
}


void loop() {
     
/////////////////////Codigo para comunicación/////////////////////////
  
    almacenarPuertoSerie();
    leerPuertoSerie();


    almacenarPuertoSerie2();
    leerPuertoSerie2();
    
/////////////////////Codigo para control del programa////////////////
    
    M[1]=(X[0] & !M[2] ) | (X[0] & M[3]);
    M[4]=((M[1] | M[4]) & !M[2]) | ((M[1] | M[4]) & M[3]);
    M[5]= M[4] & M[1];
    M[6]=!M[5] & !M[1];
    M[8]=M[6] & !M[7];
    //M[10]=M[9] &
    Y[0]=M[4] & (!M[2] | M[3]);
    M[3]= (M[11] | (M[3] && !M[10]) | M[10]) & !M[12];

    

/////////////////Mensajes de información del sistema/////////////




    
    //////////// Actualización de las entradas y salidas////////////////
    actualizarPines();


    
    TON[0].entrada = !TON[0].salida;
    actualizarTON(0);

    if (TON[0].salida) {
        contador++;
        //Serial.println(contador);

        Serial2.print(construirMensaje(String (contador) ));
    }

     /*
    String cadena = enviarSeniales();
    Serial.print (cadena);
    Serial.println();
    */

}



String construirMensaje(String mensaje) {
  String respuesta = "";
  respuesta.concat(caracterInicio);
  respuesta.concat(mensaje);
  respuesta.concat(caracterFinal);
  return respuesta;
}




















/////////////////////Creación del Mensaje//////////////////////////


void actualizarPines(){

    X[0]= digitalRead (di_0);
    X[1]= digitalRead (di_1);
    X[2]= digitalRead (di_2);
    X[3]= digitalRead (di_3);
    X[4]= digitalRead (di_4);
    X[5]= digitalRead (di_5);
    
    digitalWrite (do_0,Y[0]);
    digitalWrite (do_1,Y[1]);
    digitalWrite (do_2,Y[2]);
    
    //Serial.print ("\t" );
}


void almacenarPuertoSerie () {

  while(Serial.available()) {
      byte caracterLeido = Serial.read();
      //Serial.write (caracterLeido);
      bufferLectura[indiceBuffer++] =  caracterLeido;

      if (indiceBuffer > int( TAMANIO_BUFFER-10)){
        indiceBuffer = 0;
      }
  }
}





void leerPuertoSerie() {

    char instrucion = 0;
    boolean inicioDeInstruccionEncontrado = false;
    int posicionInicio= 0;
    int posicionFinal = 0;
    byte * cadena;
    String instruccionDetectada = "";
    
    for (int i = 0; i < indiceBuffer; i++) {
    
        if (bufferLectura[i] == caracterInicio ) {
            //Serial.print("Se encontro el caracter de inicio en la posicion");
            //Serial.println (i);
            inicioDeInstruccionEncontrado = true;
            posicionInicio = i;
        }
      
        if (inicioDeInstruccionEncontrado && (bufferLectura[i] == caracterFinal) ) {
            //Serial.print("Se encontro el caracter de final en la posicion");
            //Serial.println (i);
            posicionFinal = i;
            instruccionDetectada = extraerInstruccion(bufferLectura, posicionInicio, posicionFinal);
            seleccionarInstruccion(instruccionDetectada);
            
            indiceBuffer = 0;
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

void seleccionarInstruccion (String cadena) {  
  // caracterInicial + instruccion + datos + tamanio + codigo de verificacion de error + caracterFinal
  // caracterInicial + instruccion + datos + caracterFinal

  // inst : String  

  Serial.print("Seleccionando instrucción");
  Serial.print(cadena);
}
























String enviarSeniales() {
  String respuesta = "";
  respuesta.concat(caracterInicio);
  respuesta.concat(RESPUESTA_ENVIAR_SENIALES);
  
  for(int i=0; i<6; i++) {
    respuesta.concat(String(X[i]));
  }
  
  for(int i=0; i<3; i++){
    respuesta.concat(String(Y[i]));
  }
  
  respuesta.concat(caracterFinal);
  return respuesta;
}




void almacenarPuertoSerie2 () {
  while(Serial2.available()) {
      byte caracterLeido = Serial2.read();
      //Serial.write (caracterLeido);
      bufferLectura2[indiceBuffer2++] =  caracterLeido;

      if (indiceBuffer2 > int( TAMANIO_BUFFER_2-10)){
        indiceBuffer2 = 0;
      }
  }
}

void leerPuertoSerie2() {
  
  /*
  Serial.print ("buffer indice");
  Serial.print (indiceBuffer2);
  */

  if (indiceBuffer2 > 0) {
    for(int i=0; i< indiceBuffer2; i++){
      Serial.write (bufferLectura2[i]);
  
    }
    //Serial.print ("\n");
    indiceBuffer2 = 0;
  }

  
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
