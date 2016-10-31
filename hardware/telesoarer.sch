EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:gruvin
LIBS:telesoarer-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "Telesoarer"
Date "2016-10-31"
Rev "1.0"
Comp ""
Comment1 ""
Comment2 ""
Comment3 "for RC Slope Soaring / Sailplanes"
Comment4 "Altitude Telemetry and Lost Model Alarm"
$EndDescr
Text Notes 1650 6250 0    60   ~ 0
3x2 pogo\npin pads
$Comp
L R R6
U 1 1 51A7498E
P 6600 5850
F 0 "R6" V 6680 5850 50  0000 C CNN
F 1 "1K" V 6600 5850 50  0000 C CNN
F 2 "footprints:R_0603" H 6600 5850 60  0001 C CNN
F 3 "" H 6600 5850 60  0001 C CNN
	1    6600 5850
	0    1    1    0   
$EndComp
$Comp
L +5V #PWR01
U 1 1 51A74974
P 7200 5050
F 0 "#PWR01" H 7200 5140 20  0001 C CNN
F 1 "+5V" H 7250 5200 30  0000 C CNN
F 2 "" H 7200 5050 60  0001 C CNN
F 3 "" H 7200 5050 60  0001 C CNN
	1    7200 5050
	1    0    0    -1  
$EndComp
$Comp
L GND-RESCUE-telesoarer-mlf #PWR02
U 1 1 51A74963
P 7200 6200
F 0 "#PWR02" H 7200 6200 30  0001 C CNN
F 1 "GND" H 7200 6130 30  0001 C CNN
F 2 "" H 7200 6200 60  0001 C CNN
F 3 "" H 7200 6200 60  0001 C CNN
	1    7200 6200
	1    0    0    -1  
$EndComp
$Comp
L BEEPER PZ1
U 1 1 51A7493C
P 9500 5450
F 0 "PZ1" H 9600 5150 60  0000 C CNN
F 1 "PZ_OUT" H 9600 5750 60  0000 C CNN
F 2 "footprints:PIN_ARRAY_2X1" H 9500 5450 60  0001 C CNN
F 3 "" H 9500 5450 60  0001 C CNN
	1    9500 5450
	1    0    0    -1  
$EndComp
$Comp
L GND-RESCUE-telesoarer-mlf #PWR03
U 1 1 51A744A2
P 5500 2000
F 0 "#PWR03" H 5500 2000 30  0001 C CNN
F 1 "GND" H 5500 1930 30  0001 C CNN
F 2 "" H 5500 2000 60  0001 C CNN
F 3 "" H 5500 2000 60  0001 C CNN
	1    5500 2000
	1    0    0    -1  
$EndComp
Text Label 4200 1700 2    60   ~ 0
LED_GREEN
Text Label 4200 1400 2    60   ~ 0
LED_BLUE
Text Label 4200 1100 2    60   ~ 0
LED_RED
$Comp
L R R5
U 1 1 51A743F1
P 4500 1700
F 0 "R5" V 4580 1700 50  0000 C CNN
F 1 "1K5" V 4500 1700 50  0000 C CNN
F 2 "footprints:R_0603" H 4500 1700 60  0001 C CNN
F 3 "" H 4500 1700 60  0001 C CNN
	1    4500 1700
	0    1    1    0   
$EndComp
$Comp
L R R4
U 1 1 51A743EE
P 4500 1400
F 0 "R4" V 4580 1400 50  0000 C CNN
F 1 "3K9" V 4500 1400 50  0000 C CNN
F 2 "footprints:R_0603" H 4500 1400 60  0001 C CNN
F 3 "" H 4500 1400 60  0001 C CNN
	1    4500 1400
	0    1    1    0   
$EndComp
$Comp
L R R3
U 1 1 51A743EB
P 4500 1100
F 0 "R3" V 4580 1100 50  0000 C CNN
F 1 "1K" V 4500 1100 50  0000 C CNN
F 2 "footprints:R_0603" H 4500 1100 60  0001 C CNN
F 3 "" H 4500 1100 60  0001 C CNN
	1    4500 1100
	0    1    1    0   
