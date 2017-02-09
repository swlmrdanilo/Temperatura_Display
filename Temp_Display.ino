
/*

  PrintTest.pde

  Universal 8bit Graphics Library, https://github.com/olikraus/u8glib/

*/
#define luce A1 //ingresso fotoresistenza
#include "U8glib.h" //libreria per display
#include <OneWire.h> //libreria sensore
#include <DallasTemperature.h> //libreria sensore
#define ONE_WIRE_BUS 14 //indica la porta dovè collegato il sensore A0
OneWire ourWire(ONE_WIRE_BUS);
DallasTemperature sensors(&ourWire);

// setup u8g object, please remove comment from one of the following constructor calls
// IMPORTANT NOTE: The following list is incomplete. The complete list of supported 
// devices with all constructor calls is here: https://github.com/olikraus/u8glib/wiki/device

U8GLIB_SH1106_128X64 u8g(10, 9, 12, 11, 13); // GND  -> GND VCC  -> 5V / 3v3 CLK -> 10 MOSI  -> 9 RES  -> 13 DC  -> 11 CS  -> 12


void draw(void) {
  // graphic commands to redraw the complete screen should be placed here  
  //u8g.setFont(u8g_font_helvR08);
  u8g.setFont(u8g_font_lucasfont_alternate);
  u8g.drawStr( 95, 9, "IW2EVH");
  
  //scrive il valore della fotoresistenza
  u8g.setPrintPos( 0, 9);
  u8g.print(analogRead(luce), DEC);//scriviamo il valore della luce

  //u8g.setFont(u8g_font_lucasfont_alternate);
  //u8g.drawStr( 0, 9, "Luce:");
  
  u8g.setFont(u8g_font_unifont);
  u8g.drawStr( 8,30, "Temp. Ambiente");

// call procedure from base class, http://arduino.cc/en/Serial/Print
  u8g.setFont(u8g_font_freedoomr10r);
  u8g.setPrintPos( 49, 60);
  u8g.print(sensors.getTempCByIndex(0),1); // aggiunto ",1" per un solo decimale

  u8g.setFont(u8g_font_lucasfont_alternate);
  u8g.drawStr( 79, 51, "o");


 int ValoreLuce=analogRead(luce);

  if(ValoreLuce<=700)
{
  u8g.setFont(u8g_font_lucasfont_alternate);
  u8g.setPrintPos( 25, 9);
  u8g.print("Luce Scarsa");//scrivo il valore della luce
}
 
  if(ValoreLuce>700 && ValoreLuce<=830)
{
  u8g.setFont(u8g_font_lucasfont_alternate);
  u8g.setPrintPos( 25, 9);
  u8g.print("Poca Luce");//scrivo il valore della luce
} 
  if(ValoreLuce>830)
  {
  u8g.setFont(u8g_font_lucasfont_alternate);
  u8g.setPrintPos( 24, 9);
  u8g.print("Luce Normale");//scrivo il valore della luce
  }
}

void setup(void) {

  sensors.begin();
  
  
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

  //cnt = 3;

}

void loop(void) {
                
  sensors.requestTemperatures(); 

  
  
// picture loop
  u8g.firstPage();  
  do {
    draw();
  } while( u8g.nextPage() );
  
  // rebuild the picture after some delay
  delay(500);
}



