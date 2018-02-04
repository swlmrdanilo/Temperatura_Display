/*
  File modulo Clock DS3231 + DHT22 su display + Dispaly SH1106 128X64 + BMP280
  Ultima mod.4/02/2018
  Aggiunto sensore BMP280
  VCC <----> 3.3v
  GND <----> GND
  SCL/SCK <----> A5(Analog pin 5)
  SDA/SDI <----> A4(Analog pin 4)

  Universal 8bit Graphics Library, https://github.com/olikraus/u8glib/

*/


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
  return ( (val / 10 * 16) + (val % 10) );
}
// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val)
{
  return ( (val / 16 * 10) + (val % 16) );
}

// -------- parametri per display ------------
//U8GLIB_SH1106_128X64 u8g(10, 9, 12, 11, 13); // GND  -> GND VCC  -> 5V / 3v3 CLK -> 10 MOSI  -> 9 RES  -> 13 DC  -> 11 CS  -> 12
U8GLIB_SH1106_128X64 u8g(10, 9, 6, 8, 7); // GND  -> GND VCC  -> 5V CLK -> 10 MOSI  -> 9 RES  -> 7 DC  -> 8 CS  -> 6

// ----- gestione delle Temperature Max e Min

float maxTemp = -100;
float minTemp = 100;
float previousTemp;

// --- gestione freccie temperature ---
unsigned long previousCount1 = 0;
unsigned long previousCount2 = 0;
unsigned long currentCount;
// --- gestione scorrimento pagine
unsigned long previousCount3 = 0;
unsigned long previousCount4 = 0;
//unsigned long currentCount;


void setup(void)
{

  // ----------- DS3231 -------------
  Wire.begin();//scrivo i dati del modulo DS3231
  Serial.begin(9600);
  // set the initial time here:
  // DS3231 seconds, minutes, hours, day, date, month, year
  //setDS3231time(00,35,17,7,25,11,17);// setta ora


  u8g.setColorIndex(1);
  pinMode(12, OUTPUT);
  dht.begin();//scrivo i dati del sensore DHT22
  bme.begin();//scrivo i dati del sensore BMP22


  // flip screen, if required
  //u8g.setRot180();

}

void setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte
                   dayOfMonth, byte month, byte year)
{
  // sets time and date data to DS3231
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set next input to start at the seconds register
  Wire.write(decToBcd(second)); // set seconds
  Wire.write(decToBcd(minute)); // set minutes
  Wire.write(decToBcd(hour)); // set hours
  Wire.write(decToBcd(dayOfWeek)); // set day of week (1=Sunday, 7=Saturday)
  Wire.write(decToBcd(dayOfMonth)); // set date (1 to 31)
  Wire.write(decToBcd(month)); // set month
  Wire.write(decToBcd(year)); // set year (0 to 99)
  Wire.endTransmission();
}
void readDS3231time(byte *second,
                    byte *minute,
                    byte *hour,
                    byte *dayOfWeek,
                    byte *dayOfMonth,
                    byte *month,
                    byte *year)
{
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set DS3231 register pointer to 00h
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
  // request seven bytes of data from DS3231 starting from register 00h
  *second = bcdToDec(Wire.read() & 0x7f);
  *minute = bcdToDec(Wire.read());
  *hour = bcdToDec(Wire.read() & 0x3f);
  *dayOfWeek = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month = bcdToDec(Wire.read());
  *year = bcdToDec(Wire.read());
}


void loop(void)
{

  if (millis() - previousCount1 > 6000) { //memorizzo la temperatura ogni 60 sec
    previousTemp = dht.readTemperature();
    previousCount1 = millis();
  }

  if (millis() - previousCount3 >= 1000) {
    previousCount3 = millis();
    u8g.firstPage();
    do
    {
      draw1();
    } while ( u8g.nextPage() );
  }

  if (millis() - previousCount4 >= 30000) {

    previousCount4 = millis();
    u8g.firstPage();
    do
    {

      draw2();

    } while ( u8g.nextPage() );
    delay(9000);
  }
}