$EndComp
$Comp
L LED D3
U 1 1 51A743A6
P 5100 1700
F 0 "D3" H 5100 1800 50  0000 C CNN
F 1 "GREEN" H 5100 1600 50  0000 C CNN
F 2 "footprints:LED_0603" H 5100 1700 60  0001 C CNN
F 3 "" H 5100 1700 60  0001 C CNN
	1    5100 1700
	-1   0    0    -1  
$EndComp
$Comp
L LED D2
U 1 1 51A743A4
P 5100 1400
F 0 "D2" H 5100 1500 50  0000 C CNN
F 1 "BLUE" H 5100 1300 50  0000 C CNN
F 2 "footprints:LED_0603" H 5100 1400 60  0001 C CNN
F 3 "" H 5100 1400 60  0001 C CNN
	1    5100 1400
	-1   0    0    -1  
$EndComp
$Comp
L LED D1
U 1 1 51A743A2
P 5100 1100
F 0 "D1" H 5100 1200 50  0000 C CNN
F 1 "RED" H 5100 1000 50  0000 C CNN
F 2 "footprints:LED_0603" H 5100 1100 60  0001 C CNN
F 3 "" H 5100 1100 60  0001 C CNN
	1    5100 1100
	-1   0    0    -1  
$EndComp
NoConn ~ 3800 3150
NoConn ~ 3800 3050
NoConn ~ 3800 2950
Text Label 5750 3350 0    60   ~ 0
LED_GREEN
Text Label 5750 3250 0    60   ~ 0
LED_BLUE
Text Label 5750 3150 0    60   ~ 0
LED_RED
$Comp
L +5V #PWR04
U 1 1 504AC376
P 2250 5250
F 0 "#PWR04" H 2250 5340 20  0001 C CNN
F 1 "+5V" H 2300 5400 30  0000 C CNN
F 2 "" H 2250 5250 60  0001 C CNN
F 3 "" H 2250 5250 60  0001 C CNN
	1    2250 5250
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR05
U 1 1 4ED08824
P 4050 5400
F 0 "#PWR05" H 4050 5490 20  0001 C CNN
F 1 "+5V" H 4100 5550 30  0000 C CNN
F 2 "" H 4050 5400 60  0001 C CNN
F 3 "" H 4050 5400 60  0001 C CNN
	1    4050 5400
	1    0    0    -1  
$EndComp
$Comp
L GND-RESCUE-telesoarer-mlf #PWR06
U 1 1 4ED08810
P 4050 6350
F 0 "#PWR06" H 4050 6350 30  0001 C CNN
F 1 "GND" H 4050 6280 30  0001 C CNN
F 2 "" H 4050 6350 60  0001 C CNN
F 3 "" H 4050 6350 60  0001 C CNN
	1    4050 6350
	1    0    0    -1  
$EndComp
$Comp
L CP C2
U 1 1 4ED087CB
P 4050 5850
F 0 "C2" H 4100 5950 50  0000 L CNN
F 1 "22u" H 4100 5750 50  0000 L CNN
F 2 "footprints:TantalC_SizeS_EIA-3216" H 4050 5850 60  0001 C CNN
F 3 "" H 4050 5850 60  0001 C CNN
	1    4050 5850
	1    0    0    -1  
$EndComp
$Comp
L C-RESCUE-telesoarer-mlf C1
U 1 1 4ED087A6
P 3650 5850
F 0 "C1" H 3700 5950 50  0000 L CNN
F 1 "0u1" H 3700 5750 50  0000 L CNN
F 2 "footprints:C_0603" H 3650 5850 60  0001 C CNN
F 3 "" H 3650 5850 60  0001 C CNN
	1    3650 5850
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR07
U 1 1 4ECC4A1D
P 2550 4250
F 0 "#PWR07" H 2550 4340 20  0001 C CNN
F 1 "+5V" V 2600 4400 30  0000 C CNN
F 2 "" H 2550 4250 60  0001 C CNN
F 3 "" H 2550 4250 60  0001 C CNN
	1    2550 4250
	0    -1   -1   0   
$EndComp
Text Label 3650 4250 2    60   ~ 0
RESET
$Comp
L R R1
U 1 1 4ECC49EE
P 2900 4250
F 0 "R1" V 3050 4350 50  0000 C CNN
F 1 "5K6" V 2900 4250 50  0000 C CNN
F 2 "footprints:R_0603" H 2900 4250 60  0001 C CNN
F 3 "" H 2900 4250 60  0001 C CNN
	1    2900 4250
	0    1    1    0   
