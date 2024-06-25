/*
 * Lavuhka_00.c
 *
 * Created: 12.06.2015 11:36:24
 *  Author: ѕисанец ј.Ќ 
 */ 
#include <avr/io.h>
#include <avr/interrupt.h> // подгружаем прерывание  
#define Servo 5            // сервопривод  
#define IRsensor 4         // ик датчик  
#define button 6           // кнопка перезапуска
#define sound_input 1      // управл€ющий сигнал дл€ звукового модул€
#define sound_output 7     // управл€ющий сигнал дл€ звукового модул€
#define sound_reset 0      // управл€ющий сигнал дл€ звукового модул€
volatile unsigned int action,pause2;   
 ISR  (TIMER0_COMP_vect)  //прерывание таймера T0
  {
	pause2++; 
  }	 
//*******************//
//*******************//
int main(void)
{
	DDRC &=~(1<<sound_input);
	DDRC |= (1<<sound_output);	
	PORTC |=(1<<sound_output);
	DDRC |=(1<<sound_reset);
	PORTC |=(1<<sound_reset);
	TCCR0 = 0b00001101;  
    TIMSK|=(1<<1);
	DDRA |=(1 << Servo);     
	DDRB &=~(1 << IRsensor);  
	DDRD &=~(1 << button);   
	PORTD |=(1 << button);  
	DDRD |=(1<<5); 	           
	action |=(1<<0)|(1<<1);  
	asm("sei");
  //===========//   
    while(1)
    {
	   if (!( action &(1<<0) ))     // зафексировано срабатывание двтчика
	    {		  	
		  if(!(PINB & (1<<IRsensor)))  // как только датчик даст ноль ( нет движений )
		   {
	         action |=(1<<0);       // готов к отслеживанию
			 PORTD &=~(1<<5);          
		   }			 
		  else
		     PORTD |=(1<<5);		    	
	    }
	 //===================//		   				   
	  else
		{
		  if (!( action & (1 << 1)))         // цикл отработан 
		   {
		     if ( PINB & (1 << IRsensor))    // датчик движени€ зафиксировал 
		      {			  		    			
			    pause2 = 0;                  // врем€ работы двигател€
			    PORTA |=(1 << Servo);        // запуск мотора 
				PORTC |=(1<<sound_output);  
				PORTC &=~(1<<sound_reset); 		        
			    action |=(1<<1);
		      }
			  else
			  {	
				if (!(action & (1 << 2))) 
				 {
				   if (!( PINC & (1<<sound_input))) // запуск звука 
					  PORTC &=~(1<<sound_output); 					  	
					else
					{
					  PORTC |=(1<<sound_output);    // звук воспроизводитс€
					  action |= (1 << 2); 
					  pause2 = 0;	
					}										   	 
				 }
				else
				 if (!( PINC & (1<<sound_input))) //  воспроизведение звука закончено
				  {		  
					if ( pause2 > 50000 )   //пауза на включение звука
					 {
					   action &=~ (1 << 2);
					 }					  
				  }					     				    						  			  			  			  
		     }
		 }		  	 
		else
		  { 	
		   if ( pause2 > 1000)            
	        {
		      PORTA &=~(1 << Servo);   // остановка мотора
			  PORTC |=(1<<sound_reset);
		      pause2 = 0; 
			  if ( PIND & (1<<5))      // в каком состо€нии светодиод сигнальный
				PORTD &=~(1<<5);      
			  else
				PORTD |=(1<<5);   			   	
	        }	   							   		 			 
		   if (!( PIND & (1<<button) ))  // запуск цикла сначала (кнопка)
			 action = 0;             	  		  				
		 }		 
       }
	 }	   
}