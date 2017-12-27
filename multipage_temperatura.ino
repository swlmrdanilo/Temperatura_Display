
#include "Wire.h" //libreria comunicazione I2C
#include <DHT.h> //libreria sensore DHT22
#include "U8glib.h" //libreria per display
#define I_DHT22 14 //indica la porta dovè collegato il sensore DHT22 A0
#define DHTTYPE DHT22 //modello del sensore  DHT 22  (AM2302), AM2321
DHT dht(I_DHT22, DHTTYPE); //Inizializzazione del sensore
// ----- Sensore BMP280 -----
#include "SPI.h"
#include <Adafruit_Sensor.h>
#include "Adafruit_BMP280.h"
// #define BMP280_ADDRESS 0x76
// usa I2C
Adafruit_BMP280 bme;
// ------- parte DS3231 -------
#define DS3231_I2C_ADDRESS 0x68
// Convert normal decimal numbers to binary coded decimal
byte decToBcd(byte val)
{
  return( (val/10*16) + (val%10) );
}
// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val)
{
  return( (val/16*10) + (val%16) );
}

// -------- parametri per display ------------
U8GLIB_SH1106_128X64 u8g(10, 9, 12, 11, 13); // GND  -> GND VCC  -> 5V / 3v3 CLK -> 10 MOSI  -> 9 RES  -> 13 DC  -> 11 CS  -> 12

// ----- gestione delle Temperature Max e Min  
  float maxTemp=-100;
  float minTemp=100;
  
  
  
  


void setup(void)
{
  u8g.setColorIndex(1);
  pinMode(12, OUTPUT);
  dht.begin();//scrivo i dati del sensore DHT22
  bme.begin();//scrivo i dati del sensore BMP22
  
  
  // flip screen, if required
  //u8g.setRot180();




  
}
void loop(void)
{



  
  u8g.firstPage();
  do
  {
    draw1();
  } while ( u8g.nextPage() );

  delay(990);

  u8g.firstPage();
  do
  {
    draw2();
  } while ( u8g.nextPage() );

  delay(15990);
}



void draw1(void)
{

    
   //Grafica del display draw1
  u8g.drawFrame(0,0,128,64);//disegno la cornice
  //u8g.drawLine( 7, 15, 121, 15);//disegno la riga sotto al titolo
  u8g.setFont(u8g_font_6x12);
  u8g.drawStr( 39,40, ":");
  u8g.drawStr( 81,40, ":");
  u8g.setFont(u8g_font_6x12);
  u8g.drawStr( 84,57, "/");
  u8g.drawStr( 104,57, "/");
  //u8g.setFont(u8g_font_04b_03b);
  //u8g.drawStr( 81,11, "HI");



  
}
void draw2(void)// schermata con i dati meteo
{
  //Grafica del display draw1
  u8g.drawFrame(0,0,128,64);//disegno la cornice


 //Gestione temperatura - umidità - HI
  float h = dht.readHumidity();//Umidità
  float t = dht.readTemperature();//Calcola la temperatura in Cesius
  float hic = dht.computeHeatIndex(t, h, false);//Calcola l'indice di calore in Cesius (isFahreheit = false)
  upT=analogRead(dht.readTemperature(),1)
  
  

  //valore temperatura
  u8g.setFont(u8g_font_profont11);
  u8g.setPrintPos( 8, 13);
  u8g.print(dht.readTemperature(),1); // aggiunto ",1" per un solo decimale
  //simbolo °
  u8g.setFont(u8g_font_profont10);
  u8g.drawStr( 33, 10, "o");

 
  
/*
  if (dht.readTemperature()>upT) {
  u8g.setFont(u8g_font_6x12_67_75); 
  u8g.drawStr(15, 55, "\x11");}

  else if (dht.readTemperature()<maxTemp) {
  u8g.setFont(u8g_font_6x12_67_75); 
  u8g.drawStr(15, 55, "\x13");}
  

*/

/*
  u8g.setFont(u8g_font_profont11);
  u8g.setPrintPos( 8, 33);
  u8g.print(upT,2);
*/
  //valore temperatura max
  //simbolo TMax
  u8g.setFont(u8g_font_profont10);
  u8g.drawStr( 22, 52, "TMax");
  u8g.setFont(u8g_font_profont11);
  u8g.setPrintPos( 45, 55);
  if (dht.readTemperature()>=maxTemp){maxTemp = dht.readTemperature();} u8g.print(maxTemp,1);
  //simbolo °
  u8g.setFont(u8g_font_profont10);
  u8g.drawStr( 72, 52, "o");

  //valore temperatura min
  u8g.setFont(u8g_font_profont11);
  u8g.setPrintPos( 85, 55);
  if (dht.readTemperature()<=minTemp){minTemp = dht.readTemperature();} u8g.print(minTemp,1);
 
 //valore umidità
  u8g.setFont(u8g_font_profont11);
  u8g.setPrintPos( 45, 13);
  u8g.print(dht.readHumidity(),1); // aggiunto ",1" per un solo decimale
  //simbolo %
  u8g.setFont(u8g_font_04b_03b);
  u8g.drawStr( 71, 13, "%");  

 if (dht.readHumidity()>= 60){u8g.setPrintPos( 35, 33); u8g.setFont(u8g_font_04b_03b); 
  u8g.drawStr(15, 55, "NL");}
  else if (dht.readHumidity()<= 40){u8g.setPrintPos( 35, 33); u8g.setFont(u8g_font_04b_03b); 
  u8g.drawStr(15, 55, "NL");}

  
  

  //valore indice calore
  u8g.setFont(u8g_font_profont11);
  u8g.setPrintPos( 93, 13);
  u8g.print(hic,1); // aggiunto ",1" per un solo decimale
  //simbolo °
  u8g.setFont(u8g_font_profont10);
  u8g.drawStr( 119, 10, "o");

  //valore pressione
  u8g.setFont(u8g_font_profont11);
  u8g.setPrintPos( 8, 24);
  u8g.print(bme.readPressure() / 100,0); // 100 Pa = 1 millibar
  //simbolo hPa
  u8g.setFont(u8g_font_profont10);
  u8g.drawStr( 35, 22, "hPa");

  if (bme.readAltitude(1029)>=200) {u8g.setFont(u8g_font_unifont_76); u8g.drawStr( 77, 42, "\x22");}
  else if (bme.readAltitude(1029)<=200) {u8g.setFont(u8g_font_unifont_76); u8g.drawStr( 77, 42, "\x20");}
  

/* valore altezza
  u8g.setFont(u8g_font_profont11);
  u8g.setPrintPos( 57, 24);
  u8g.print(bme.readAltitude(1029),0);//modificare il valore tra le parentesi per un accurata altezza
  //simbolo m s.l.m.
  u8g.setFont(u8g_font_profont10);
  u8g.drawStr( 77, 22, "m"); 
  */

  
}
