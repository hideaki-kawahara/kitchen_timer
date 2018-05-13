#include <M5Stack.h>
boolean but_A = false, but_LEFT = false, but_RIGHT = false;
uint32_t targetTime = 0;
byte omm = 99, oss = 99;
uint8_t mm = 3;
int8_t ss= 0;
bool hasStop = true;
bool hasEnd = false;
void setup() {
  M5.begin();
  M5.Lcd.fillScreen(TFT_LIGHTGREY);
  targetTime = millis() + 1000;

  M5.Lcd.setTextFont(2);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(TFT_BLACK, TFT_LIGHTGREY);
  M5.Lcd.setCursor( 60, 200);
  M5.Lcd.print("M");
  M5.Lcd.setCursor( 150, 200);
  M5.Lcd.print("S");
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor( 240, 200);
  M5.Lcd.print("START");
  M5.Lcd.setCursor( 240, 220);
  M5.Lcd.print("/STOP");

  Serial.println("START");
}

void loop() {
  if(M5.BtnC.wasPressed() && !hasStop) {
      hasStop = true;
  } else if (M5.BtnC.wasPressed() && hasStop) {
      hasStop = false;
  }
  if(M5.BtnB.wasPressed() && hasStop) {
      ss++;
      hasEnd = false;
      if (ss == 60){
        ss = 0;
      }
      draw_seconds(ss);
  }
  if(M5.BtnA.wasPressed() && hasStop) {
      mm++;
      hasEnd = false;
      if (mm == 60){
        mm = 0;
      }
      draw_minutes(mm);
  }  
  M5.update();
    if (targetTime < millis()) {
      targetTime = millis() + 1000;

      // 時間停止してない
      if (!hasStop){
        ss--;
        if (ss == -1) {
          // 分秒ともに0なら表示停止＆timerストップ
          if (mm == 0) {
            ss =0;
            hasStop = true;
            hasEnd = true;
            M5.Speaker.beep();
          } else {
            ss = 59;
            omm = mm;
            mm--;
          }
        }
      }

      if (hasEnd) {
        // timerストップの表示
        M5.Lcd.setCursor( 100, 150);
        M5.Lcd.setTextFont(2);
        M5.Lcd.print("Timer Stop");
      } else {
        // 表示を消す
        M5.Lcd.setCursor( 100, 150);
        M5.Lcd.setTextFont(2);
        M5.Lcd.print("          ");
      }

      // 7seg font
      M5.Lcd.setTextFont(7);
      M5.Lcd.setTextSize(2);
      M5.Lcd.setTextColor(TFT_BLACK, TFT_LIGHTGREY);

      // 分の更新
      if (omm != mm) {
        omm = mm;
        draw_minutes(mm);
      }

      // 秒の更新
      if (oss != ss) {
        oss = ss;

        // インジケーターの点滅
        if (ss % 2 && !hasStop) {
          M5.Lcd.setCursor( 150, 40);
          M5.Lcd.setTextColor(TFT_LIGHTGREY, TFT_LIGHTGREY);
          M5.Lcd.print(":");
          M5.Lcd.setTextColor(TFT_BLACK, TFT_LIGHTGREY);
        }
        else {
          M5.Lcd.setCursor( 150, 40);
          M5.Lcd.print(":");
        }
        
        draw_seconds(ss);  
      }
    }
}

// 分の表示
void draw_minutes(int mm) {
  if (mm < 10) {
    M5.Lcd.setCursor( 0, 40);
    M5.Lcd.print("0");
    M5.Lcd.print(mm);        
  } else {
    M5.Lcd.setCursor( 0, 40);
    M5.Lcd.print(mm);        
  }
}

// 秒の表示
void draw_seconds(int ss) {
  if (ss < 10) {
    M5.Lcd.setCursor( 180, 40);
    M5.Lcd.print("0");
    M5.Lcd.print(ss);
  } else {
    M5.Lcd.setCursor( 180, 40);
    M5.Lcd.print(ss);          
  }
}