void draw1(void)
{

  // ---- Gestione DS3231 ----
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  // retrieve data from DS3231
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);

  //Grafica del display draw1
  u8g.drawFrame(0, 0, 128, 64); //disegno la cornice
  //u8g.drawLine( 7, 15, 121, 15);//disegno la riga sotto al titolo
  u8g.setFont(u8g_font_6x12);
  u8g.drawStr( 39, 20, ":");
  u8g.drawStr( 83, 20, ":");
  u8g.setFont(u8g_font_6x12);
  u8g.drawStr( 84, 57, "/");
  u8g.drawStr( 104, 57, "/");

  //Ora
  u8g.setFont(u8g_font_freedoomr25n);
  u8g.setPrintPos( 3, 40);
  if (hour < 10) u8g.print("0"); u8g.print(hour, DEC); //Scrive l'ora aggiungendo 0 se una cifra
  u8g.setFont(u8g_font_freedoomr25n);
  u8g.setPrintPos( 45, 40);
  if (minute < 10) u8g.print("0"); u8g.print(minute, DEC); //Scrive i minuti aggiungendo 0 se una cifra
  u8g.setFont(u8g_font_freedoomr25n);
  u8g.setPrintPos( 87, 40);
  if (second < 10) u8g.print("0"); u8g.print(second, DEC); //Scrive i minuti aggiungendo 0 se una cifra

  //Datario
  u8g.setFont(u8g_font_6x12);
  u8g.setPrintPos( 8, 57);
  //prima di scrivere il giorno lo converto in lettere
  switch (dayOfWeek) {
    case 1:
      u8g.print("Sunday");
      break;
    case 2:
      u8g.print("Monday");
      break;
    case 3:
      u8g.print("Tuesday");
      break;
    case 4:
      u8g.print("Wednesday");
      break;
    case 5:
      u8g.print("Thursday");
      break;
    case 6:
      u8g.print("Friday");
      break;
    case 7:
      u8g.print("Saturday");
      break;
  }

  // --- Data ---
  u8g.setFont(u8g_font_6x12);
  u8g.setPrintPos( 70, 57);
  if (dayOfMonth < 10) u8g.print("0"); u8g.print(dayOfMonth, DEC); //scrivo il giorno
  u8g.setFont(u8g_font_6x12);
  u8g.setPrintPos( 91, 57);
  if (month < 10) u8g.print("0"); u8g.print(month, DEC); //scrivo il mese
  u8g.setFont(u8g_font_6x12);
  u8g.setPrintPos( 111, 57);
  u8g.print(year, DEC); //scrivo l'anno


}
void draw2(void)// schermata con i dati meteo
{
  //Grafica del display draw1
  u8g.drawFrame(0, 0, 128, 64); //disegno la cornice
  


  //Gestione temperatura - umidità - HI
  float h = dht.readHumidity();//Umidità
  float t = dht.readTemperature();//Calcola la temperatura in Cesius
  float hic = dht.computeHeatIndex(t, h, false);//Calcola l'indice di calore in Cesius (isFahreheit = false)


  //valore temperatura
  u8g.setFont(u8g_font_profont15);
  //u8g.setFont(u8g_font_unifont);
  u8g.setPrintPos( 28, 34);
  u8g.print(dht.readTemperature(), 1); // aggiunto ",1" per un solo decimale
  u8g.setFont(u8g_font_profont10);
  u8g.drawStr( 57, 29, "o");
  u8g.drawStr( 6, 29, "Temp");
  // --- funzione freccia andamento temperatura ---
  if (millis() - previousCount2 > 12000) { //confronto la temperatura ogni 120 sec
    if (previousTemp > dht.readTemperature()) {
      u8g.setFont(u8g_font_6x12_67_75);
      u8g.drawStr(62, 35, "\x13");
      previousCount2 = currentCount;
    }
    else if (previousTemp < dht.readTemperature()) {
      u8g.setFont(u8g_font_6x12_67_75);
      u8g.drawStr(62, 35, "\x11");
      previousCount2 = currentCount;
    }
    else {
      u8g.setFont(u8g_font_04b_03b);
      u8g.drawStr(62, 35, "=");
      previousCount2 = currentCount;
    }
  }

  //valore indice calore
  u8g.setFont(u8g_font_profont15);
  u8g.setPrintPos( 87, 34);
  u8g.print(hic, 1); // aggiunto ",1" per un solo decimale
  //simbolo °
  u8g.setFont(u8g_font_profont10);
  u8g.drawStr( 119, 30, "o");
  u8g.setFont(u8g_font_profont10);
  u8g.drawStr( 71, 30, "HI");

  //valore temperatura Max
  //simbolo TMax
  u8g.setFont(u8g_font_profont10);
  u8g.drawStr( 6, 54, "TMax");
  u8g.setFont(u8g_font_profont11);
  u8g.setPrintPos( 30, 57);
  if (dht.readTemperature() >= maxTemp) {
    maxTemp = dht.readTemperature();
  } u8g.print(maxTemp, 1);
  //simbolo ° T massima
  u8g.setFont(u8g_font_profont10);
  u8g.drawStr( 56, 54, "o");

  //valore temperatura Min
  //simbolo TMin
  u8g.setFont(u8g_font_profont10);
  u8g.drawStr( 64, 54, "TMin");
  u8g.setFont(u8g_font_profont11);
  u8g.setPrintPos( 88, 57);
  if (dht.readTemperature() <= minTemp) {
    minTemp = dht.readTemperature();
  } u8g.print(minTemp, 1);
  //simbolo ° T minima
  u8g.setFont(u8g_font_profont10);
  u8g.drawStr( 114, 54, "o");

  //valore umidità
  u8g.setFont(u8g_font_profont11);
  u8g.setPrintPos( 71, 13);
  u8g.print(dht.readHumidity(), 1); // aggiunto ",1" per un solo decimale
  //simbolo %
  u8g.setFont(u8g_font_04b_03b);
  u8g.drawStr( 100, 13, "%");

  //valore pressione
  u8g.setFont(u8g_font_profont11);
  u8g.setPrintPos( 10, 14);
  u8g.print(bme.readPressure() / 100, 0); // 100 Pa = 1 millibar
  //simbolo hPa
  u8g.setFont(u8g_font_profont10);
  u8g.drawStr( 37, 11, "hPa");


  /* valore altezza
    u8g.setFont(u8g_font_profont11);
    u8g.setPrintPos( 57, 24);
    u8g.print(bme.readAltitude(1029),0);//modificare il valore tra le parentesi per un accurata altezza
    //simbolo m s.l.m.
    u8g.setFont(u8g_font_profont10);
    u8g.drawStr( 77, 22, "m");
  */


}
