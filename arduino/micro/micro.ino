///////////////////Codigo para declarar pines de entrada y salida

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
//SOLO UNA EJECUCIÓN
void setup() {
//Lectura de entradas

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
M[2]=0; // 0=Usuario fuera, 1=Usuario dentro
//M[3]=0; // 1=Encendido módo manual, 0= Apagado modo manual
M[10]=0; // Activación por alarma programada, 1=Encendido y 0=Apagado
M[5]=0; //Mensaje de alarma
//M[6]= Activación de serraduras, Acción de control
//M[7]= Botón de Apagado de Alarma
//M[1]-M[7]= IDENTIFICACIÓN
//M[8]-M[15]= SALIDA DE ALARMAS



}

char* enviarAlarmas(){

char Mensaje[64];
return Mensaje;
}


void apagarAlarma{

M[3]=1;

  
}


void loop() {
 /*for(int i=0;i<6;i++){
Serial.print(X[i]);
if(i==5){
Serial.println();
}
Serial.write(" ,");
 }*/


 /////////////////////Codigo para control del programa

M[3]=X[1];
M[7]=X[2];
M[1]=X[0] & (!M[2] | M[3]);
M[4]=(M[1] | M[4]) & !M[2] & !M[3];
//M[5]= M[4] & M[1];
M[8]=M[4] & !M[5] & !M[1] & !M[7];
//

if(M[4]==1){
Y[0]=M[8];
}

/*
M[4]=(M[1] | M4[4]) & !M[2] & !M[3];
//Contador
if(i>=1){
M[6]= !M[5] & !M[1];
}
M[8]=M[6] & !M[7];
//M[10]= TOF1.Q
Q[0]= !M[2] | M[3];
M[3]= (M[11] | M[3] | M[10]) & !M[12];*/


Serial.print(" X[0]= ");
Serial.print(X[0]);
Serial.print(" M[1]= ");
Serial.print(M[1]);
Serial.print(" M[4]= ");
Serial.print(M[4]);
Serial.print(" M[6]= ");
Serial.print(M[6]);
Serial.println();




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


Serial.print ("\t" );
  }
