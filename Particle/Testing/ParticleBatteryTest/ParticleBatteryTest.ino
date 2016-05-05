void setup() {

}

void loop() {
FuelGauge fuel;
Serial.print("Voltage: ");
Serial.println( fuel.getVCell() );
Serial.print("Charge: ");
Serial.println( fuel.getSoC() );
delay(5000);
}
