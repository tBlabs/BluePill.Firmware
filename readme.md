# BluePill.Firmware 

Firmware for BluePill development board.

**6x** Input  
**6x** Output **+1** Build in LED   
**4x** ADC  
**4x** PWM  
**1x** RTC  
**1x** 4-digit 7-seg Display  
**1x** Buzzer

## Any driver?

Search for `BluePill.Driver` in my repo.

## Board pinout

```
                             | | | |
                    N/C --o  o o o o  o-- 3V3
 Output1 (build in led) --o           o-- GND
                    N/C --o           o-- 5V
		    N/C --o           o-- Input3 
		   ADC1 --o           o-- Input4
		   ADC2 --o           o-- Display Data 
		   ADC3 --o           o-- Display Clock
		   ADC4 --o           o-- Display Latch
		 Buzzer --o           o-- N/C
	        Output2 --o           o-- N/C  
		   PWM1 --o           o-- N/C
		   PWM2 --o           o-- Output3
		   PWM3 --o           o-- Output4  
		   PWM4 --o           o-- Rx   
		 Input1 --o           o-- Tx   
		 Input2 --o           o-- Output5  
		  RESET --o           o-- Output6
		    3V3 --o           o-- Output7  
		    GND --o           o-- Input5  
		    GND --o    USB    o-- Input6 
```

## Serial config

19200:8:1:n  
Tx pin: A9  
Rx pin: A10

## Request

> 0xBB 0xAA {FrameType:8} {DataSize:8} {Data:0+} {Xor:8}

| Type			| Value | Data size | Params			   | Possible response |
| --------------------- | ----- | --------- | ---------------------------- | ----------------- |
| Ping			| 0x01	| 0 bytes   | *none*			   | Pong	       |
| Get			| 0x02	| 1	    | addr:8			   | Update / Error    |
| Get all sensors	| 0x03	| 0	    | *none*			   | UpdateAll / Error |
| Set			| 0x04	| 5	    | addr:8, value:32		   | Update / Error    |
| Push state set        | 0x05	| 2	    | pushEnable:8, scanInterval:8 | PushStateUpdate   |

## Response

0xAB {FrameType:8} {Data:0+} {Xor:8}

| Type               | Value | Data size  | Data values      |
| ------------------ | ----- | ---------- | ---------------- |
| Pong		     | 0x01  | 0 bytes    | *none*           |
| Error		     | 0x02  | 1	  | errorCode:8      |
| Update (single IO) | 0x03  | 5	  | addr:8, value:32 |
| All sensors update | 0x04  | 44	  | input1:32, input2:32, ..., rtc:32 |
| Push state update  | 0x05  | 1	  | state:8	     |

# IO addresses (`addr` param to pin mapping)

| Addr  | Type             | Pin  |
| ----- | ---------------- | ---- |
| 0x00  | Digital input    | B10  |
| 0x01  | Digital input    | B11  |
| 0x02  | Digital input    | B9   |
| 0x03  | Digital input    | B8   |
| 0x04  | Digital input    | B13  |
| 0x05  | Digital input    | B12  |
| 0x06  | ADC		   | A0   |
| 0x07  | ADC		   | A1   |
| 0x08  | ADC		   | A2   |
| 0x09  | ADC		   | A3   |
| 0x0A  | Real time clock  | None |
| 0x0B  | Digital output   | C13 (build in led) |
| 0x0C  | Digital output   | A5   |
| 0x0D  | Digital output   | A12  |
| 0x0E  | Digital output   | A11  |
| 0x0F  | Digital output   | A8   |
| 0x10  | Digital output   | B15  |
| 0x11  | Digital output   | B14  |
| 0x12  | PWM		   | A6   |
| 0x13  | PWM		   | A7   |
| 0x14  | PWM		   | B0   |
| 0x15  | PWM		   | B1   |
| 0x16	| Display value	   | B5 (data), B6 (clock), B7 (latch) |
| 0x17	| Display dot	   | N/D  |
| 0x18	| Buzzer enable	   | A4   |
| 0x19	| Buzzer frequency | *^*  |
| 0x20	| Buzzer volume	   | *^*  |

# Programming with J-Link
 
## J-link SWD pinout
  
```
Vref --o   o-- NC
N/D  --o   o-- GND
N/D  --o   o-- GND
DIO  --o   o-- GND
CLK  --o|  o-- GND
N/D  --o|  o-- GND
SWO  --o   o-- GND
RST  --o   o-- GND
N/D  --o   o-- GND
5V   --o   o-- GND
```
N/D - not used  
SWO - not used

## STM32F103C8 SWD pins
```
           G C D 3
	   N L I V
	   D K O 3
           | | | |
Vbat --o   | | | |  o-- 3,3V
C13  --o   o o o o  o-- GND
C14  --o            o-- 5V
       |    Blue    |
      ...   Pill   ...
       | (top view) |
RST  --o            o-- B15
       |            |
```
## Connection

| J-link | Blue Pill |
| ------ | --------- |
| GND	 | GND       |
| CLK    | CLK       |
| DIO	 | DIO       |
| Vref	 | 3V3       |
| RST	 | RST       |
       
## Blue Pill powering

STM must be powered from external 5V source.  
J-Link 5V out is not working.

# Chip burn

Use Visual Studio + VisualGDB

## CLI?

No idea

# Testing from serial terminal (Bray++)

## Ping-Pong

Set terminal Receiver into HEX mode.    
Send `$BB$AA$01$00$10`.
You should receive `AB 01 AA` response.

## Build in led (pin C13) on/off

Send `$BB$AA$04$05$0B$00$00$00$00$1B` to turn LED on.  
`$BB$AA$04$05$0B$01$00$00$00$1A` to off.