$EndComp
Text Label 3650 4050 2    60   ~ 0
SCK
Text Label 3650 3950 2    60   ~ 0
MISO
Text Label 3650 3850 2    60   ~ 0
MOSI
$Comp
L GND-RESCUE-telesoarer-mlf #PWR08
U 1 1 4ECC4963
P 4800 5500
F 0 "#PWR08" H 4800 5500 30  0001 C CNN
F 1 "GND" H 4800 5430 30  0001 C CNN
F 2 "" H 4800 5500 60  0001 C CNN
F 3 "" H 4800 5500 60  0001 C CNN
	1    4800 5500
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR09
U 1 1 4ECC494B
P 4800 2350
F 0 "#PWR09" H 4800 2440 20  0001 C CNN
F 1 "+5V" H 4850 2500 30  0000 C CNN
F 2 "" H 4800 2350 60  0001 C CNN
F 3 "" H 4800 2350 60  0001 C CNN
	1    4800 2350
	1    0    0    -1  
$EndComp
$Comp
L ATMEGA328P IC1
U 1 1 4ECC4939
P 4800 3750
F 0 "IC1" H 4800 4200 60  0000 C CNN
F 1 "ATMEGA328P" H 4800 4350 60  0000 C CNN
F 2 "footprints:MLF-32_2mmVIA_SINK" H 4800 3750 60  0001 C CNN
F 3 "" H 4800 3750 60  0001 C CNN
	1    4800 3750
	1    0    0    -1  
$EndComp
Text Label 2400 5850 0    60   ~ 0
RESET
Text Label 2400 5750 0    60   ~ 0
MOSI
Text Label 2400 5650 0    60   ~ 0
SCK
Text Label 2400 5450 0    60   ~ 0
MISO
$Comp
L GND-RESCUE-telesoarer-mlf #PWR010
U 1 1 4ECB2F15
P 2250 6150
F 0 "#PWR010" H 2250 6150 30  0001 C CNN
F 1 "GND" H 2250 6080 30  0001 C CNN
F 2 "" H 2250 6150 60  0001 C CNN
F 3 "" H 2250 6150 60  0001 C CNN
	1    2250 6150
	-1   0    0    -1  
$EndComp
NoConn ~ 5750 4350
NoConn ~ 3800 3650
$Comp
L NPN-BCE Q1
U 1 1 51B6AE78
P 7100 5850
F 0 "Q1" H 7400 5850 50  0000 R CNN
F 1 "BC847C" H 7550 5750 50  0000 R CNN
F 2 "footprints:SOT23-BCE" H 7100 5850 60  0001 C CNN
F 3 "" H 7100 5850 60  0000 C CNN
	1    7100 5850
	1    0    0    -1  
$EndComp
NoConn ~ 5750 3450
NoConn ~ 5750 3050
NoConn ~ 5750 2950
NoConn ~ 5750 4450
NoConn ~ 3800 4750
NoConn ~ 3800 4550
$Comp
L AUTOTRANS L1
U 1 1 5470FC2F
P 8850 5650
F 0 "L1" V 9000 5600 40  0000 C CNN
F 1 "AUTOTRANS" V 9100 5500 40  0000 C CNN
F 2 "footprints:TRI-3" H 8850 5600 60  0001 C CNN
F 3 "~" H 8850 5600 60  0000 C CNN
	1    8850 5650
	0    1    1    0   
$EndComp
NoConn ~ 5750 4150
NoConn ~ 5750 3950
$Comp
L GND-RESCUE-telesoarer-mlf #PWR011
U 1 1 5470FFF7
P 1850 2250
F 0 "#PWR011" H 1850 2250 30  0001 C CNN
F 1 "GND" H 1850 2180 30  0001 C CNN
F 2 "" H 1850 2250 60  0001 C CNN
F 3 "" H 1850 2250 60  0001 C CNN
	1    1850 2250
	-1   0    0    -1  
$EndComp
$Comp
L +5V #PWR012
U 1 1 54710003
P 1850 1450
F 0 "#PWR012" H 1850 1540 20  0001 C CNN
F 1 "+5V" H 1900 1600 30  0000 C CNN
F 2 "" H 1850 1450 60  0001 C CNN
F 3 "" H 1850 1450 60  0001 C CNN
	1    1850 1450
	1    0    0    -1  
