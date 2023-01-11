#define BLYNK_TEMPLATE_ID "put your template id"
#define BLYNK_DEVICE_NAME "put your preffered device name"
#define BLYNK_AUTH_TOKEN "put your token"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <DHT.h>;
//#include <TimerOne.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define DHTPIN 5     // what pin we're connected to DEFINE DHT CODE////////////////////
#define DHTTYPE DHT22   // DHT 22
DHT dht(DHTPIN, DHTTYPE);
uint8_t DHTPin = D1; 

int const trigPin = 2;//12;
int const echoPin = 0;//13;
int const buzzPin = 14;//11;
const int green= 15;//2;
const int orange= 13;//3;
const int red= 12;//4;

char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "put your wifi id";
char pass[] = "put your wifi password";

BlynkTimer timer;

int notificationMode = 0;

//Variables
//int chk;
int h;  //Stores humidity value
int t; //Stores temperature value



//-----------------------------------BLYNK TEST---------------------//
void myTimerEvent()
{

   long dur;
long dist;
long per;
digitalWrite(trigPin,LOW);
delayMicroseconds(2);
digitalWrite(trigPin,HIGH);
delayMicroseconds(10);
digitalWrite(trigPin,LOW);
dur=pulseIn(echoPin,HIGH);
dist=(dur*0.034)/2;

  per=map(dist,10.5,2,0,100);
  h = dht.readTemperature(); // Gets the values of the temperature
  t = dht.readHumidity();



  Blynk.virtualWrite(V1, per);
  Blynk.virtualWrite(V2, h);
  Blynk.virtualWrite(V3, t);

  
  if(per > 80){
    if(notificationMode != 2){
      notificationMode = 2;
      Blynk.logEvent("notify", "Water level is high");
    }
  }
  else if(per > 55 && dist < 80){
    if(notificationMode != 3){
      notificationMode = 3;
      Blynk.logEvent("notify", "Water level is medium");
    }
  }
  else{
    notificationMode = 0; //Reset Mptification
  }
}



//-----------------------------------BLYNK TEST---------------------//


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
 Blynk.begin(auth, ssid, pass);
  lcd.begin();
  dht.begin();
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(orange, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(buzzPin, OUTPUT);
  digitalWrite(green,HIGH);
  digitalWrite(orange,HIGH);
  digitalWrite(red,HIGH);
  digitalWrite(buzzPin,LOW);
  lcd.setCursor(0,0);
 lcd.print(" Flood Sensing");
 lcd.setCursor(0,1);
 lcd.print("by Tengku Izham 2022");
 delay(2000);
 lcd.clear();
 lcd.print("     Welcome");
 delay(2000);
 lcd.clear();
timer.setInterval(1000L, myTimerEvent);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  long dur;
long dist;
long per;
digitalWrite(trigPin,LOW);
delayMicroseconds(2);
digitalWrite(trigPin,HIGH);
delayMicroseconds(10);
digitalWrite(trigPin,LOW);
dur=pulseIn(echoPin,HIGH);
dist=(dur*0.034)/2;
//dist=(dur/2) /29.5;
per=map(dist,10.5,2,0,100);

//--------------------//

h = dht.readHumidity();
    t = dht.readTemperature();
    
 Blynk.run();
  timer.run(); // Initiates BlynkTimer
//---------------------//

if(per<0)
{
  per=0;
}
if(per>100)
{
  per=100;
}
Serial.println(String(per));
lcd.setCursor(0,0);
lcd.print("Water Level:");
lcd.print(String(per));
lcd.print("%  ");
if(per>=80)       
{
  lcd.setCursor(0,1);
  lcd.print("Red Alert!   ");

 


  digitalWrite(green,LOW);
  digitalWrite(orange,LOW);
  digitalWrite(red,HIGH);

  digitalWrite(buzzPin,HIGH);
  delay(2000);
  digitalWrite(buzzPin,LOW);
  delay(2000);
  digitalWrite(buzzPin,HIGH);
  delay(2000);
  digitalWrite(buzzPin,LOW);
  delay(5000);


   lcd.clear();
    
}
else if(per>=55)   
{
  lcd.setCursor(0,1);
  lcd.print("Orange Alert!  ");
  digitalWrite(orange,HIGH);
  digitalWrite(green,LOW);
  digitalWrite(red,LOW);
  digitalWrite(buzzPin,HIGH);
  delay(3000);
  digitalWrite(buzzPin,LOW);
  delay(5000);
lcd.clear();


   
  
}else          
{
 lcd.setCursor(0,1);
 lcd.print("Green Alert!  ");
 digitalWrite(green,HIGH); 
 digitalWrite(orange,LOW);
 digitalWrite(red,LOW);
 digitalWrite(buzzPin,LOW);
delay(5000);
 lcd.clear();

 lcd.setCursor(0, 0);
    lcd.println("Humidity & Temp ");
    
    lcd.setCursor(0, 1);
    lcd.print("");
    lcd.print(t);
   lcd.print("%");

    //lcd.setCursor(6, 1);
   // lcd.println(" ");
     
    lcd.setCursor(6, 1);
    lcd.print("");
    lcd.print(h);
    lcd.print("c");

}

delay(5000);

}
