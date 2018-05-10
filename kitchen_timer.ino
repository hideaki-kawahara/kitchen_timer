#include <M5Stack.h>

#define TFT_GREY 0x5AEB
boolean but_A = false, but_LEFT = false, but_RIGHT = false;

uint32_t targetTime = 0;
byte omm = 99, oss = 99;
byte xcolon = 0, xsecs = 0;
unsigned int colour = 0;
uint8_t mm=1;
int8_t ss=10;
bool hasStop = true;
bool hasEnd = false;
void setup() {
  //Serial.begin(115200);
  M5.begin();
  // M5.Lcd.setRotation(1);
  M5.Lcd.fillScreen(TFT_BLACK);

  M5.Lcd.setTextSize(1);
  M5.Lcd.setTextColor(TFT_YELLOW, TFT_BLACK);

  targetTime = millis() + 1000;
  Serial.println("START");
}

void loop() {
  if(M5.BtnC.wasPressed() && !hasStop) {
      hasStop = true;
      Serial.println("test1");
  } else if (M5.BtnC.wasPressed() && hasStop) {
      hasStop = false;
      Serial.println("test2");
  }
  if(M5.BtnB.wasPressed() && hasStop) {
      ss++;
      hasEnd = false;
      if (ss == 60){
        ss = 0;
      }
  }
  if(M5.BtnA.wasPressed() && hasStop) {
      mm++;
      hasEnd = false;
      if (mm == 60){
        mm = 0;
      }
  }
  
  M5.update();
    if (targetTime < millis()) {
      // Set next update for 1 second later
      targetTime = millis() + 1000;

      if (!hasStop){
        // Adjust the time values by adding 1 second
        ss--;              // Advance second
        if (ss == -1) {    // Check for roll-over
          if (mm == 0) {   // Check for roll-over
            ss =0;
            hasStop = true;
            hasEnd = true;
          } else {
            ss = 59;          // Reset seconds to zero
            omm = mm;        // Save last minute time for display update
            mm--;            // Advance minute
          }
        }
      }
      if (hasEnd) {
        M5.Lcd.setCursor( 100, 180);
        M5.Lcd.println("Timer Stop");
      } else {
        M5.Lcd.setCursor( 100, 180);
        M5.Lcd.println("            ");
      }

      // Update digital time
      int xpos = 0;
      int ypos = 85; // Top left corner ot clock text, about half way down
      int ysecs = ypos + 24;
  
      if (omm != mm) { // Redraw hours and minutes time every minute
        omm = mm;
        // Draw hours and minutes
        xcolon = xpos; // Save colon coord for later to flash on/off later
        if (mm < 10) xpos += M5.Lcd.drawChar('0', xpos, ypos, 8); // Add minutes leading zero
        xpos += M5.Lcd.drawNumber(mm, xpos, ypos, 8);             // Draw minutes
        xsecs = xpos; // Sae seconds 'x' position for later display updates
      }
      if (oss != ss) { // Redraw seconds time every second
        oss = ss;
        xpos = xsecs;
  
        if (ss % 2) { // Flash the colons on/off
          M5.Lcd.setTextColor(0x39C4, TFT_BLACK);        // Set colour to grey to dim colon
          xpos += M5.Lcd.drawChar(':', xsecs, ysecs, 6); // Seconds colon
          M5.Lcd.setTextColor(TFT_YELLOW, TFT_BLACK);    // Set colour back to yellow
        }
        else {
          xpos += M5.Lcd.drawChar(':', xsecs, ysecs, 6); // Seconds colon
        }
  
        //Draw seconds
        if (ss < 10) xpos += M5.Lcd.drawChar('0', xpos, ysecs, 6); // Add leading zero
        M5.Lcd.drawNumber(ss, xpos, ysecs, 6);                     // Draw seconds
      }
    }
}

