#include "Keyboard.h"

const int buttonPin1 = 2; // 첫 번째 버튼 핀 번호
const int buttonPin2 = 3; // 두 번째 버튼 핀 번호
const unsigned long debounceDelay = 50; // 디바운싱 딜레이 (밀리초)
const unsigned long shortPressDuration = 100; // 짧은 누름으로 간주할 시간 (밀리초)

int lastButtonState1 = HIGH;  // 첫 번째 버튼의 마지막 상태
int buttonState1 = HIGH;      // 첫 번째 버튼의 현재 상태
unsigned long lastDebounceTime1 = 0; // 첫 번째 버튼의 마지막 디바운싱 시간
unsigned long pressStartTime1 = 0; // 첫 번째 버튼의 누름 시작 시간

int lastButtonState2 = HIGH;  // 두 번째 버튼의 마지막 상태
int buttonState2 = HIGH;      // 두 번째 버튼의 현재 상태
unsigned long lastDebounceTime2 = 0; // 두 번째 버튼의 마지막 디바운싱 시간
unsigned long pressStartTime2 = 0; // 두 번째 버튼의 누름 시작 시간

bool enterPrinted = false; // "enter" 메시지가 출력되었는지 추적하는 변수

bool shortFlag1 = false;
bool shortFlag2 = false;

void setup() {
  // 버튼 핀을 입력으로 설정
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
  // 시리얼 통신 시작
  Serial.begin(9600);
}

void loop() {
  // 현재 버튼 상태를 읽기
  int reading1 = digitalRead(buttonPin1);
  int reading2 = digitalRead(buttonPin2);

  // 첫 번째 버튼 디바운싱 처리
  if (reading1 != lastButtonState1) {
    lastDebounceTime1 = millis();

    // 버튼이 눌렸을 때 즉시 반응하도록 처리
    if (reading1 == LOW && lastButtonState1 == HIGH) {
      Serial.println("left");
      Keyboard.write(KEY_LEFT_ARROW);
      Keyboard.releaseAll();
      shortFlag1 = true;
    }
  }

  if ((millis() - lastDebounceTime1) > debounceDelay) {
    // 디바운싱 후 버튼 상태가 바뀌었는지 확인
    if (reading1 != buttonState1) {
      buttonState1 = reading1;
      // 버튼 상태가 LOW(눌림)이 되었을 때 누름 시작 시간 기록
      if(shortFlag1){
        // Serial.println("shortButton 1 pressed");
      }else{
        if (buttonState1 == LOW) {
          pressStartTime1 = millis();
          Serial.println("longButton 1 pressed");
          Keyboard.write(KEY_LEFT_ARROW);
          Keyboard.releaseAll();
          shortFlag1 = false;
        } else {
          // 버튼이 떼졌을 때 짧은 누름으로 판단하고 메시지 출력
          if (millis() - pressStartTime1 < shortPressDuration && !(buttonState1 == LOW && buttonState2 == LOW)) {
            // Serial.println("Button 1 pressed");
          }
        }
      }
    }
  }
  // 마지막 버튼 상태 업데이트
  lastButtonState1 = reading1;

  // 두 번째 버튼 디바운싱 처리
  if (reading2 != lastButtonState2) {
    lastDebounceTime2 = millis();

    // 버튼이 눌렸을 때 즉시 반응하도록 처리
    if (reading2 == LOW && lastButtonState2 == HIGH) {
      Serial.println("right");
      Keyboard.write(KEY_RIGHT_ARROW);
      Keyboard.releaseAll();
      shortFlag2 = true;
    }
  }

  if ((millis() - lastDebounceTime2) > debounceDelay) {
    // 디바운싱 후 버튼 상태가 바뀌었는지 확인
    if (reading2 != buttonState2) {
      buttonState2 = reading2;
      
      if(shortFlag2){
        // Serial.println("shortButton 2 pressed");
      }else{
          // 버튼 상태가 LOW(눌림)이 되었을 때 누름 시작 시간 기록
          if (buttonState2 == LOW) {
            pressStartTime2 = millis();
            Serial.println("Button 2 pressed");
            Keyboard.write(KEY_RIGHT_ARROW);
            Keyboard.releaseAll();
            shortFlag2 = false;
          } else {
            // 버튼이 떼졌을 때 짧은 누름으로 판단하고 메시지 출력
            if (millis() - pressStartTime2 < shortPressDuration && !(buttonState1 == LOW && buttonState2 == LOW)) {
              // Serial.println("Button 2 pressed");
          }
        }
      }
    }
  }

  // 마지막 버튼 상태 업데이트
  lastButtonState2 = reading2;

  // 두 버튼이 모두 눌렸는지 확인
  if (buttonState1 == LOW && buttonState2 == LOW) {
    if (!enterPrinted) {
      Serial.println("enter");
      Keyboard.write(KEY_RETURN);
      Keyboard.releaseAll();
      enterPrinted = true; // "enter" 메시지가 출력되었음을 표시
    }
  } else {
    enterPrinted = false; // 두 버튼이 모두 눌리지 않은 경우 상태를 초기화
  }

  // 디버깅 출력을 너무 자주 하지 않도록 약간의 지연 추가
  delay(50);
}


