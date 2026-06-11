#include <Arduino.h>
#include <LiquidCrystal.h>
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11
#define BUZZER_PIN 3
#define MQ135_PIN  A0

LiquidCrystal lcd(7,8,9,10,11,12);
DHT dht(DHTPIN, DHTTYPE);
bool mqEnabled = false;

void beep(int ms){
  digitalWrite(BUZZER_PIN, HIGH);
  delay(ms);
  digitalWrite(BUZZER_PIN, LOW);
}

void setup(){
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
  Serial.begin(9600);
  dht.begin();
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Air Quality Monitor");
  lcd.setCursor(0,1);
  lcd.print("Starting up...");
  delay(1200);
  beep(100);
}

void loop(){
  delay(2000);
  float humid = dht.readHumidity();
  float temp = dht.readTemperature();
  if(isnan(humid)||isnan(temp)){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("DHT Sensor Error");
    lcd.setCursor(0,1);
    Serial.print("Check the wires!");
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  int airRaw = -1;
  int airPct = -1;

  if(mqEnabled){
    airRaw = analogRead(MQ135_PIN);
    airPct = map(airRaw, 0, 1023, 0, 100);
    airPct = constrain(airPct, 0, 100);
  }

  Serial.print("Temp: ");
  Serial.print(temp);
  Serial.print(" | Humidity: ");
  Serial.print(humid);
  Serial.print(" % ");

  if(mqEnabled){
    Serial.print("| Air Quality: ");
    Serial.print(airPct);
    Serial.print(" % (Raw: ");
    Serial.print(airRaw);
    Serial.print(")");
  }
  Serial.println();

  lcd.clear();
lcd.setCursor(0,0);
lcd.print("Temp: ");
lcd.print(temp, 1);
lcd.print("C H:");
lcd.print((int)humid);
  lcd.print("%");
  lcd.setCursor(0,1);

  if(mqEnabled){
    lcd.print("Air: ");
    lcd.print(airPct);
    lcd.print("% ");
    if(airPct>70){
      lcd.print("POOR");
      beep(200);
    }
    else if(airPct>40){
      lcd.print("FAIR");
    }
    else{
      lcd.print("GOOD");
    }
  }else{
    if(temp>30){
      lcd.print("Temp High!");
      beep(200);
    }
    else if(temp<15){
      lcd.print("Temp Low!");
      beep(200);
    }else if (humid>70){
      lcd.print("Humidity High!");
      beep(200);
    }
    else if(humid<30){
      lcd.print("Humidity Low!");
      beep(200);
    }
  }
}