$EndComp
Text Label 2050 1850 0    60   ~ 0
SIG
Text Label 3050 3550 2    60   ~ 0
SIG
$Comp
L R R2
U 1 1 547100F8
P 3350 3550
F 0 "R2" V 3430 3550 50  0000 C CNN
F 1 "200" V 3350 3550 50  0000 C CNN
F 2 "footprints:R_0603" H 3350 3550 60  0001 C CNN
F 3 "" H 3350 3550 60  0001 C CNN
	1    3350 3550
	0    1    1    0   
$EndComp
Text Notes 6400 3650 0    60   ~ 0
Debug UART\nBreak-out
$Comp
L CONN_01X03 K2
U 1 1 5478F3CC
P 6650 3850
F 0 "K2" H 6650 4050 50  0000 C CNN
F 1 "G  T  R" V 6750 3850 40  0000 C CNN
F 2 "footprints:TP-3" H 6650 3850 60  0001 C CNN
F 3 "" H 6650 3850 60  0000 C CNN
	1    6650 3850
	1    0    0    1   
$EndComp
$Comp
L GND-RESCUE-telesoarer-mlf #PWR013
U 1 1 5478F49D
P 6350 4050
F 0 "#PWR013" H 6350 4050 30  0001 C CNN
F 1 "GND" H 6350 3980 30  0001 C CNN
F 2 "" H 6350 4050 60  0001 C CNN
F 3 "" H 6350 4050 60  0001 C CNN
	1    6350 4050
	1    0    0    -1  
$EndComp
Connection ~ 5500 1700
Wire Wire Line
	5500 1700 5300 1700
Wire Wire Line
	4200 1100 4350 1100
Wire Wire Line
	4200 1700 4350 1700
Wire Wire Line
	4650 1400 4900 1400
Wire Wire Line
	2150 4050 3800 4050
Wire Wire Line
	2250 5250 2250 5550
Wire Wire Line
	2250 5550 2050 5550
Wire Wire Line
	4050 5400 4050 5700
Wire Wire Line
	3300 5500 4050 5500
Wire Wire Line
	3650 5500 3650 5650
Wire Wire Line
	2150 3850 3800 3850
Wire Wire Line
	2250 6150 2250 5950
Wire Wire Line
	2050 5850 2400 5850
Wire Wire Line
	2050 5650 2400 5650
Wire Wire Line
	2050 5450 2400 5450
Wire Wire Line
	2050 5750 2400 5750
Wire Wire Line
	2250 5950 2050 5950
Wire Wire Line
	2150 3950 3800 3950
Connection ~ 4050 5500
Wire Wire Line
	4650 1100 4900 1100
Wire Wire Line
	4650 1700 4900 1700
Wire Wire Line
	4200 1400 4350 1400
Wire Wire Line
	5300 1400 5500 1400
Connection ~ 5500 1400
Wire Wire Line
	6750 5850 6900 5850
Wire Wire Line
	7200 6050 7200 6200
Wire Wire Line
	5300 1100 5500 1100
Wire Wire Line
	5500 1100 5500 2000
Wire Wire Line
	1500 2050 1850 2050
Wire Wire Line
	1500 1950 1850 1950
Wire Wire Line
	1850 1950 1850 1450
Wire Wire Line
	1850 2050 1850 2250
Wire Wire Line
	1500 1850 2750 1850
Wire Wire Line
	3500 3550 3800 3550
Wire Wire Line
	2750 3550 3200 3550
Wire Wire Line
	4050 6000 4050 6350
Wire Wire Line
	3650 6050 3650 6200
Wire Wire Line
	3300 6200 4050 6200
Connection ~ 4050 6200
Wire Wire Line
	7200 5450 7200 5650
Wire Wire Line
	5750 3750 6450 3750
Wire Wire Line
	5750 3850 6450 3850
Wire Wire Line
	6350 3950 6450 3950
Wire Wire Line
	6350 3950 6350 4050
Wire Wire Line
	4700 5250 4700 5350
Wire Wire Line
	4700 5350 4900 5350
Wire Wire Line
	4900 5350 4900 5250
