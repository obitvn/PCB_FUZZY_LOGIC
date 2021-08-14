//Include thư viện

#include <luatmo2ngovao.h>



#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal.h>

// Chân nối với Arduino
#define TEMP_IN 10
//Thiết đặt thư viện onewire
OneWire Temp_in_ds18b20(TEMP_IN);
//Mình dùng thư viện DallasTemperature để đọc cho nhanh
DallasTemperature CamBienDauVao(&Temp_in_ds18b20);

// Chân nối với Arduino
#define TEMP_OUT 11
//Thiết đặt thư viện onewire
OneWire Temp_out_ds18b20(TEMP_OUT);
//Mình dùng thư viện DallasTemperature để đọc cho nhanh
DallasTemperature CamBienDauRa(&Temp_out_ds18b20);



unsigned char nhiet_in, nhiet_out, fan_duty, heat_duty;
unsigned char mode ;

LiquidCrystal lcd(A5, A4, A0, A1, A2, A3);

#define START 2

void setup(void)
{
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Init Sensor");
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  digitalWrite(5,0);
  digitalWrite(6,0);
  pinMode(START, INPUT);
  CamBienDauVao.begin();
  CamBienDauRa.begin();
  delay(1000);
  lcd.setCursor(0, 0);
  lcd.print("Done        ");
}

void control_duty_fan(uint8_t percent_duty)
{
 analogWrite(5,(percent_duty*255)/100);
}

void control_duty_heat(uint8_t percent_duty)
{
 analogWrite(6,(percent_duty*255)/100);
}


void scan_button()
{
  if(digitalRead(START) == 0)
  {
   
    while (digitalRead(START) == 0) ;
      {
      };
    
    mode ++;
    if(mode > 1) mode = 0;
  }
}

void loop(void)
{ 
 
  scan_button();
  if(mode==0)
  {
    lcd.setCursor(0, 0);
    lcd.print("SLEEP...      ");
    for(int i=0; i<17; i++)
    {
      lcd.setCursor(i, 1);
      lcd.print(" +>");
      lcd.setCursor(16-i, 1);
      lcd.print("<- ");
      delay(200);
      scan_button();
    }
  }
  else
  {
  CamBienDauVao.requestTemperatures();  
  CamBienDauRa.requestTemperatures(); 
  nhiet_in = CamBienDauVao.getTempCByIndex(0); 
  nhiet_out = CamBienDauRa.getTempCByIndex(0); 
  FUZZY_SetPwmDuty( nhiet_in, nhiet_out, &fan_duty, &heat_duty);
  control_duty_fan(fan_duty);
  control_duty_heat(heat_duty);
  lcd.setCursor(0, 0);
  lcd.print("IN: ");
  lcd.print(nhiet_in);
  lcd.print(" FAN: ");
  lcd.print(fan_duty);
  lcd.setCursor(0, 1);
  lcd.print("OUT: ");
  lcd.print(nhiet_out);
  lcd.print(" HEAT: ");
  lcd.print(heat_duty);
  
  }
}
