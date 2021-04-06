                            /*
 * main.c    ATmega32    F_CPU = 8000000 Hz
 * 
 * Created on: 2021-01-02
 *     Author: admin
*/                                       
#include <avr/io.h>                       
#include <util/delay.h>                                                 
#include <stdbool.h>                                                                                              
#include <avr/interrupt.h>
#include "hd44780.h"                  
#include "ds18b20.h"                                                            
#include "dht.h"                                                                        
#include "bmp085.h"   
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1) 
                                                                                         
uint8_t wyswietl[]={126, 12, 182, 158, 204, 218, 250, 14, 254, 222}; 
int seconds = 0;                        
int minutes = 1;                                 
int hours = 0;                                                
bool readFromSensors = true;       
bool sendDataToServer = false;
                                                
ISR (TIMER1_COMPA_vect)    // Timer1 ISR                                                              
{                                     
    seconds++;                                                                         
                                                                  
    if(seconds == 60){   
        seconds = 0;                                        
        minutes++;                                                                                 
        readFromSensors = true;           
    }  
                                                                   
    if(minutes == 60){                                                  
        minutes = 0;                                                
        hours++;
    }
       
    if(hours > 23){                                                                  
        hours = 0;            
    }    
    
    if((minutes == 0 || minutes == 15 || minutes == 30 || minutes == 45) && (seconds == 0)){
       sendDataToServer = true;   
    }                                                     
    
}        
   
void UART_init(long USART_BAUDRATE)  
{
    UCSRB |= (1 << RXEN) | (1 << TXEN);   /* Turn on the transmission and reception */
    UCSRC |= (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1); /* Use 8-bit character sizes */
                       
    UBRRL = BAUD_PRESCALE;                     /* Load lower 8-bits of the baud rate value */
    UBRRH = (BAUD_PRESCALE >> 8);            /*Load upper 8-bits*/
}                            
                                                                                                  
unsigned char UART_RxChar()     
{
    while ((UCSRA & (1 << RXC)) == 0); /*Do nothing until data have been received*/
    return(UDR);                /* return the byte*/                                                                                      
}

void UART_TxChar(char ch)
{
    while (! (UCSRA & (1<<UDRE))); /*Wait for empty transmit buffer*/
    UDR = ch ;                                                           
}

void UART_SendString(char *str)                                   
{
    unsigned char j=0;
                                              
    while (str[j]!=0)    /*send string up to null */             
    {                                                                                                   
        UART_TxChar(str[j]);                                         
        j++;                         
    }
}                                                          
    

void reverse(char* str, int len) 
{ 
    int i = 0, j = len - 1, temp; 
    while (i < j) { 
        temp = str[i]; 
        str[i] = str[j];          
        str[j] = temp; 
        i++; 
        j--;           
    } 
}       
     
int intToStr(int x, char str[], int d) 
{ 
    int i = 0; 
    while (x) { 
        str[i++] = (x % 10) + '0'; 
        x = x / 10;                                   
    } 
  
    // If number of digits required is more, then 
    // add 0s at the beginning 
    while (i < d) 
        str[i++] = '0'; 
  
    reverse(str, i); 
    str[i] = '\0'; 
    return i; 
}                   
         
void ftoa(float n, char* res, int afterpoint) 
{ 
    int ipart = (int)n; 
  
    float fpart = n - (float)ipart; 
  
    int i = intToStr(ipart, res, 0); 
  
    if (afterpoint != 0) { 
        res[i] = '.'; // add dot 

        fpart = fpart * pow(10, afterpoint); 
  
        intToStr((int)fpart, res + i + 1, afterpoint); 
    } 
}                                                                                             
                                                                     
int main( void ) {               
    DDRA=0xFF;                                          
    PORTA=0x00;                
                                                        
    MCUCSR |= 0x80;                                                                             
    MCUCSR |= 0x80;                                 
    DDRC = 0xF0;                
    PORTC = 0x00;           

    TCCR1B |= (1 << WGM12); // Ustawienie trybu CT
    TCCR1B |= (1 << CS12); // Ustawienie preskalera na wartoœæ 256
    OCR1A = 31250; // Przepe³nienie co 1 sekundê
    TIMSK = 1<<OCIE1A;                                       
                        

    char buforIn[8];                                             
    float tempIn = 0;  
    char char_In[10];
    
    char buforOut[8];                                             
    float tempOut = 0;                             
    char char_Out[10];            
                                          
    char buforHum[8];                                                  
    int8_t humidity = 0;  
    char char_Hum[10]; 
       
    char buforPress[10];                                        
    int pressure = 0;       
    char char_Press[10];  
    
    char output_str[50];
                                               
    sei();                                                                                                                               
    bmp085_init();        
    LCD_Initalize();                                                   
    UART_init(9600);                                 
                                                                     
    while(1) {  
    
        if(readFromSensors){     
        tempIn=ds18b20_gettemp();        
        sprintf(buforIn,"Wew=%1.1f", tempIn);                 
        LCD_GoTo(0,0);                            
        LCD_WriteText(buforIn);                              
                                                                        
        tempOut=ds18b20_gettempOut();                                                           
        sprintf(buforOut,"Zew=%1.1f", tempOut);                
        LCD_GoTo(8,0);                            
        LCD_WriteText(buforOut);                                                          
                                                                                                       
        humidity = dht11_gethumidity();                              
        sprintf(buforHum,"Wilg=%1.1d", humidity);                                                    
        LCD_GoTo(0,1);                                                 
        LCD_WriteText(buforHum);                                                      
                                                                       
        pressure = bmp085_getpressure()/100;      
        sprintf(buforPress,"Cis=%1.1d", pressure);                       
        LCD_GoTo(8,1);                                                                        
        LCD_WriteText("Cis=981");  
                                                                      
        readFromSensors = false;                                        
        }                                                              
                                                                                                            
        if(sendDataToServer){                                                          
                                                                        
            ftoa(tempIn, char_In, 1);              
            ftoa(tempOut, char_Out, 1);   
            sprintf(char_Hum,"%1.1d", humidity);          
            sprintf(char_Press,"%1.1d", pressure);                   
            sprintf(output_str, "s%s,%s,%s,%se", char_In, char_Out, char_Hum, char_Press);   
            _delay_ms(1000);  
            UART_SendString(output_str);              
                                              
            sendDataToServer = false;
        }                                                     
                                                                    
              PORTA = ~wyswietl[hours / 10 % 10];         
              PORTC = (unsigned char)0x10;              
              _delay_ms(5);                                           
              PORTA = ~wyswietl[hours % 10];                        
              PORTC = (unsigned char)0x20;                  
              _delay_ms(5);                             
              PORTA = ~wyswietl[minutes / 10 % 10]; 
              PORTC = (unsigned char)0x40; 
              _delay_ms(5);  
              PORTA = ~wyswietl[minutes % 10]; 
              PORTC = (unsigned char)0x80;                                                                          
              _delay_ms(5);                        
              PORTC = 0x00;
        
    }                                
                                              
}                                                             