$Comp
L GND-RESCUE-telesoarer-mlf #PWR014
U 1 1 5812887F
P 5750 5250
F 0 "#PWR014" H 5750 5250 30  0001 C CNN
F 1 "GND" H 5750 5180 30  0001 C CNN
F 2 "" H 5750 5250 60  0001 C CNN
F 3 "" H 5750 5250 60  0001 C CNN
	1    5750 5250
	1    0    0    -1  
$EndComp
Wire Wire Line
	5750 4750 5750 5250
Wire Wire Line
	4700 2500 4700 2400
Wire Wire Line
	4700 2400 4900 2400
Wire Wire Line
	4800 2350 4800 2500
Connection ~ 4800 2400
Wire Wire Line
	4900 2400 4900 2500
$Comp
L CONN_01X03 K1
U 1 1 581563C9
P 1300 1950
F 0 "K1" H 1300 1750 50  0000 C CNN
F 1 "RC SIG IN" H 1600 1950 50  0000 C CNN
F 2 "footprints:SIL-3" H 1250 2300 50  0001 C CNN
F 3 "" H 1300 1950 50  0000 C CNN
	1    1300 1950
	-1   0    0    1   
$EndComp
$Comp
L CONN_01X06 P1
U 1 1 5815651E
P 1850 5700
F 0 "P1" H 1850 6050 50  0000 C CNN
F 1 "ATMEL ISP" H 2150 5700 50  0000 C CNN
F 2 "footprints:PIN_ARRAY_3x2_GRUVIN" H 1700 4900 50  0001 C CNN
F 3 "" H 1850 5700 50  0000 C CNN
	1    1850 5700
	-1   0    0    -1  
$EndComp
Wire Wire Line
	4800 5250 4800 5500
Connection ~ 4800 5350
$Comp
L Q_NMOS_DSG Q2
U 1 1 5815B84B
P 7700 4350
F 0 "Q2" H 8000 4400 50  0000 R CNN
F 1 "2N7000" H 8150 4300 50  0000 R CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H 7900 4450 50  0001 C CNN
F 3 "" H 7700 4350 50  0000 C CNN
	1    7700 4350
	1    0    0    -1  
$EndComp
$Comp
L R R8
U 1 1 5815B900
P 7050 4350
F 0 "R8" V 7130 4350 50  0000 C CNN
F 1 "220K" V 7050 4350 50  0000 C CNN
F 2 "footprints:R_0603" V 6980 4350 50  0001 C CNN
F 3 "" H 7050 4350 50  0000 C CNN
	1    7050 4350
	0    1    1    0   
$EndComp
$Comp
L R R10
U 1 1 5815B971
P 7550 4050
F 0 "R10" V 7630 4050 50  0000 C CNN
F 1 "330K" V 7550 4050 50  0000 C CNN
F 2 "footprints:R_0603" V 7480 4050 50  0001 C CNN
F 3 "" H 7550 4050 50  0000 C CNN
	1    7550 4050
	0    1    1    0   
$EndComp
Wire Wire Line
	7200 4350 7500 4350
Wire Wire Line
	7400 4050 7350 4050
Wire Wire Line
	7350 3950 7350 4350
Connection ~ 7350 4350
$Comp
L R R9
U 1 1 5815BB15
P 7350 3800
F 0 "R9" V 7430 3800 50  0000 C CNN
F 1 "220K" V 7350 3800 50  0000 C CNN
F 2 "footprints:R_0603" V 7280 3800 50  0001 C CNN
F 3 "" H 7350 3800 50  0000 C CNN
	1    7350 3800
	1    0    0    -1  
$EndComp
Connection ~ 7350 4050
$Comp
L R R11
U 1 1 5815BCF5
P 7800 3800
F 0 "R11" V 7880 3800 50  0000 C CNN
F 1 "1K" V 7800 3800 50  0000 C CNN
F 2 "footprints:R_0603" V 7730 3800 50  0001 C CNN
F 3 "" H 7800 3800 50  0000 C CNN
	1    7800 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	7800 3950 7800 4150
Wire Wire Line
	7700 4050 8450 4050
Connection ~ 7800 4050
$Comp
L +3V3 #PWR015
U 1 1 5815BF76
P 7800 3450
F 0 "#PWR015" H 7800 3300 50  0001 C CNN
F 1 "+3V3" H 7800 3590 50  0000 C CNN
F 2 "" H 7800 3450 50  0000 C CNN
F 3 "" H 7800 3450 50  0000 C CNN
	1    7800 3450
	1    0    0    -1  
