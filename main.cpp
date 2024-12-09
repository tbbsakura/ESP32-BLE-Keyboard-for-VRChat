// BleKeyboard.h, BleKeyboard.cpp を同じファイルに置いてビルドしてください
//  https://github.com/T-vK/ESP32-BLE-Keyboard/blob/master/BleKeyboard.h
//  https://github.com/T-vK/ESP32-BLE-Keyboard/blob/master/BleKeyboard.cpp

const int pins[] = { 1,4,3 }; // 入力用pin 3つのGPUO
// Questでいう ボタンセンサー（親指）、トリガー（人差し指）、グリップ（中指）の３ヶ所に対応
// PULLUPしておいて、スイッチおしたらGNDに落ちてLOWになるようにする
// HW PULLUPしてない場合は setup 内の pinMode でINPUT_PULLUPにする


#include <Arduino.h>
#include <BleKeyboard.h>

BleKeyboard bleKeyboard;


const char* pinsSeparator[] = { ",", ",", " = " }; // シリアル出力時の区切り文字 A,B,C = の体
const char* strSigns[] = { " ", "idle", "fist", "open", "point", "peace", "rnr", "gun", "thumbs up" }; // FN# to Hand-sign name
//const int fnkey2ges[] = { -1, 2, 7, 0, 5, 4, 6, 1, 3 }; // FN2 => gesture2, etc.
const int ges2fnkey[] = { 3, 7, 1, 8, 5, 4, 6, 2 }; // gesture0 => FN3, etc.
const int vkeyFn[] = { 0, KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6, KEY_F7, KEY_F8 };

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");
  bleKeyboard.begin();

  for ( int i = 0; i < sizeof(pins)/sizeof(int); i++ ) {
    pinMode(pins[i],INPUT); // HW で PULLUP してないときは INPUT_PULLUP に
  }
}

void SendFn(int n, bool left = false, uint8_t dsec = 3) // n=1 ならFN1を,  dsec秒後に、left or right SHIFT押下状態として送信
{
  if ( n < 1 || n > 8 ) { // 送信するのは FN1-FN8の間のみ
    Serial.printf("invalid number: %d", n);
    return;
  }
  Serial.printf("Sending %s SHIFT + F%d in %d seconds\n", left ? "LEFT": "RIGHT", n, dsec);
  if (dsec > 0) delay(dsec * 1000);
  bleKeyboard.press(left ? KEY_LEFT_SHIFT : KEY_RIGHT_SHIFT);
  bleKeyboard.press(vkeyFn[n]);
  delay(100);
  bleKeyboard.releaseAll();
}

// たとえば、Serial で FL1 と送れば、LEFT SHIFT+ F1 を3秒後に送出する(その3秒の間に、キー入力したいアプリをactiveにする)
void loopSerialCommand() 
{
  if ( Serial.available()>0 ) {
    String cmd = Serial.readStringUntil('\n');
    if (bleKeyboard.isConnected()==false) { // BLE接続していない場合は送出しない
      Serial.printf("Not connected\n");
      return;
    }

    char lr = cmd.charAt(0);
    char f  = cmd.charAt(1);
    char n  = cmd.charAt(2);
    if ( f == 'F' && (lr =='L' || lr =='R') ) { // FL/FR両方大文字のとき
      f += 0x20; // 小文字(ASCIIコード 大文字+0x20)にする
      lr += 0x20;
    }
    if ( f == 'f' && (lr =='l' || lr =='r') && (n >= '1' && n <= '8')) {
       n -= '0'; // '0'-'9' を  0-9のintに変換
       SendFn( (int)n, lr == 'l', 3);
    }
    else {
      Serial.println("command not recognized");
    }
  }
}

// loopごとにピンの状態をチェックして、0-7の値を得て、右シフト＋FN1-8を送出する
void checkPin() {
  static int lastSent = 9;
  String out;
  for ( int i = 0; i < sizeof(pins)/sizeof(int); i++ ) {
    out += String(pins[i]);
    out += pinsSeparator[i];
  }
  int bits = 0;
  for ( int i = 0; i < sizeof(pins)/sizeof(int); i++ ) {
    int n = digitalRead(pins[i]) == HIGH ? 0:1;
    bits += n << i;
    out += n == HIGH ? "HIGH," : "LOW ,";
  }
  int key = ges2fnkey[bits];
  if ( key >= 0 )  {
    if ( key != lastSent ){
      Serial.print(out);
      Serial.printf("RIGHT SHIFT + F%d (%s) to be sent", key, strSigns[key]);
      SendFn( key, false, 0 );
      lastSent = key;
    }
  }
}

void loop() {
  loopSerialCommand();
  checkPin();
  delay(100);
}
