


/*
  temp_Display con DHT22
  ultima mod.09_02_2017
  
  PrintTest.pde

  Universal 8bit Graphics Library, https://github.com/olikraus/u8glib/
  
*/
#include <DHT.h> //libreria sensore DHT22
#include "U8glib.h" //libreria per display
#define luce A1 //ingresso fotoresistenza
#define I_DHT22 14 //indica la porta dovè collegato il sensore DHT22 A0
#define DHTTYPE DHT22 //modello del sensore  DHT 22  (AM2302), AM2321
DHT dht(I_DHT22, DHTTYPE); //Inizializzazione del sensore


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
  u8g.drawStr( 11,48, "Heat Index");

  //Gestione temperatura - umidità - HI
  float h = dht.readHumidity();//Umidità
  float t = dht.readTemperature();//Calcola la temperatura in Cesius
  float hic = dht.computeHeatIndex(t, h, false);//Calcola l'indice di calore in Cesius (isFahreheit = false)

//valore temperatura, http://arduino.cc/en/Serial/Print
  u8g.setFont(u8g_font_freedoomr10r);
  u8g.setPrintPos( 23, 36);
  u8g.print(dht.readTemperature(),1); // aggiunto ",1" per un solo decimale
//simbolo °
  u8g.setFont(u8g_font_lucasfont_alternate);
  u8g.drawStr( 52, 29, "o");

//valore umidità, http://arduino.cc/en/Serial/Print
  u8g.setFont(u8g_font_freedoomr10r);
  u8g.setPrintPos( 82, 36);
  u8g.print(dht.readHumidity(),1); // aggiunto ",1" per un solo decimale
//simbolo %
  u8g.setFont(u8g_font_lucasfont_alternate);
  u8g.drawStr( 114, 34, "%");

//valore indice calore, http://arduino.cc/en/Serial/Print
  u8g.setFont(u8g_font_freedoomr10r);
  u8g.setPrintPos( 69, 51);
  u8g.print(hic,1); // aggiunto ",1" per un solo decimale
//simbolo °
  u8g.setFont(u8g_font_lucasfont_alternate);
  u8g.drawStr( 100, 44, "o");



//gestione dell'intensità luce sul display
 int ValoreLuce=analogRead(luce);

  if(ValoreLuce<=720)
{ 
  u8g.setFont(u8g_font_lucasfont_alternate);
  u8g.setPrintPos( 34,61);
  u8g.print("Luce Scarsa");//scrivo il valore della luce
}
 
  if(ValoreLuce>720 && ValoreLuce<=830)
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



void setup(void) {

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
                
  
  
// picture loop
  u8g.firstPage();  
  do {
    draw();
  } while( u8g.nextPage() );

  
  
  // rebuild the picture after some delay
  delay(1000);
}



