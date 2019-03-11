# Elkrem Decentralized SDK 

A simple SDK library for decentralilzed blockchain networks. 

## Usage 

This usage's setps is just a workaround in order to test the SDK with ESP32 board from platformio, In the future the whole project will be integrated already inside Platformio cores at delivery.

1- Create a platformio project choosing **ESP32 thing** in boards and **Arduino** framework.

2- Copy paste sdk/src files to **src** directory in project.

3- Copy paste vendor folders to **lib** directory 

4-  Copy paste example shown to src\main.cpp.

5- Compile and Upload.

## Example
```c++
#include "Elkrem.h"

void setup()
{
  // put your setup code here, to run once:
  Elkrem.init();
  Ethereum.requestAccountBalance("0x5ed8cee6b63b1c6afce3ad7c92f4fd7e1b8fad9f");
  Ethereum.requestCurrentBlock();
}

void loop()
{
  // put your main code here, to run repeatedly:
}
```