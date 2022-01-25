///////////////////Codigo de comunicacion////////////////////////////
#define TAMANIO_BUFFER 100
byte bufferLectura[TAMANIO_BUFFER];

int indiceBuffer = 0;

char caracterInicio = '#';
char caracterFinal = '&';

//////////////////////////////Funciones de usuario//////////////

void leerAlmacenarSerie ();
void decodificarInstrucion();

int senialGPS0();
int senialGPS1();
int activManual();
int desactivManual();

String extraerInstruccion(byte*, int, int);
//String identificarInstruccion(byte*, int, int);

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


/////////////////////Codigo para inicializar puertos de comunicación
   Serial.begin(9600);
//M[1]=Identificación de entrada 1 --> Respuesta (%=Abierto y *Cerrado)
M[2]=1; // 0=Usuario fuera, 1=Usuario dentro --> Señal GPS ()
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
}


void loop() {
     
/////////////////////Codigo para comunicación/////////////////////////
    leerAlmacenarSerie();
    decodificarInstrucion();
    
/////////////////////Codigo para control del programa////////////////
    
    //M[3]=0;
    //M[7]=X[2];
    M[1]=(X[0] & !M[2] ) | (X[0] & M[3]);
    M[4]=((M[1] | M[4]) & !M[2]) | ((M[1] | M[4]) & M[3]);
    M[5]= M[4] & M[1];
    M[6]=!M[5] & !M[1];
    M[8]=M[6] & !M[7];
    //M[10]=M[9] &
    Y[0]=M[4] & (!M[2] | M[3]);
    M[3]= (M[11] | (M[3] && !M[10]) | M[10]) & !M[12];

/////////////////Mensajes de información del sistema/////////////
      
    /*Serial.print(" X[0]= ");
    Serial.print(X[0]);
    Serial.println();*/


if(M[1] == 1 && M[4] == 1){
    Serial.print("Zaguan Cerrado"); // #1A15&  
    Serial.println();    
}

if(M[1] == 0 && M[4] == 1){
    Serial.print("Zaguan Abierto"); // #0A15&
    Serial.println();
}

if(M[4] == 1){
    Serial.print("Sistema Activado"); 
    Serial.println();
}

if(M[5] == 1 && M[4] == 1){
    Serial.print("Lugar Asegurado"); // #1A054& 
    Serial.println();
}

if(M[6] == 1 && M[4] == 1){
    Serial.print("Sistema violentado"); // #1A064&
    Serial.println();
}

if(M[8] == 1 && M[4] == 1){
    Serial.print("Llamada en curso"); // #1A084& 
    Serial.println();  
}

if(Y[0] == 1 && M[4] == 1){
    Serial.print("Cerradura Zaguan activada"); // #1Y014&
    Serial.println();
}
if(M[3] == 1 && M[11] == 1){
    Serial.print("Activado manualmente"); // #1B114& 
    Serial.println();  
}

if(M[3] == 0 && M[12] == 1){
    Serial.print("desactivado manualmente");  // #1B124&
    Serial.println();  
}

if(M[3] == 1 && M[10] == 1){
    Serial.print("Acivación por horario"); // #1B104&
    Serial.println();  
}

    
//////////// Actualización de las entradas y salidas////////////////
    actualizarPines();

}


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

char* enviarAlarmas(){
    
    char Mensaje[64];
    return Mensaje;
}


void apagarAlarma (){

  //M[3]=1;

}


void leerAlmacenarSerie () {

  while(Serial.available()) {
      byte caracterLeido = Serial.read();
      //Serial.write (caracterLeido);
      bufferLectura[indiceBuffer++] =  caracterLeido;

      if (indiceBuffer > int( TAMANIO_BUFFER-10)){
        indiceBuffer = 0;
        // TODO: recorrer el arreglo
      }
  }
}

void decodificarInstrucion() {

  /*
 
    Serial.print(">");

  for(int i = 0; i< TAMANIO_BUFFER;i++){
    Serial.write(bufferLectura[i]);
  }
  Serial.print("<");
  Serial.println();
*/

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
      indiceBuffer = 0;
      Serial.println (instruccionDetectada);
    }
    
    
    
    // extraer la cadena de texto entre esos caracteres


    // hacer una serie de comparaciones para obtener la instruccion
    

    instrucion = 'h';
    
  }

  /*
     switch(instrucion) {

      case instrucion_1: 
        funcion_1();
        break;
        
      case instrucion_2: 
        funcion_2();
        break;
    
    }
*/
}

String extraerInstruccion(byte* bufferLectura, int posicionInicio, int posicionFinal) {

  String inst = "";
  
  for (int i = posicionInicio; i <= posicionFinal; i++) {
    //Serial.write(bufferLectura [i]);
    inst += (char) bufferLectura [i];
  }
  
  //Serial.println();
  Serial.print (inst);

  if(inst == "#0B024&"){
    senialGPS0();
    //M[2]=0;
    Serial.print("Usuario Fuera");
    Serial.println();
  }

  if (inst == "#1B024&"){
    senialGPS1();
    //M[2]=1;
    Serial.print("Usuario dentro");
    Serial.println();
  }
  if (inst == "#1B114&"){ 
    activManual();  
    Serial.print("Activación manual");
    Serial.println();
  }
  if (inst == "#1B124&"){
    desactivManual(); 
    Serial.print("Desactivación manual");
    Serial.println();
  }  
  delay(2000);

 //return inst;
}

int senialGPS0(){
  M[2]=0;
}

int senialGPS1(){
  M[2]=1;
}

int activManual(){
  M[11]=1;
}

int desactivManual(){
  M[12]=1;
}
