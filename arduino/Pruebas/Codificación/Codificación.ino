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
M[2]=0; // 0=Usuario fuera, 1=Usuario dentro --> Señal GPS ()
//M[3]=0; // 1=Encendido módo manual, 0= Apagado modo manual --> Señal de encenddido manual --> Petisión #_%1A24&
//I0=Interruptor electromágnetico del zaguan --> *
//M4=Encendido del sistema --> Fisico #_%1A24&
M[5]=0; //Mensaje de activación a la aplicación
//M[6]=Activación de alarma y mensaje a la aplicación
//M[8]=Llamada de emergencia
//M[9]=Botón para activación de alarma programada
M[10]=0; // Activación por alarma programada por horario, 1=Encendido y 0=Apagado
//Q[0]=Activación de cerradura electromagnetica
//M[11]=Botón de activación manual
//M[12]=Botón de desactivación manual


}

void loop() {

}
