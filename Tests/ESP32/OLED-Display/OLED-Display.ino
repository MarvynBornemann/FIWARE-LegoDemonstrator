#include <U8g2lib.h>
#include <Wire.h>
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);
const char COPYRIGHT_SYMBOL[] = { 0xa9, '\0' };
void u8g2_prepare() {
 u8g2.setFont(u8g2_font_6x10_tf);
 u8g2.setFontRefHeightExtendedText();
 u8g2.setDrawColor(1);
 u8g2.setFontPosTop();
 u8g2.setFontDirection(0);
}
void u8g2_box_frame() {
 u8g2.drawStr(0, 0, "drawBox");
 u8g2.drawBox(5, 10, 20, 10);
 u8g2.drawStr(60, 0, "drawFrame");
 u8g2.drawFrame(65, 10, 20, 10);
}
void u8g2_r_frame_box() {
 u8g2.drawStr(0, 0, "drawRFrame");
 u8g2.drawRFrame(5, 10, 40, 15, 3);
 u8g2.drawStr(70, 0, "drawRBox");
 u8g2.drawRBox(70, 10, 25, 15, 3);
}
void u8g2_disc_circle() {
 u8g2.drawStr(0, 0, "drawDisc");
 u8g2.drawDisc(10, 18, 9);
 u8g2.drawDisc(30, 16, 7);
 u8g2.drawStr(60, 0, "drawCircle");
 u8g2.drawCircle(70, 18, 9);
 u8g2.drawCircle(90, 16, 7);
}

void u8g2_string_orientation() {
 u8g2.setFontDirection(0);
 u8g2.drawStr(5, 15, "0");
 u8g2.setFontDirection(3);
 u8g2.drawStr(40, 25, "90");
 u8g2.setFontDirection(2);
 u8g2.drawStr(75, 15, "180");
 u8g2.setFontDirection(1);
 u8g2.drawStr(100, 10, "270");
}
void u8g2_line() {
 u8g2.drawStr( 0, 0, "drawLine");
 u8g2.drawLine(7, 10, 40, 32);
 u8g2.drawLine(14, 10, 60, 32);
 u8g2.drawLine(28, 10, 80, 32);
 u8g2.drawLine(35, 10, 100, 32);
}
void u8g2_triangle() {
 u8g2.drawStr( 0, 0, "drawTriangle");
 u8g2.drawTriangle(14, 7, 45, 30, 10, 32);
}
void u8g2_unicode() {
 u8g2.drawStr(0, 0, "Unicode");
 u8g2.setFont(u8g2_font_unifont_t_symbols);
 u8g2.setFontPosTop();
 u8g2.setFontDirection(0);
 u8g2.drawUTF8(10, 15, "☀");
 u8g2.drawUTF8(30, 15, "☁");
 u8g2.drawUTF8(50, 15, "☂");
 u8g2.drawUTF8(70, 15, "☔");
 u8g2.drawUTF8(95, 15, COPYRIGHT_SYMBOL); //COPYRIGHT SIMBOL
 u8g2.drawUTF8(115, 15, "\xb0"); // DEGREE SYMBOL
}

void setup(void) {
  Serial.begin(115200); 

  //u8g2.setBusClock(400000);
  Serial.println(u8g2.getDisplayHeight());
  u8g2.begin();
  Serial.println(u8g2.getDisplayHeight());
  u8g2_prepare();
}
void loop(void) {
 u8g2.clearBuffer();
 u8g2_prepare();
 u8g2_box_frame();
 u8g2.sendBuffer();
 delay(1500);
 u8g2.clearBuffer();
 u8g2_disc_circle();
 u8g2.sendBuffer();
 delay(1500);
 u8g2.clearBuffer();
 u8g2_r_frame_box();
 u8g2.sendBuffer();
 delay(1500);
 u8g2.clearBuffer();
 u8g2_prepare();
 u8g2_string_orientation();
 u8g2.sendBuffer();
 delay(1500);
 u8g2.clearBuffer();
 u8g2_line();
 u8g2.sendBuffer();
 delay(1500);
 u8g2.clearBuffer();
 u8g2_triangle();
 u8g2.sendBuffer();
 delay(1500);
 u8g2.clearBuffer();
 u8g2_prepare();
 u8g2_unicode();
 u8g2.sendBuffer();
 delay(5000);
}
