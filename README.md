
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
platformioで、lolin_c3_mini でプロジェクトを作り、ビルドしてください
必要なライブラリは platformio.ini に記述してあるので自動的にダウンロードされるはずです。

## NimBLEライブラリについて
NimBLE(ESP32C3やS3で使われているBLEライブラリ)の Arduino Coreライブラリは 1.4.x と 2.x.x で大きく内容が変わっており、過去に作ったコードが2.x.xで動かないというのが頻発しています。
こちらはplatformio.ini内で1.4.3固定で指定しています。

(蛇足ですが、初代ESP32で使えていた bluedroid は最近のESP32*では使えなくなっています)

## ESP32-BLE-* ライブラリについて
t-vk/ESP32 BLE Keyboard はPlatformIOにも登録されているライブラリですが、同じところで公開されているマウスのほうはNimBLEが必要なS3,C3だとうまく動きませんでした。また、キーボードとマウスは別々になっています。

マウスをS3,C3で使いたい場合は、わたしは NimBLE対応版のCombo(キーボードマウス同時可)のForkである
    ![https://github.com/A-box1000/ESP32-NimBLE-Combo.git](https://github.com/A-box1000/ESP32-NimBLE-Combo.git)
を使っています。PlatformIOのライブラリ登録はされていませんが、lib_depsにURL書いてそのまま使えます。
(Combo Fork, NimBLE対応ComboFork, それのさらなる Fork とあるので気になる方は違う版を試してもいいかもしれません)
