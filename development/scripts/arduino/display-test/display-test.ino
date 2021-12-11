#include <U8g2lib.h>
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0);
 
void setup(void) {
  u8g2.begin();
}



void loop(void) {
  u8g2.firstPage();
  do {
    //keep this the same as it pages through the data generating the screen
    u8g2.setFont(u8g2_font_ncenB14_tr);
    u8g2.drawStr(0,20,"2.04   C");
  } while ( u8g2.nextPage() );
 
  delay(1000); 
}
