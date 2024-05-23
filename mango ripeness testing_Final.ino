#include <SoftwareSerial.h>

#define SIM900_TX 9
#define SIM900_RX 10

SoftwareSerial sim900(SIM900_TX, SIM900_RX);

#include <String.h>

//char APN[] = "gpinternet";


//unsigned long myChannelNumber = 2130755;
//const char * myWriteAPIKey = "ELVNS6NI7GJWLMHH";

//float sensorValue = 0;

void setup() {
  
  sim900.begin(9600);
  Serial.begin(9600);
  delay(2000);

}

void loop() {
  int sensorValue_mq3 = analogRead(A0);                     // read sensor value
  float voltage = sensorValue_mq3 * (5.0 / 1023.0);         // convert to voltage
  float ppm_mq3 = voltage / 0.1;                            // convert to ppm
  
  Serial.print("MQ3 value:");
  Serial.print(ppm_mq3);

  int sensorValue_mq135 = analogRead(A1);           // read sensor value
  float resistance = ((1023.0 * 5.0) / sensorValue_mq135) - 1;  // convert to resistance
  float ppm_mq135 = 116.6020682 * pow(resistance, -2.769034857); // convert to ppm using the formula for MQ135 sensor
  
  Serial.print("MQ135 value:");
  Serial.print(ppm_mq135);

  //String message = "MQ3 Sensor Value: " + String(ppm); // format message
 // sim900.println("AT+CMGF=1");                        //Sets the GSM Module in Text Mode
 // delay(1000);                                          // Delay of 1000 milli seconds or 1 second
 
  //sim900.println("AT+CMGS=\"+8801304196623\"\r");  // Replace x with mobile number
  //delay(1000);
 
  //sim900.println(message);  // The SMS text you want to send
 // delay(100);
  //sim900.println((char)26);  // ASCII code of CTRL+Z
  delay(3000);
  
   if (sim900.available())
    Serial.write(sim900.read());
 
  sim900.println("AT");
  delay(1000);
 
  sim900.println("AT+CPIN?");
  delay(1000);
 
  sim900.println("AT+CREG?");
  delay(1000);
 
  sim900.println("AT+CGATT?");
  delay(1000);
 
  sim900.println("AT+CIPSHUT");
  delay(1000);
 
  sim900.println("AT+CIPSTATUS");
  delay(2000);
 
  sim900.println("AT+CIPMUX=0");
  delay(2000);

  ShowSerialData();
 
  sim900.println("AT+CSTT=\"gpinternet.com\"");//start task and setting the APN,
  delay(1000);
 
  ShowSerialData();
 
  sim900.println("AT+CIICR");//bring up wireless connection
  delay(3000);
 
  ShowSerialData();
 
  sim900.println("AT+CIFSR");//get local IP adress
  delay(2000);
 
  ShowSerialData();
 
  sim900.println("AT+CIPSPRT=0");
  delay(3000);
 
  ShowSerialData();
  
  sim900.println("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",\"80\"");//start up the connection
  delay(6000);
 
  ShowSerialData();
  
  sim900.println("AT+CIPSEND");//begin send data to remote server
  delay(4000);
  ShowSerialData();
  
  String str="GET https://api.thingspeak.com/update?api_key=7QNTFJEHEZZT36UJ&field1=" + String(ppm_mq3) +"&field2="+String(ppm_mq135);
  Serial.println(str);
  sim900.println(str);//begin send data to remote server
  
  delay(4000);
  ShowSerialData();
 
  sim900.println((char)26);//sending
  delay(5000);//waitting for reply, important! the time is base on the condition of internet 
  sim900.println();
 
  ShowSerialData();
 
  sim900.println("AT+CIPSHUT");//close the connection
  delay(100);
  ShowSerialData();
} 
void ShowSerialData()
{
  while(sim900.available()!=0)
  Serial.write(sim900.read());
  delay(5000); 
  
}


