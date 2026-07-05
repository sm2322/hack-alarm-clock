//#include <Adafruit_GFX.h>
//#include <Adafruit_ST7789.h>
//#include <SPI.h>

// screen setup
#define screen_serial_clock 10
#define screen_serial_data 8
#define screen_reset 7
#define screen_data_command 6
#define screen_chip 5
#define screen_backlight 4
// switches
#define switch_one_plus 0
#define switch_two_minus 1
#define switch_three_confirm 2
// buzzer
#define buzzer 3
// screen
//Adafruit_ST7789 screen = Adafruit_ST7789(screen_chip, screen_data_command, screen_serial_data, screen_serial_clock, screen_reset);
// Clock Variables
int current_hour = 12;
int current_minuite = 0;
int current_second = 0;
// Alarm Variables
int alarm_hour = 4;
int alarm_minuite = 12;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  //screen.init(284, 76);
  //screen.setColRowStart(82, 18);
  //screen.setRotation(1);
  pinMode(switch_one_plus, INPUT_PULLUP); 
  pinMode(switch_two_minus, INPUT_PULLUP); 
  pinMode(switch_three_confirm, INPUT_PULLUP); 
}
//Clock State
int clock_state = 0;
// 0 = display time
// 1 = set time
// 2 = set alarm
// 3 = alarm going off

// timing variables
unsigned long previousMillis = 0;
const long interval = 1000;
bool AdjustingHours = true;
void loop() {
  //screen.fillScreen(ST77XX_BLACK);
  int MinusState = digitalRead(switch_two_minus);
  int PlusState = digitalRead(switch_one_plus);
  int ConfirmState = digitalRead(switch_three_confirm);
  updateTime();
  // Show Time
  if (clock_state == 0) {
      displayCurrentTime();
      if (ConfirmState == LOW) {
        Serial.println("Set Time");
        delay(1000);
        clock_state = 1;
        delay(200); // Simple debounce delay to prevent duplicate readings
  }
  }
  // Set Time
  else if (clock_state == 1) {
    displayCurrentTime();
    if (AdjustingHours){
      // changing hours
      // hours down
      if (MinusState == LOW){
        if (current_hour <= 1){
          current_hour = 12;
        }
        else {
          current_hour -= 1;
        }
        delay(200);
    }
    // hours up
    if (PlusState == LOW){
        if (current_hour >= 12){
          current_hour = 1;
        }
        else {
          current_hour += 1;
        }
        delay(200);
    }
    // switch from hour to min
    if (ConfirmState == LOW){
        AdjustingHours = false;
        delay(200);
    } 
    }
    else {
      // min go down
      if (MinusState == LOW){
        if (current_minuite <= 1){
          current_minuite = 59;
        }
        else {
          current_minuite -= 1;
        }
        delay(200);
    }
    // min go up
    if (PlusState == LOW){
        if (current_minuite >= 59){
          current_minuite = 1;
        }
        else {
          current_minuite += 1;
        }
        delay(200);
    }
    // confirm set time, switch to alarm set
    if (ConfirmState == LOW){
         Serial.println("Time Set");
         delay(1000);
         Serial.println("Set Alarm");
         delay(1000);
         AdjustingHours = true;
         clock_state = 2;
    }
  }
  }
  // Set Alarm
  else if (clock_state == 2) {
    // show alarm time
    displayAlarmTime();
    // adjust alarm hour
    if (AdjustingHours){
      // alarm hour down
      if (MinusState == LOW){
        if (alarm_hour <= 1){
          alarm_hour = 12;
        }
        else {
          alarm_hour -= 1;
        }
        delay(200);
    }
    // alarm hour up
    if (PlusState == LOW){
        if (alarm_hour >= 12){
          alarm_hour = 1;
        }
        else {
          alarm_hour += 1;
        }
        delay(200);
    }
    // switch to alarm mins
    if (ConfirmState == LOW){
        AdjustingHours = false;
        delay(200);
    } 
    }
    else {
      // alarm min down
      if (MinusState == LOW){
        if (alarm_minuite <= 1){
          alarm_minuite = 59;
        }
        else {
          alarm_minuite -= 1;
        }
        delay(200);
    }
    // alarm min up
    if (PlusState == LOW){
        if (alarm_minuite >= 59){
          alarm_minuite = 1;
        }
        else {
          alarm_minuite += 1;
        }
        delay(200);
    }
    // finish alarm
    if (ConfirmState == LOW){
         Serial.println("Alarm Set");
         delay(1000);
         AdjustingHours = true;
         clock_state = 0;
    }
  }
}
}
void updateTime() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval){
    previousMillis = currentMillis;
    current_second += 1;
  };
  if (current_second >= 60) {
    current_second = 0;
    current_minuite += 1;
  }
  if (current_minuite >= 60){
    current_minuite = 0;
    current_hour += 1;
  }
  if (current_hour > 12){
    current_hour = 1;
  }
}
void displayCurrentTime() {
  if (current_hour < 10) Serial.print("0");
  Serial.print(current_hour);
  Serial.print(":");
  if (current_minuite < 10) Serial.print("0");
  Serial.print(current_minuite);
  Serial.println();
}
void displayAlarmTime() {
  if (alarm_hour < 10) Serial.print("0");
  Serial.print(alarm_hour);
  Serial.print(":");
  if (alarm_minuite < 10) Serial.print("0");
  Serial.print(alarm_minuite);
  Serial.println();
}


