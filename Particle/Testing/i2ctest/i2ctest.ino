//#include <Wire.h>

void setup()
{
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(9600);  // start serial for output
}

void loop()
{
  char combVal[15];
  char temp[10];
  if (Particle.connected() == false) {
    Particle.connect();
  }
  else{
    Serial.print("Connected");
  }
  readI2CPressure(combVal, temp);
  delay(5000);
}
void readI2CPressure(char * combVal, char * temp){
  int val, firstVal, secondVal, thirdVal, Status;
  bool gothere = false;
  // Read I2C Device
  Serial.println("Attempting to read!");
  Wire.requestFrom(0x48, 3);
   while(Wire.available()){    // slave may send less than requested
    for (int i = 0; i < 3 ; i++){
      val = Wire.read(); //reads the value from address 40 0x28
      if ( i == 0 ){// makes sure we only get the status bits for the first byte
        Status = val >> 14; // shifts the value over by 14 bits so we can get the status bits
        if (Status != 0){ // checks the the first two status bits to make sure there are no issues.
          Serial.println(F("Error! Data not valid"));
        }
        else{
          firstVal = val; // stores the first byte
           //#ifdef DEBUG
           Serial.print(F("FirstVal: "));
           Serial.println(firstVal);
           //#endif
        }
      }
      else if (i == 1){
        secondVal = val; // stores the second byte
     //   #ifdef DEBUG
         Serial.print(F("SecondVal: "));
         Serial.println(secondVal);
        //#endif
      }
      else if ( i == 2){
        thirdVal = val; // stores the third byte
        //#ifdef DEBUG
         Serial.print(F("temperature: "));
         Serial.println(thirdVal);
         gothere = true;
       // #endif
      }
    }
   }
   if (gothere == true){
    I2Ctemp(thirdVal, temp); //gets the temperature in fahrenheit
    pressure2string(firstVal, secondVal, combVal); //gets RAW pressure value
    //Particle.publish("temp",temp);
    //Particle.publish("Pressure",combVal);
    //Particle.disconnect();
    //Serial.print(F("temp: "));
    //Particle.publish(temp);
    //I2Ctemp(thirdVal, temp); //gets the temperature in fahrenheit
    // ressure2string(firstVal, secondVal, combVal); //gets RAW pressure value
    Serial.print(F("combVal: "));
    Serial.println(combVal);
    //Particle.publish(combVal);
    //Serial.print(F("temp: "));
    //Particle.publish(temp);
    //Serial.println(temp);
    gothere = false;
  }
}

char I2Ctemp( int val, char * temp){
  float cTemp, fTemp;
   val = val << 3; // shift the temperature to the left by 3 bits
  // Determines the Temperature in celsius
   cTemp = ((((float)val/2047)*200)-50);
    Serial.print(F("Temperature in Celsius: "));
    Serial.println(cTemp);
   fTemp = ((cTemp*9)/5) + 32;
    Serial.print(F("Temperature in Fahrenheit: "));
    //Particle.publish(fTemp);
    Serial.println(fTemp);
   //converts int to char *
   float2string(cTemp,temp,3);
 }

char pressure2string( int firstVal, int secondVal, char * combVal){
  // Converts 2 int bytes to char * and then concatenates them
   char nfv[10];
   char nsv[10];
   sprintf((char*)nfv, "%i\0",firstVal);
   sprintf((char*)nsv, "%i\0",secondVal);
   strcpy(combVal,nfv);
   strcat(combVal,nsv);
}

int float2string(float f, char* c, uint8_t prec)
// float value, where you want to store it, how many decimal places you want
{
  int fix = 1;
  int p = f;
  f -= p;
  if ( f < 0.1 ){
    fix = 0;
  }
  while (prec > 0) {
    f *= 10;
    prec--;
  }
  int q = f;
  if ( fix == 0){
     sprintf(c, "%i.%i%i\0",p,fix,q);
  }
  else
    sprintf(c, "%i.%i\0",p,q);
}
