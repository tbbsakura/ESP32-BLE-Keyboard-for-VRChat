
## M5stamp C3U用
3つのボタンをつけて、それがQuestコンでいう親指人差し指中指の状態に対応して8種類の状態になり、それに対応した右SHIFT+FN1-8を入力できるBLEキーボードとなります。<br>
PCからはESP32 Keyboard というBluetooth (BLE)デバイスとして認識、ペアリング、接続できます。

## 配線等
GPIO 1,4,3 (digitalReadできるところなら他でも。その場合main.cppのpins定義を変更)を<br>
プルアップして(10-100kくらいで)通常HIGH、<br>
スイッチをつけて、おすとGNDに繋がってLOWになるようにします。<br>
(プルアップ抵抗省略して、setup 内の pinMode でINPUT_PULLUPにするのでも良いと思います。)<br>

 ![回路図](https://github.com/tbbsakura/ESP32-BLE-Keyboard-for-VRChat/blob/main/schematic.png)

## ビルド
platformioで、lolin_c3_mini でプロジェクトを作り、
main.cpp を  BleKeyboard.h, BleKeyboard.cpp を main.cpp と同じ場所(src)に置いてビルドしてください
  - https://github.com/T-vK/ESP32-BLE-Keyboard/blob/master/BleKeyboard.h
  - https://github.com/T-vK/ESP32-BLE-Keyboard/blob/master/BleKeyboard.cpp
