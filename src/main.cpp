#include <Arduino.h>
#include "utils/PID.h"

//vel max ?? mc/s
//Dif. mode

// Define the target and current values
float y_ref[4] = {0,0,0,0};
float y[4] = {0,0,0,0};

// Define the control signal
float u[4] = {0,0,0,0};

// Define the sampling time 
unsigned long sample_time = 100; // in milliseconds

// Define the variables used to compute the sample time
unsigned long prev_time = 0;
unsigned long current_time = 0;
unsigned long time_change = 0;

//Define the variables used to obtain the wheel speed
volatile int enc_time[4],enc_end[4],enc_start[4];
volatile long enc_count[4] = {0,0,0,0};

//Define the variables to track the state of rotation
volatile char sent[4] = {0,0,0,0};

//Define the variables for data transmission
const int BUFFER_SIZE = 10;
const char STX = '\x24';
const char ETX = '\x3b';
char buf[BUFFER_SIZE],VEL[3],ANG[3],msg[10];
int vel,sen,ang,crc,check;

                              //pwm, dir
PID PIDs[4]={PID(0.1, 0.01, 0.0005, 6, 7, 255, 0, sample_time), //mot 1 
             PID(0.1, 0.01, 0.0005, 8, 9, 255, 0, sample_time), //mot 2
             PID(0.1, 0.01, 0.0005, 4, 5, 255, 0, sample_time), //mot 3
             PID(0.1, 0.01, 0.0005, 2, 3, 255, 0, sample_time)  //mot 4
              };

void enc_isr1();
void enc_isr2();
void enc_isr3();
void enc_isr4();

byte CRC8(const byte *data, size_t dataLength);

void speed_data();

void setup() {
  // put your setup code here, to run once:
  // Initialize the serial communication
  Serial.begin(115200);
  
  // Set the pin mode

  pinMode(26, INPUT_PULLUP); //CLK_enc mot1 (a - verde) 
  pinMode(27, INPUT_PULLUP); //DT_enc (b - amarillo)

  pinMode(28, INPUT_PULLUP); //CLK_enc mot2
  pinMode(29, INPUT_PULLUP); //DT_enc

  pinMode(30, INPUT_PULLUP); //CLK_enc mot3
  pinMode(31, INPUT_PULLUP); //DT_enc

  pinMode(32, INPUT_PULLUP); //CLK_enc mot4
  pinMode(33, INPUT_PULLUP); //DT_enc

  attachInterrupt(digitalPinToInterrupt(26),enc_isr1,CHANGE); //Enc Mot1
  attachInterrupt(digitalPinToInterrupt(28),enc_isr2,CHANGE); //Enc Mot2
  attachInterrupt(digitalPinToInterrupt(30),enc_isr3,CHANGE); //Enc Mot3
  attachInterrupt(digitalPinToInterrupt(32),enc_isr4,CHANGE); //Enc Mot4
  
}

void loop() {

  if (Serial.available() > 0)
  {
    if (Serial.read() == STX)
    {
      char buffer[64]; // Crear un búfer para almacenar los datos (ajusta el tamaño según sea necesario)
      int bytesRead = Serial.readBytesUntil(';', buffer, sizeof(buffer)); // Leer hasta el punto y coma
      buffer[bytesRead] = '\0'; // Agregar un carácter nulo para que buffer sea una cadena válida

      if (bytesRead > 0) {
        char* token = strtok(buffer, " "); // Dividir la cadena en tokens usando el espacio como separador
        int valueIndex = 0;
        while (token != NULL) {
          if (valueIndex < 3) { // Solo procesar los primeros 3 tokens (excluyendo el último)
            strcat(msg, token); // Concatenar el token a la variable msg
            strcat(msg, " ");   // Agregar un espacio entre los tokens
          }
          if (valueIndex == 0) { // Cuando valueIndex sea 0, estamos en el valor de X
            vel = atoi(token); // Convertir el token a un entero
          }

          if (valueIndex == 1) { // Cuando valueIndex sea 1, estamos en el valor de X
            ang = atoi(token); // Convertir el token a un entero
          }

          if (valueIndex == 2) { // Cuando valueIndex sea 2, estamos en el valor de X
            sen = atoi(token); // Convertir el token a un entero
          }

          if (valueIndex == 3) { // Cuando valueIndex sea 3, estamos en el valor de X
            crc = atoi(token); // Convertir el token a un entero
            break; // Salir del bucle una vez que hayamos encontrado el valor de X
          }

          token = strtok(NULL, " "); // Obtener el siguiente token
          valueIndex++;
        }
    
      size_t msgLength = strlen(msg);
      // Desplaza los caracteres existentes hacia la derecha para hacer espacio al nuevo carácter
      for (int i = msgLength; i > 0; i--) {
        msg[i] = msg[i - 1];
      }

      msg[0] = '$';
      msg[msgLength] = '\0';

      msgLength = strlen(msg);
      check = CRC8((byte*)&msg,msgLength);

      Serial.print(vel);
      Serial.print(" ");
      Serial.print(ang);
      Serial.print(" ");
      Serial.print(sen);
      Serial.print(" ");
      Serial.print(crc);
      Serial.print(" ");
      Serial.print(msg);
      Serial.print(" ");
      Serial.print(check);
      Serial.println(" ");

      if(check==crc){
        y_ref[0]=(2*vel-ang*24)/(2); //velangular=(2*vel-velA*L)/(2*R); vel lineal = velANG*R
        y_ref[1]=(2*vel+ang*24)/(2); 
        y_ref[2]=y_ref[0];
        y_ref[3]=y_ref[1];
        }
      }
    }
  }

  // Get the current speed
  for(int i=0; i<4; i++){
    if(enc_count[i]>=200 || enc_count[i]<=-200){
      enc_count[i]=0;

      enc_end[i]=millis();
      enc_time[i]=enc_end[i]-enc_start[i];
      y[i]=(12*M_PI*1000/(30*enc_time[i])); //cm/s
      enc_start[i]=enc_end[i];
    
    }
  }
    
  // Get the current time
  current_time = millis();
  time_change = current_time - prev_time;

  // Wait for the sampling time
  if (time_change >= sample_time) {
    for(int i=0; i<4; i++){
      PIDs[i].update(y[i],y_ref[i],sent[i]);
      u[i] = PIDs[i].getU();
    }

    // Update the previous time
    prev_time = current_time;

    // Print the desired data
    //speed_data();
  }

}

