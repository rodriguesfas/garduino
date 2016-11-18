/** 
 * ========================================================================
 * 
 * Software: Garduino
 * Descripetion: MASTER
 * Version: 0.0.1
 * Date: 24/09/2016
 * Developer: Alex VS Assis
 * 
 * ===================================================================== */


#include <Servo.h>
#include <LiquidCrystal.h>

#define BAUDRATE 9600

#define POSITION_BEGIN_SERVO 5
#define POSITION_END_SERVO   170
#define PIN_SERVO 9
Servo myservo;

/* config plants */
#define POSITION_BEGIN_REST 0
#define POSITION_PLANT_1 20
#define POSITION_PLANT_2 90
#define POSITION_PLANT_3 150

boolean status_initial = true;
boolean status_plant_1 = false;
boolean status_plant_2 = false;
boolean status_plant_3 = false;


/* Config button */
#define PIN_BUTTON_CONFIG_MENU 8
int statusButton = 0;
int inputButton = 0;
int status = 0;

/* Config led */
#define PIN_LED 13

/* config LDR */
#define PIN_LDR A1
#define LUMS 300

/* config relé */
#define PIN_RELE 7

/* config hygrometer */
#define PIN_HYGROMETER A0
#define HYGROMETER_VALUE_HUMIDADE_MAX 400  //húmido max
#define HYGROMETER_VALUE_HUMIDADE_MED 700  // seco med
#define HYGROMETER_VALUE_MIN 1000 //seco max

/* config  Joystick */
#define PIN_BUTTON_JOYSTICK  A5

int inputButtonJoystick = 0;

/* control clear */
boolean statusConfigLCD = true;

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

long previousMillis = 0; // Variável de controle do tempo
long interval = 60000;    // Tempo em ms do intervalo a ser executado
unsigned long currentMillis;

/* config comunication */
#define PIN_COMUNICATION 10

/**
 * setup - Configuration initial in system.
 */
 void setup(){
 	Serial.begin(BAUDRATE);

	//cofig myservo
	myservo.attach(PIN_SERVO);
	myservo.write(0);


	/* config pin button */
	pinMode(PIN_BUTTON_CONFIG_MENU, INPUT);

	/* config pin led */
	pinMode(PIN_LED, OUTPUT);

	/* config pin relé */
	pinMode(PIN_RELE, OUTPUT);
	digitalWrite(PIN_RELE, HIGH); //OFF

	/* config pin hygrometer */
	pinMode(PIN_HYGROMETER, INPUT);

	/*config pin Joystick */
	//pinMode(PIN_JOYSTICK_MOVE, INPUT);

	/* config. comunication */
	pinMode(PIN_COMUNICATION, OUTPUT);

	/* config LCD */
	lcd.begin(16, 2); //set up the LCD's number of columns and rows:
	lcd.print("Start System!"); // Print a message initial to the LCD

	actionLed(0);
}

/**
 * loop - 
 */
 void loop(){
 	currentMillis = millis();    //Tempo atual em ms

 	inputButton = digitalRead(PIN_BUTTON_CONFIG_MENU);

 	if (inputButton == HIGH && statusButton == LOW) { // our statusButton == HIGH
 		status = 1 - status;
 		delay(20);
 	}
 	statusButton = inputButton;

 	Serial.print("Botão Menu: ");
 	Serial.print(status);
 	Serial.println("");

 	if(status == 1){ // config servo manual.
 		printModeLCD(1);
 		configServo(1);

 	}else{ // config servo auto.
 		printModeLCD(0);
 		configServo(0);
 	}

 }

/**
 * configServo -
 */
 void configServo(int value){
 	/**
 	 * switch case
	 * [0] manual - permite controlar o braço manualmente
 	 * [1] alto - controla o braçoa altomaticamente.
 	 */
 	switch (value) {
 	    
 	    case 0: //manual
 	      actionButtonJoystick();
 	      //readJoystickX();
 	      digitalWrite(PIN_COMUNICATION, LOW);
 	    break;
 	    
 	    case 1: //auto
 	    	digitalWrite(PIN_COMUNICATION, HIGH);/* comunication */
 	    	photosynthesis();
 	    	water();
 	      break;
 	}

 }

/**
 * photosynthesis - realiza banho de luz artificial na planta.
 */
 void photosynthesis(){
 	int value = readLDR();
 	
 	//Serial.println(value);

 	/**
 	 * value <= LUMS - acende LED.
 	 * value > LUMS - desliga LED. 
 	 */
 	if(value <= LUMS) actionLed(0);
 	else actionLed(1);
 }


