This project has a demonstration purpose to show the advantage of using Ad Hoc protocol code generator.
The idea was to replace a manually written protocol of the [PowerProfiler project](https://github.com/pvvx/STM32INA2xxPowerProfiler) with AdHoc code.
later become clear to me that the project should be completely rethought and rewritten.

The main routings of the original project are located in [the main.c file](https://github.com/pvvx/STM32INA2xxPowerProfiler/blob/master/STM32F103/Main/main.c). I did not touch others files, just edit main.c content. And add 'adhoc' folder with AdHoc protocol generated source code. 
