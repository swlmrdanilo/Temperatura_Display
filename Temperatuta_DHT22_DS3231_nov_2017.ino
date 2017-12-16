
/*
  File modulo Clock DS3231 + DHT22 su display + Dispaly SH1106 128X64
  Ultima mod.25/11/2017
  Aggiunto giorno delle settimana e sistemato data
  
  PrintTest.pde

  Universal 8bit Graphics Library, https://github.com/olikraus/u8glib/
  
*/

#include <DHT.h> //libreria sensore DHT22
#include "U8glib.h" //libreria per display
#define I_DHT22 14 //indica la porta dovè collegato il sensore DHT22 A0
#define DHTTYPE DHT22 //modello del sensore  DHT 22  (AM2302), AM2321
DHT dht(I_DHT22, DHTTYPE); //Inizializzazione del sensore


// ------- parte DS3231 -------
#include "Wire.h"
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

// ------- fine DS3231 -------

// setup u8g object, please remove comment from one of the following constructor calls
// IMPORTANT NOTE: The following list is incomplete. The complete list of supported 
// devices with all constructor calls is here: https://github.com/olikraus/u8glib/wiki/device

// -------- parametri per display ------------
U8GLIB_SH1106_128X64 u8g(10, 9, 12, 11, 13); // GND  -> GND VCC  -> 5V / 3v3 CLK -> 10 MOSI  -> 9 RES  -> 13 DC  -> 11 CS  -> 12



void displayTime()
{


//Gestione temperatura - umidità - HI
  float h = dht.readHumidity();//Umidità
  float t = dht.readTemperature();//Calcola la temperatura in Cesius
  float hic = dht.computeHeatIndex(t, h, false);//Calcola l'indice di calore in Cesius (isFahreheit = false)

  
byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  // retrieve data from DS3231
readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,&year);

  //Grafica del display
  u8g.drawFrame(0,0,128,64);//disegno la cornice
  //u8g.drawLine( 7, 15, 121, 15);//disegno la riga sotto al titolo
  u8g.setFont(u8g_font_freedoomr10r);
  u8g.drawStr( 39,40, ":");
  u8g.drawStr( 81,40, ":");
  u8g.setFont(u8g_font_6x12);
  u8g.drawStr( 84,57, "/");
  u8g.drawStr( 104,57, "/");
  u8g.setFont(u8g_font_04b_03b);
  u8g.drawStr( 81,11, "HI");

  //Ora
  u8g.setFont(u8g_font_helvR24r);
  u8g.setPrintPos( 4,45);
  u8g.print(hour,DEC);
  u8g.setFont(u8g_font_helvR24r);
  u8g.setPrintPos( 45,45);
  u8g.print(minute,DEC);
  u8g.setFont(u8g_font_helvR24r);
  u8g.setPrintPos( 89,45);
  u8g.print(second,DEC);

  //Datario
  u8g.setFont(u8g_font_6x12);
  u8g.setPrintPos( 8,57);
 //prima di scrivere il giorno lo converto in lettere
  switch(dayOfWeek){
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
  u8g.setFont(u8g_font_6x12);
  u8g.setPrintPos( 70,57);
  u8g.print(dayOfMonth,DEC);//scrivo il giorno
  u8g.setFont(u8g_font_6x12);
  u8g.setPrintPos( 91,57);
  u8g.print(month,DEC); //scrivo il mese
  u8g.setFont(u8g_font_6x12);
  u8g.setPrintPos( 111,57);
  u8g.print(year,DEC); //scrivo l'anno
 
  //valore temperatura
  u8g.setFont(u8g_font_profont11);
  u8g.setPrintPos( 8, 13);
  u8g.print(dht.readTemperature(),1); // aggiunto ",1" per un solo decimale
  //simbolo °
  u8g.setFont(u8g_font_profont10);
  u8g.drawStr( 33, 10, "o");

  //valore umidità
  u8g.setFont(u8g_font_profont11);
  u8g.setPrintPos( 45, 13);
  u8g.print(dht.readHumidity(),1); // aggiunto ",1" per un solo decimale
  //simbolo %
  u8g.setFont(u8g_font_04b_03b);
  u8g.drawStr( 71, 13, "%");  

  //valore indice calore
  u8g.setFont(u8g_font_profont11);
  u8g.setPrintPos( 93, 13);
  u8g.print(hic,1); // aggiunto ",1" per un solo decimale
  //simbolo °
  u8g.setFont(u8g_font_profont10);
  u8g.drawStr( 119, 10, "o");

}

void setup(void) {   

  dht.begin();

// ----------- DS3231 -------------
  Wire.begin();
  Serial.begin(9600);
  // set the initial time here:
  // DS3231 seconds, minutes, hours, day, date, month, year
   //setDS3231time(00,35,17,7,25,11,17);// setta ora

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

// ---------------------------------

  

  u8g.setColorIndex(2);//scrivi con un pixel
  
  // flip screen, if required
  // u8g.setRot180();
  
  // set SPI backup if required
  //u8g.setHardwareBackup(u8g_backup_avr_spi);

  // assign default color value
  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) {
    u8g.setColorIndex(255);     // white
  }
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT ) {
    u8g.setColorIndex(3);         // max intensity
  }
  else if ( u8g.getMode() == U8G_MODE_BW ) {
    u8g.setColorIndex(1);         // pixel on
  }
  else if ( u8g.getMode() == U8G_MODE_HICOLOR ) {
    u8g.setHiColorByRGB(255,255,255);
  }

//  cnt = 3;


}

void loop(void) {

  
  
//picture loop
  u8g.firstPage();  
  do {
   displayTime();
 //  draw();
   } while( u8g.nextPage() );
  
  // rebuild the picture after some delay
  delay(5);
}