void enc_isr1(){
// If the inputDT state is different than the inputCLK state then
// the encoder is rotating clockwise
  if (digitalRead(26) != digitalRead(27)) {
    enc_count[0] ++;
    sent[0]= '0'; //adelante
   } else {
    // Encoder is rotating counterclockwise
    enc_count[0] --;
    sent[0]= '1'; //Atras
   }

}
void enc_isr2(){
// If the inputDT state is different than the inputCLK state then
// the encoder is rotating clockwise
  if (digitalRead(28) != digitalRead(29)) {
    enc_count[1] ++;
    sent[1]= '0';
   } else {
    // Encoder is rotating counterclockwise
    enc_count[1] --;
    sent[1]= '1';
   }

}
void enc_isr3(){
// If the inputDT state is different than the inputCLK state then
// the encoder is rotating clockwise
  if (digitalRead(30) != digitalRead(31)) {
    enc_count[2] ++;
    sent[2]= '0';
   } else {
    // Encoder is rotating counterclockwise
    enc_count[2] --;
    sent[2]= '1';
   }

}
void enc_isr4(){
// If the inputDT state is different than the inputCLK state then
// the encoder is rotating clockwise
  if (digitalRead(32) != digitalRead(33)) {
    enc_count[3] ++;
    sent[3]= '1';
   } else {
    // Encoder is rotating counterclockwise
    enc_count[3] --;
    sent[3]= '0';
   }

}

//CRC-8 - based on the CRC8 formulas by Dallas/Maxim
//code released under the therms of the GNU GPL 3.0 license
byte CRC8(const byte *data, size_t dataLength)
{
  byte crc = 0x00;
  while (dataLength--)
  {
    byte extract = *data++;
    for (byte tempI = 8; tempI; tempI--)
  {
      byte sum = (crc ^ extract) & 0x01;
      crc >>= 1;
      if (sum)
    {
        crc ^= 0x8C;
      }
      extract >>= 1;
    }
  }
  return crc;
}
 
void speed_data(){
  Serial.print(time_change);
  Serial.print(" ");
  Serial.print(y_ref[0]);
  Serial.print(" ");
  Serial.print(y_ref[1]);
  Serial.print(" ");
  Serial.print(y_ref[2]);
  Serial.print(" ");
  Serial.print(y_ref[3]);
  Serial.print(" ");
  Serial.print(y[0]);
  Serial.print(" ");
  Serial.print(y[1]);
  Serial.print(" ");
  Serial.print(y[2]);
  Serial.print(" ");
  Serial.print(y[3]);
  Serial.print(" ");
  Serial.print(sent[0]);
  Serial.print(" ");
  Serial.print(sent[1]);
  Serial.print(" ");
  Serial.print(sent[2]);
  Serial.print(" ");
  Serial.print(sent[3]);
  Serial.print(" ");
  Serial.print(u[0]);
  Serial.print(" ");
  Serial.print(u[1]);
  Serial.print(" ");
  Serial.print(u[2]);
  Serial.print(" ");
  Serial.print(u[3]);
  Serial.println(" ");
}