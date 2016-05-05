void setup()
{
  // Configures internal multiplexer
  Utils.setMuxAux1();
  // Configures baudrate
  beginSerial(9600,1);
  serialFlush(1);  //clear buffers
  digitalWrite(SENS_PW_3V3,HIGH);
  USB.ON();
  delay(500);
}

void loop()
{ 
  float val1 = analogRead(ANALOG1);
  val1 = ((val1*3.3)/1023)*163.4;
  USB.print(" ANALOG " );
  USB.println(val1);
  char maxbotix[3];
  while(serialAvailable(1) > 0) 
  {
     if((serialRead(1))=='R'){ 
      for(int n = 0; n < 3; n++){
        USB.print(n);
         USB.print(": ");
         USB.println(serialRead(1));
          maxbotix[n]=serialRead(1); 
      }
          USB.print("maxbotix: ");
          USB.println(maxbotix); 
    }
   // USB.print("reading ");
   // USB.println(serialRead(1));
    serialFlush(1);

  }
  //USB.println(serialRead(1));
      
    delay(3000);
}
