
/*
  File Temperatura DHT22 e modulo Clock DS3231
  ultima mod.14/09/2017
  
  PrintTest.pde

  Universal 8bit Graphics Library, https://github.com/olikraus/u8glib/
  
*/

#include <DHT.h> //libreria sensore DHT22
#include "U8glib.h" //libreria per display
#define luce A1 //ingresso fotoresistenza
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

U8GLIB_SH1106_128X64 u8g(10, 9, 12, 11, 13); // GND  -> GND VCC  -> 5V / 3v3 CLK -> 10 MOSI  -> 9 RES  -> 13 DC  -> 11 CS  -> 12

void draw(void) {

u8g.drawFrame(0,0,128,64);//disegno la cornice
u8g.drawLine( 7, 15, 121, 15);//disegno la riga sotto al titolo

  
  // graphic commands to redraw the complete screen should be placed here  
  //u8g.setFont(u8g_font_helvR08);
  //u8g.setFont(u8g_font_lucasfont_alternate);
  //u8g.drawStr( 95, 9, "IW2EVH");
  
  //scrive il valore della fotoresistenza
  //u8g.setPrintPos( 0, 9);
  //u8g.print(analogRead(luce), DEC);//scriviamo il valore della luce

  //u8g.setFont(u8g_font_lucasfont_alternate);
  //u8g.drawStr( 0, 9, "Luce:");
  
  u8g.setFont(u8g_font_7x13Br);
  u8g.drawStr( 36,12, "Ambiente");
  u8g.setFont(u8g_font_freedoomr10r);
  u8g.drawStr( 9,34, "T");
  u8g.setFont(u8g_font_freedoomr10r);
  u8g.drawStr( 65,34, "U");
  u8g.setFont(u8g_font_lucasfont_alternate);
  u8g.drawStr( 12,48, "Heat Index");

  //Gestione temperatura - umidità - HI
  float h = dht.readHumidity();//Umidità
  float t = dht.readTemperature();//Calcola la temperatura in Cesius
  float hic = dht.computeHeatIndex(t, h, false);//Calcola l'indice di calore in Cesius (isFahreheit = false)
  
//clock
  //u8g.setFont(u8g_font_7x13Br);
  //u8g.setPrintPos( 36, 12);
  //u8g.print("Danilo");

//valore temperatura
  u8g.setFont(u8g_font_freedoomr10r);
  u8g.setPrintPos( 23, 36);
  u8g.print(dht.readTemperature(),1); // aggiunto ",1" per un solo decimale
//simbolo °
  u8g.setFont(u8g_font_lucasfont_alternate);
  u8g.drawStr( 52, 29, "o");

//valore umidità
  u8g.setFont(u8g_font_freedoomr10r);
  u8g.setPrintPos( 82, 36);
  u8g.print(dht.readHumidity(),1); // aggiunto ",1" per un solo decimale
//simbolo %
  u8g.setFont(u8g_font_lucasfont_alternate);
  u8g.drawStr( 114, 34, "%");

//valore indice calore
  u8g.setFont(u8g_font_freedoomr10r);
  u8g.setPrintPos( 70, 51);
  u8g.print(hic,1); // aggiunto ",1" per un solo decimale
//simbolo °
  u8g.setFont(u8g_font_lucasfont_alternate);
  u8g.drawStr( 102, 44, "o");



//gestione dell'intensità luce sul display
 int ValoreLuce=analogRead(luce);

  if(ValoreLuce<=740)
{ 
  u8g.setFont(u8g_font_lucasfont_alternate);
  u8g.setPrintPos( 34,61);
  u8g.print("Luce Scarsa");//scrivo il valore della luce
}
 
  if(ValoreLuce>740 && ValoreLuce<=830)
{
  
  u8g.setFont(u8g_font_lucasfont_alternate);
  u8g.setPrintPos( 34,61);
  u8g.print("Luce Normale");//scrivo il valore della luce
  
} 
  if(ValoreLuce>830)
  {
  u8g.setFont(u8g_font_lucasfont_alternate);
  u8g.setPrintPos( 34,61);
  u8g.print("Luce Intensa");//scrivo il valore della luce
  }
}

// ------- invio alla seriale DS3231 -------
void displayTime()
{
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  // retrieve data from DS3231
  
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,
  &year);
  
  // send it to the serial monitor
   Serial.print(hour, DEC);
  // convert the byte variable to a decimal number when displayed
  Serial.print(":");
  if (minute<10)
  {
    Serial.print("0");
  }
  Serial.print(minute, DEC);
  Serial.print(":");
  if (second<10)
  {
    Serial.print("0");
  }
  Serial.print(second, DEC);
  Serial.print(" ");
  Serial.print(dayOfMonth, DEC);
  Serial.print("/");
  Serial.print(month, DEC);
  Serial.print("/");
  Serial.print(year, DEC);
  Serial.print(" Day of week: ");
  switch(dayOfWeek){
  case 1:
    Serial.println("Sunday");
    break;
  case 2:
    Serial.println("Monday");
    break;
  case 3:
    Serial.println("Tuesday");
    break;
  case 4:
    Serial.println("Wednesday");
    break;
  case 5:
    Serial.println("Thursday");
    break;
  case 6:
    Serial.println("Friday");
    break;
  case 7:
    Serial.println("Saturday");
    break;
  }
}


// ------- fine DS3231 -------

void setup(void) {

// ----------- DS3231 -------------
Wire.begin();
  Serial.begin(9600);
  // set the initial time here:
  // DS3231 seconds, minutes, hours, day, date, month, year
  // setDS3231time(10,05,12,7,19,8,17);// setta ora

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

  dht.begin();

  u8g.setColorIndex(1);//scrivi con un pixel
  
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
              
  displayTime(); // display the real-time clock data on the Serial Monitor,
  
// picture loop
  u8g.firstPage();  
  do {
    draw();
  } while( u8g.nextPage() );
  
  // rebuild the picture after some delay
  delay(9000);
}


