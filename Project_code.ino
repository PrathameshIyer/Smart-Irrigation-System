
#include<SoftwareSerial.h>
SoftwareSerial Serial1(2,3);
#include<LiquidCrystal.h>
LiquidCrystal lcd(4,5,6,7,8,9);
int led=A1;
int flag=0;
String str="";
int motor =12;
int sensor =13;
void setup()
{
  lcd.begin(16,2);
  Serial1.begin(9600);
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  pinMode(motor, OUTPUT);
  pinMode(sensor, INPUT_PULLUP);
  lcd.print("AUTOMATIC WATER");
   lcd.setCursor(0,1);
    lcd.print("IRRIGATION SYS....");
  lcd.setCursor(4,1);
  delay(2000);
  lcd.clear();
  lcd.print("PRESENTED BY");
  lcd.setCursor(0,1);
  lcd.print("GROUP N0.:- A10");
  delay(2000);
   lcd.clear();
  lcd.print("..WELCOME YOU..");
  delay(2000);
  lcd.clear();
  lcd.print("PLEASE WAIT...");
  delay(2000);
  gsmInit();
  delay(1000);
  lcd.clear();
  lcd.print("System Ready");
}
void loop()
{
    lcd.setCursor(0,0);
    lcd.print("Automatic Mode");
    if(digitalRead(sensor)==1 && flag==0)
    {
      delay(1000);
      if(digitalRead(sensor)==1)
      {
        digitalWrite(led, HIGH);
        lcd.clear();
        lcd.print("Motor ON    ");
        delay(3000);
        sendSMS("Low Soil Moisture detected. Motor turned ON");
       
      
        digitalWrite(motor, HIGH);
        delay(5000);
        flag=1;
      }
    }
    else if(digitalRead(sensor)==0 && flag==1)
    {
      delay(1000);
      if(digitalRead(sensor)==0)
      {
        digitalWrite(led, LOW);
        lcd.clear();
       
        lcd.print("Motor OFF");
        delay(3000);
        sendSMS("Soil Moisture is Normal. Motor turned OFF");
        digitalWrite(motor, LOW);
       
      
        flag=0;
      }
    }
}
 
void sendSMS(String msg)
{
  lcd.clear();
  lcd.print("Sending SMS");
  Serial1.println("AT+CMGF=1");
  delay(500);
  Serial1.print("AT+CMGS=");
  Serial1.print('"');
  Serial1.print("+918830031894");    // number
  Serial1.print('"');
  Serial1.println();
  delay(500);
  Serial1.println(msg);
  delay(500);
  Serial1.write(26);
  delay(1000);
  lcd.clear();
  lcd.print("SMS Sent");
  delay(1000);
  lcd.begin(16,2);
}
void gsmInit()
{
  lcd.clear();
  lcd.print("Finding Module..");
  boolean at_flag=1;
  while(at_flag)
  {
    Serial1.println("AT");
    while(Serial1.available()>0)
    {
      if(Serial1.find("OK"))
      at_flag=0;
    }
    delay(1000);
  }
  Serial1.println("ATE0");
  lcd.clear();
  lcd.print("Finding Network..");
  boolean net_flag=1;
  while(net_flag)
  {
    Serial1.println("AT+CPIN?");
    while(Serial1.available()>0)
    {
      if(Serial1.find("READY"))
      net_flag=0;
      break;
    }
    delay(1000);
  }
   Serial1.println("AT+CNMI=2,2,0,0,0");
   delay(1000);
   Serial1.println("AT+CMGF=1");
   delay(1000);
   Serial1.println("AT+CSMP=17,167,0,0");
   lcd.clear();
   Serial1.flush();
}

#define BLYNK_PRINT Serial
#define Motor_A_1A 12 // Motor A PWM Speed
#define Motor_A_1B 14 // Motor A Direction

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "wfE9bfe0echnYDj02sJ0qmJJsoA-Oz3Y";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Realme";
char pass[] = "12345678";

int sensorData;

BlynkTimer timer;

// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void myTimerEvent()
{
  sensorData = analogRead(A0);
   Serial.print(" Moisture=");
   Serial.println(sensorData);
  if (sensorData > 900)
  {
    digitalWrite( Motor_A_1B, HIGH );
    analogWrite( Motor_A_1A, 128); 
   // Serial.println("Hi");
   Blynk.notify("Alert : Plant Need water ");
   Blynk.email("prathmiyer18@gmail.com","plant watering system","Alert : Plant Need water ");
  }
  else
  {
    digitalWrite( Motor_A_1B, LOW);
    analogWrite( Motor_A_1A, 0); 
    //Serial.println("Lo");
  }
  
  Blynk.virtualWrite(V5, sensorData);
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
}

void setup()
{
  // Debug console
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  pinMode( Motor_A_1B, OUTPUT );
  pinMode( Motor_A_1A, OUTPUT );
  digitalWrite( Motor_A_1B, LOW );
  digitalWrite( Motor_A_1A, LOW );

  // Setup a function to be called every second
  timer.setInterval(1000L, myTimerEvent);
}

void loop()
{
  Blynk.run();
  timer.run(); // Initiates BlynkTimer
}
