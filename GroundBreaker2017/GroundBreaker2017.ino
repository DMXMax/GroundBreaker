/*
  Serial Music
  Listen on a Pin, play song when motion sensor triggers
  
 */
 
// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
byte ledPin = 13;
byte motionSensorPin  = 9;
byte buttonPin= 8;
byte lightPin =4;
byte relayLeftPin =5;
byte relayRightPin = 6;


const byte SOUNDCOUNT = 6;
const byte SOUND_STOP = 0xEF;
const byte MAX_VOL = 0xE7;
const int SCAN_DELAY = 10; // This delay was 1000 for the motion sensor. Now turned to 0
const int SOUND_DURATION = 5000;
const int SILENCE_DURATION = 3000;
const int PULSE_DELAY = 50;
const int BAUD = 4800;
const int INITIALIZATION_DELAY = 5000;
const int COMMAND_DELAY = 500;
const int DEBOUNCE_THRESHOLD=5;


int bDebounceCount; //debounce counter

//an array of function pointers to hold all the different dances we do!
void (*dances[3])();
// the setup routine runs once when you press reset:
void setup() 
{                
  // initialize the digital pin as an output.

  pinMode(ledPin, OUTPUT);
  pinMode(lightPin, OUTPUT);
  pinMode(relayLeftPin, OUTPUT);
  pinMode(relayRightPin, OUTPUT);
  

  digitalWrite(relayLeftPin, LOW);
  digitalWrite(relayRightPin, LOW);

  pinMode(buttonPin, INPUT_PULLUP);
  digitalWrite( buttonPin, HIGH); //set the pull-down resistor on the button pin
  pinMode(motionSensorPin, INPUT);
  //digitalWrite(motionSensorPin, LOW); // set pull-down resistors pulldowns don't exist! This will fail. Will need to revisit how to handle this later.

  randomSeed(analogRead(0)); // initalize randomness

  Serial.begin(BAUD);
  Serial.write(SOUND_STOP); // Reset board

  delay(COMMAND_DELAY);  
  Serial.write(MAX_VOL); // Max volume

  
  //put the dance functions into the array of functions
  dances[0] = zombiedance1;
  dances[1] = zombiedance2;
  dances[2] = zombiedance3;

  bDebounceCount = 0;
 // delay(INITIALIZATION_DELAY); // Pause to let the Motion Sensor settle.

//Give the PIR time to initialize. Provide light feedback 
 for (int y =0; y< 6; y++)
 {
    for( int i = 0;i<2;i++)
 
   {
      digitalWrite(lightPin, LOW);
      delay(200);
      digitalWrite(lightPin, HIGH);
      delay(200);
   }

  delay(INITIALIZATION_DELAY);
 }

 // End if PIR initialization

 digitalWrite(lightPin, LOW);

 
}


// the loop routine0x0uns over and over again forever:
void loop() 
{

  
  if( /*digitalRead(buttonPin) == LOW ||*/ digitalRead(motionSensorPin) == HIGH)
  {
    

    if( bDebounceCount > DEBOUNCE_THRESHOLD )
    {
      digitalWrite(ledPin, HIGH);
      //digitalWrite(lightPin, HIGH); //the relay is in NC positon. The light is to be always on
    
      Serial.write(random(SOUNDCOUNT)+1);

      delay(1000);
      //pointer to a function -- execute a random dance!
      (*dances[random(3)])();
       //  (*dances[1])();
     
      delay(SOUND_DURATION);

      //reset everything
      Serial.write( SOUND_STOP );
     // digitalWrite(lightPin, LOW);
      digitalWrite(relayLeftPin, LOW);
      digitalWrite(relayRightPin, LOW);
      
      delay(SILENCE_DURATION);
      bDebounceCount = 0;
    }
    else // bDebounceCount <= DEBOUNCE_THRESHOLD
    {
      bDebounceCount++;
      delay(SCAN_DELAY);
     
    }

  }
  else //motionSensorPin == LOW
  {
    //this is kind of like a negative debounce. If, for some reason, the switch jitters, a single 0 will not 
    //affect the overall accuracy.
    
    //bDebounceCount--;
    bDebounceCount = max(--bDebounceCount, 0);
    delay(SCAN_DELAY);
  }
  
}
  
 void zombiedance1()
 {
   for( int i = 0; i<3;i++ ) 
   {
     digitalWrite(relayLeftPin, HIGH);
     digitalWrite(relayRightPin, HIGH);
     randomDelay();
     digitalWrite(relayLeftPin, LOW);
     digitalWrite(relayRightPin, LOW);
     randomDelay();
   }
 }
 
  void zombiedance2()
 {

     digitalWrite(relayLeftPin, HIGH);
     digitalWrite(relayRightPin, LOW);
  
   for( int i = 0; i<3;i++ ) 
   {
     digitalWrite(relayLeftPin, HIGH);
     delay(300);
     digitalWrite(relayRightPin, LOW);
     randomDelay();
     digitalWrite(relayLeftPin, LOW);
     delay(300);
     digitalWrite(relayRightPin, HIGH);
     randomDelay();
   }
 }

 void zombiedance3()
 {
   for( int i = 0; i<3;i++ ) 
   {
     digitalWrite(relayLeftPin, HIGH);
     delay(300);
     digitalWrite(relayRightPin, HIGH);
     delay(300);
     digitalWrite(relayLeftPin, LOW);
     delay(300);
     digitalWrite(relayRightPin, LOW);
     randomDelay();
   }
 }
 
 /*void zombiedance4()
 {
   for( int i = 0; i<6;i++ ) 
   {
  
       digitalWrite(randomRelayPin(), randomHighLow());
       delay(500);
       digitalWrite(randomRelayPin(), randomHighLow());
       randomDelay();
   }    

 }*/
   

int randomHighLow()
{
  return random(2)==0 ? HIGH : LOW;
}

int randomRelayPin()
{
  return random(2) == 0 ? relayLeftPin : relayRightPin;
}
 
 void randomDelay()
 {
   delay(300+ (150*(random(4))));
 }