/** 
 * readLDR - realiza a leitura do sensor de luminosidade LDR.
 */
 int readLDR(){
 	/**
 	 * map(value, fromLow, fromHigh, toLow, toHigh)
 	 */
 	int value = analogRead(map(PIN_LDR, 0, 1024, 0, 100));
 	//Serial.print("LDR: ");
 	//Serial.print(value);
 	//Serial.println("");

 	return value;
 }

/**
 * actionLed -
 */
 void actionLed(int value){
 	/**
 	 * switch case
	 * [0] - desliga led
 	 * [1] - liga led
 	 */
 	switch (value) {
 	    case 0:
 	      	digitalWrite(PIN_LED, LOW);
 	      break;
 	    case 1:
 	      	digitalWrite(PIN_LED, HIGH);
 	      break;
 	}
 }

/**
 * water - rega planta.
 */
 void water(){
 	int value = readHygrometer();

 	//printSensorLCD(0, value); //imprime value in LCD.
 	//Serial.print("Hygrometer: ");
 	//Serial.print(value);
 	//Serial.println();
 	
 	if(value > HYGROMETER_VALUE_HUMIDADE_MED) // seco
 		actionRele(1); // (1 ON) rega planta.

 	else if(value < HYGROMETER_VALUE_HUMIDADE_MAX) //húmido
 		actionRele(0); // (0 OFF) desligada a bomba d'água.

 }

/**
 * readHygrometer - Realiza leitura do sensor hygrometer.
 */
 int readHygrometer(){
 	/**
 	 * map(value, fromLow, fromHigh, toLow, toHigh)
 	 */
 	int value = analogRead(PIN_HYGROMETER);
 	
 	//config err
 	//if(value < 0) value = 0;
 	//else if(value > 100) value = 100;

 	return value;
 }


  /**
  * actionButtonJoystick
  */
 void actionButtonJoystick(){
 	inputButtonJoystick = digitalRead(PIN_BUTTON_JOYSTICK);

 	if(inputButtonJoystick == 1){ //Desligado
 		actionRele(0); //Desligar Bomba d'água
 		actionLed(0); //Desliga LED.
 	}else { //Ligado
 		actionRele(1); //Liga bomba d'água.
 		actionLed(1); //Liga Led.
 	}
 }

/**
 * actionRele - responsalvel pelo acionamento da bonba d'água.
 */
 void actionRele(int value){
 	/**
 	 * switch case
	 * [0] - desliga relé
 	 * [1] - liga relé
 	 */
 	switch (value) {
 	    case 0:
 	    	digitalWrite(PIN_RELE, HIGH); //OFF
 	    break;

 	    case 1:
 	      digitalWrite(PIN_RELE, LOW); //ON
 	    break;
 	}

 }

 int time(){
 	lcd.setCursor(0, 1);
 	lcd.print("Time: " );
  			lcd.print(millis() / 1000);
 }

/**
 * printModeLCD - Write msg in LCD.
 */
 void printModeLCD(int option){
 	/**
 	 * type msg print lcd.
 	 */
 	switch (option) {
 	    case 0:
 	      lcd.setCursor(1, 1);
 	      lcd.print("Control manual.");
 	      break;

 	    case 1:
 	      lcd.setCursor(1, 1);
 	      lcd.print("Control auto.");
 	      break;

 	    case 2:
 	      lcd.setCursor(0, 1);
 	      lcd.print("Led: ON");
 	    break;

 	    case 3:
 	      lcd.setCursor(0, 1);
 	      lcd.print("Led: OFF");
 	    break;

 	    case 4:
 	      lcd.setCursor(0, 1);
 	      lcd.print("Irr: ON");
 	    break;

 	    case 5:
 	      lcd.setCursor(0, 1);
 	      lcd.print("Irr.: OFF");
 	    break;
 	}

 }

 /**
 * printSensorLCD - Write value in LCD.
 */
 void printSensorLCD(int option, int value){
 	/**
 	 * type value print lcd.
 	 * 
 	 * case [0] showl msg control manual
 	 */
 	switch (option) {
 	    case 0:
 	      lcd.setCursor(0, 1);
 	      lcd.print("Hum.: ");
 	      lcd.print(value);
 	    break;

 	}

 }




