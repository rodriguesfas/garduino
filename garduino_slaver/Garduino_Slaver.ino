/** 
 * ========================================================================
 * 
 * Software: Garduino
 * Descripetion: SLAVER
 * Version: 0.0.1
 * Date: 24/09/2016
 * Developer: Alex VS Assis
 * 
 * Aula 70 - Controle de Servo com Motor Shield L293D
 * Autor: Eng. Wagner Rambo  Data: Junho de 2016
 * ===================================================================== */
*/
 
// --- Mapeamento de Hardware ---
#define   dir_clk      4     //clock do registrador no digital  4
#define   dir_ser      8     //dados do registrador no digital  8
#define   dir_latch   12     //latch do registrador no digital 12
#define   dir_en       7     //enable do registrador no digital 7
#define   pwm2a       11     //pwm motor1
#define   pwm2b        3     //pwm motor2
#define   pwm0a        6     //pwm motor3
#define   pwm0b        5     //pwm motor4
#define   servo2       9     //controle do Servo 2
#define   servo1      10     //controle do Servo 1

#define PIN_COMUNICATION 2

#define PIN_JOYSTICK_MOVE         A2
int portCom = 0;
 
 
// --- Protótipo das Funções Auxiliares ---
void sendCommand(unsigned char value);   //Função para enviar o byte para saída do registrador
void servo0graus();
void servo90graus();
void servo180graus();
 
 
// --- Configurações Iniciais ---
void setup() {

	Serial.begin(9600);

  pinMode(dir_clk,   OUTPUT);    //saída para clock
  pinMode(dir_ser,   OUTPUT);    //saída para dados
  pinMode(dir_latch, OUTPUT);    //saída para latch
  pinMode(dir_en,    OUTPUT);    //saída para enable
  pinMode(pwm2a,     OUTPUT);    //saída para pwm motor1
  pinMode(pwm2b,     OUTPUT);    //saída para pwm motor2
  pinMode(pwm0a,     OUTPUT);    //saída para pwm motor3
  pinMode(pwm0b,     OUTPUT);    //saída para pwm motor4
  pinMode(servo2,    OUTPUT);    //saída para Servo 2
  pinMode(servo1,    OUTPUT);    //saída para Servo 1
   
  digitalWrite(dir_en, LOW);
  analogWrite(pwm2a, 0xFF);
  analogWrite(pwm2b, 0xFF);
  analogWrite(pwm0a, 0xFF);
  analogWrite(pwm0b, 0xFF);

  /* config comunication */
  pinMode(PIN_COMUNICATION, INPUT);
 
 
} //end setup
 
 
void loop() {
   
if(digitalRead(PIN_COMUNICATION)){
  for(char i=0;i<100;i++) servo0graus();    //move o servo para a posição 0º por 2 segundos
  for(char i=0;i<100;i++) servo90graus();    //move o servo para a posição 90º por 2 segundos
  for(char i=0;i<100;i++) servo180graus();    //move o servo para a posição 180º por 2 segundos
  for(char i=0;i<100;i++) servo90graus();    //move o servo para a posição 90º por 2 segundos
}else{
	readJoystickX();
}
 
 
} //end loop

/**
 * readJoystickX
 */
 void readJoystickX(){
 	portCom = analogRead(PIN_JOYSTICK_MOVE); /*Pino X do Joystick para receber dados*/
  	portCom = map(portCom, 0, 1023, 0, 180); /*Faz um mapeamento dos dados recebidos*/

  	Serial.println(portCom);

  	if(portCom >= 100){
  		servo180graus();
  	} else if((portCom == 0) || (portCom <= 45)){
  		servo0graus();
  	}else if(portCom > 45 || portCom <= 100){
  		servo90graus();
  	}

 }

 
 
// --- Desenvolvimento das Funções Auxiliares ---
void sendCommand(unsigned char value)
{
   
    boolean verify;                  //bit de controle
 
    digitalWrite(dir_latch, LOW);    //desliga latch
    digitalWrite(dir_clk,   LOW);    //desabilita o clock
     
    for(unsigned char i=0x0; i<0x08; i++)    //loop finito para enviar os 8 bits
    {
       digitalWrite(dir_clk, LOW);   //clock em low
        
       if(value & (1<<i)) verify = HIGH;
       else verify = LOW;
        
       digitalWrite(dir_ser, verify); //Habilita saída de dados conforme condição acima
       digitalWrite(dir_clk, HIGH);
     
    } //end for
     
     
    digitalWrite(dir_clk, LOW);
     
    digitalWrite(dir_latch, HIGH);
 
 
} //end sendCommand
 
 
 
 /**
  * servo0graus
  * 600 = 0º
  * 1000 = 40º
  */
void servo0graus()              //Posiciona o servo em 0 graus
{
   digitalWrite(servo1, HIGH);  //pulso do servo
   delayMicroseconds(1000);      //0.6ms
   digitalWrite(servo1, LOW);   //completa periodo do servo
   for(int i=0;i<32;i++)delayMicroseconds(1000);
                               // 20ms = 20000us
                               // 20000us - 600us = 19400us
                               // 19400/600 = ~~32
 
} //end servo0graus
 
 
void servo90graus()             //Posiciona o servo em 90 graus
{
   digitalWrite(servo1, HIGH);  //pulso do servo
   delayMicroseconds(1500);     //1.5ms
   digitalWrite(servo1, LOW);   //completa periodo do servo
   for(int i=0;i<12;i++)delayMicroseconds(1500);
                               // 20ms = 20000us
                               // 20000us - 1500us = 18500us
                               // 18500/1500 = ~~12 
 
} //end servo0graus
 

 /**
  * 2000 = 160º
  * 2400 = 180º
  */
void servo180graus()             //Posiciona o servo em 180 graus
{
   digitalWrite(servo1, HIGH);  //pulso do servo
   delayMicroseconds(2000);     //2.4ms
   digitalWrite(servo1, LOW);   //completa periodo do servo
   for(int i=0;i<7;i++)delayMicroseconds(2000);
                               // 20ms = 20000us
                               // 20000us - 2400us = 17600us
                               // 17600/2400 = ~~7
 
} //end servo0graus