$EndComp
$Comp
L +3V3 #PWR016
U 1 1 5815BFCD
P 7350 3450
F 0 "#PWR016" H 7350 3300 50  0001 C CNN
F 1 "+3V3" H 7350 3590 50  0000 C CNN
F 2 "" H 7350 3450 50  0000 C CNN
F 3 "" H 7350 3450 50  0000 C CNN
	1    7350 3450
	1    0    0    -1  
$EndComp
Wire Wire Line
	7350 3650 7350 3450
Wire Wire Line
	7800 3650 7800 3450
$Comp
L GND #PWR017
U 1 1 5815C0E8
P 7800 4700
F 0 "#PWR017" H 7800 4450 50  0001 C CNN
F 1 "GND" H 7800 4550 50  0000 C CNN
F 2 "" H 7800 4700 50  0000 C CNN
F 3 "" H 7800 4700 50  0000 C CNN
	1    7800 4700
	1    0    0    -1  
$EndComp
Wire Wire Line
	7800 4700 7800 4550
$Comp
L C-RESCUE-telesoarer-mlf C3
U 1 1 5815C274
P 6800 4650
F 0 "C3" H 6850 4750 50  0000 L CNN
F 1 "0u1" H 6850 4550 50  0000 L CNN
F 2 "footprints:C_0603" H 6800 4650 60  0001 C CNN
F 3 "" H 6800 4650 60  0001 C CNN
	1    6800 4650
	1    0    0    -1  
$EndComp
$Comp
L R R7
U 1 1 5815C393
P 6550 4350
F 0 "R7" V 6630 4350 50  0000 C CNN
F 1 "56K" V 6550 4350 50  0000 C CNN
F 2 "footprints:R_0603" V 6480 4350 50  0001 C CNN
F 3 "" H 6550 4350 50  0000 C CNN
	1    6550 4350
	0    1    1    0   
$EndComp
Wire Wire Line
	6400 4350 6100 4350
Wire Wire Line
	5750 4050 6100 4050
Wire Wire Line
	6700 4350 6900 4350
Wire Wire Line
	6800 4450 6800 4350
Connection ~ 6800 4350
Wire Wire Line
	6800 4850 6800 4950
$Comp
L GND #PWR018
U 1 1 5815D1DB
P 6800 4950
F 0 "#PWR018" H 6800 4700 50  0001 C CNN
F 1 "GND" H 6800 4800 50  0000 C CNN
F 2 "" H 6800 4950 50  0000 C CNN
F 3 "" H 6800 4950 50  0000 C CNN
	1    6800 4950
	1    0    0    -1  
$EndComp
Wire Wire Line
	6450 5850 5950 5850
Wire Wire Line
	5950 5850 5950 4250
Wire Wire Line
	5950 4250 5750 4250
Wire Wire Line
	6100 4050 6100 4350
Wire Wire Line
	8350 5350 9300 5350
Wire Wire Line
	8800 5350 8800 5550
Wire Wire Line
	9300 5550 9100 5550
$Comp
L CONN_01X02 P2
U 1 1 5815E6BA
P 7500 5400
F 0 "P2" H 7500 5550 50  0000 C CNN
F 1 "PZ" H 7600 5400 50  0000 C CNN
F 2 "footprints:PIN_ARRAY_2X1" H 7500 5400 50  0001 C CNN
F 3 "" H 7500 5400 50  0000 C CNN
	1    7500 5400
	1    0    0    1   
$EndComp
$Comp
L CONN_01X02 P3
U 1 1 5815E744
P 8150 5400
F 0 "P3" H 8150 5550 50  0000 C CNN
F 1 "PZ_IN" V 8250 5400 50  0000 C CNN
F 2 "footprints:PIN_ARRAY_2X1" H 8150 5400 50  0001 C CNN
F 3 "" H 8150 5400 50  0000 C CNN
	1    8150 5400
	-1   0    0    1   
$EndComp
Wire Wire Line
	7200 5450 7300 5450
Wire Wire Line
	7300 5350 7200 5350
Wire Wire Line
	7200 5350 7200 5050
Wire Wire Line
	8350 5450 8600 5450
Wire Wire Line
	8600 5450 8600 5550
Connection ~ 8800 5350
Wire Notes Line
	7950 4950 9850 4950
