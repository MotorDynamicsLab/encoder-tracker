# encoder-tracker
Tracks up to four quadrature encoder signals, encoder counts can then be transmitted via SPI to another microcontroller.

Originally developed for the Farmduino controller to offload resource intensive encoder tracking to a seperate microcontroller. The STM32 microcontroller with its general purpose timers are ideally suited to tracking multiple quadrature signals at once.  

## Communication Protocol
After CS is enabled, slave awaits 8 bit command from master, slave then transfers data of variable length depending on command  

|     |     |
| --- | --- |  
| Chip select pin |	D22 (PA0) |	 
| CPOL | 1 |  
| CPHA | 1 |  
| Master |	Arduino Mega |  
| Slave	| STM32	|  
| Format |	8 bit command byte + (optional data from slave) (MSB first)	|  


### STM32 function	
STM32 will use its onboard general purpose timers to simultaneously keep track of all 4 encoder signals. Positional information for each encoder will be stored as 32bit signed integer. 	
### Reset Behavior	 
On a power-on reset, the STM32 will initialize all 4 encoder positions to 0. When a motor moves forward, its corresponding position increments; when the motor reveres, the position decrements.	
### Position Overflow	 
In the unlikely case of a positional overflow, (0x7FFFFFFF + 1 or 0xFFFFFFFF - 1), the positional information will wrap around (2,147,483,647 + 1 = -2,147,483,648 and vice versa)	
		
		
### Reset Position Command	
The slave will reset the position of selected encoders to zero, encoders are selected by setting its corresponding bit in the most significant 4 bits of the command. No subsequent data is transferred from slave.  

| Bit 0-3	| Bit 4-7 |
| --- | --- |
| 0b0000	| bit4 = X1, bit5 = X2, bit6 = Y, bit7 = Z |
		
### Read Position Command
The slave will transfer the positional data of selected encoders to the master after the 8bit command. Encoders are selected by setting its corresponding bit in the most significant 4 bits of the command. Positional data for each encoder is 32bit wide so the length of the subsequent data transfer depends on how many encoders are selected. For example, if 1 encoder is selected, 4 bytes will be transferred, if 2 encoders are selected, 8 bytes will be transferred, etc. The order of the transfer is fixed, so X1 will be transferred first then X2, then Y, then Z - any unselected encoders will simply be skipped.	

| Bit 0-3	| Bit 4-7 |
| --- | --- |
|	0b1111	| bit4 = X1, bit5 = X2, bit6 = Y, bit7 = Z |