Wire Notes Line
	9850 4950 9850 6050
Wire Notes Line
	9850 6050 7950 6050
Wire Notes Line
	7950 6050 7950 4950
$Comp
L MS5611 U1
U 1 1 5815FC96
P 1650 3650
F 0 "U1" H 1250 4000 50  0000 L CNN
F 1 "MS5611" H 1500 3550 50  0000 L CNN
F 2 "footprints:MS5611_8_3mmx5mm_SMD" H 1650 3650 50  0001 C CNN
F 3 "" H 1650 3650 50  0000 C CNN
	1    1650 3650
	1    0    0    -1  
$EndComp
Wire Wire Line
	2300 3750 3800 3750
Wire Wire Line
	2300 3500 2300 3750
Wire Wire Line
	2300 3600 2150 3600
Wire Wire Line
	2150 3500 2300 3500
Connection ~ 2300 3600
$Comp
L +5V #PWR019
U 1 1 58160BFF
P 1650 3050
F 0 "#PWR019" H 1650 3140 20  0001 C CNN
F 1 "+5V" H 1700 3200 30  0000 C CNN
F 2 "" H 1650 3050 60  0001 C CNN
F 3 "" H 1650 3050 60  0001 C CNN
	1    1650 3050
	1    0    0    -1  
$EndComp
$Comp
L GND-RESCUE-telesoarer-mlf #PWR020
U 1 1 58160C61
P 1650 4400
F 0 "#PWR020" H 1650 4400 30  0001 C CNN
F 1 "GND" H 1650 4330 30  0001 C CNN
F 2 "" H 1650 4400 60  0001 C CNN
F 3 "" H 1650 4400 60  0001 C CNN
	1    1650 4400
	1    0    0    -1  
$EndComp
Wire Wire Line
	1650 4250 1650 4400
Wire Wire Line
	1650 3250 1650 3050
Wire Wire Line
	1150 3750 1050 3750
Wire Wire Line
	1050 3750 1050 4300
Wire Wire Line
	1050 4300 1650 4300
Connection ~ 1650 4300
Wire Wire Line
	2750 1850 2750 3550
Wire Wire Line
	2550 4250 2750 4250
Wire Wire Line
	3050 4250 3800 4250
Text Label 3650 3750 2    60   ~ 0
~SS
$Comp
L CONN_01X02 P4
U 1 1 581633CD
P 8650 4100
F 0 "P4" H 8650 4250 50  0000 C CNN
F 1 "AOUT" H 8850 4100 50  0000 C CNN
F 2 "footprints:PIN_ARRAY_2X1" H 8650 4100 50  0001 C CNN
F 3 "" H 8650 4100 50  0000 C CNN
	1    8650 4100
	1    0    0    1   
$EndComp
Wire Wire Line
	8450 4150 8300 4150
Wire Wire Line
	8300 4150 8300 4500
$Comp
L GND #PWR021
U 1 1 581635EF
P 8300 4500
F 0 "#PWR021" H 8300 4250 50  0001 C CNN
F 1 "GND" H 8300 4350 50  0000 C CNN
F 2 "" H 8300 4500 50  0000 C CNN
F 3 "" H 8300 4500 50  0000 C CNN
	1    8300 4500
	1    0    0    -1  
$EndComp
$Comp
L C-RESCUE-telesoarer-mlf C4
U 1 1 58164239
P 3300 5850
F 0 "C4" H 3350 5950 50  0000 L CNN
F 1 "0u1" H 3350 5750 50  0000 L CNN
F 2 "footprints:C_0603" H 3300 5850 60  0001 C CNN
F 3 "" H 3300 5850 60  0001 C CNN
	1    3300 5850
	1    0    0    -1  
$EndComp
Wire Wire Line
	3300 5650 3300 5500
Connection ~ 3650 5500
Wire Wire Line
	3300 6050 3300 6200
Connection ~ 3650 6200
Wire Wire Line
	7200 5250 7200 5150
Wire Wire Line
	7200 5150 8400 5150
Wire Wire Line
	8400 5150 8400 5350
Connection ~ 8400 5350
Connection ~ 7200 5250
Wire Wire Line
	7200 5600 8400 5600
Wire Wire Line
	8400 5600 8400 5450
Connection ~ 8400 5450
Connection ~ 7200 5600
$EndSCHEMATC
