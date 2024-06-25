/*
 * Lavuhka_00.c
 *
 * Created: 12.06.2015 11:36:24
 *  Author: ������� �.� 
 */ 
#include <avr/io.h>
#include <avr/interrupt.h> // ���������� ����������  
#define Servo 5            // �����������  
#define IRsensor 4         // �� ������  
#define button 6           // ������ �����������
#define sound_input 1      // ����������� ������ ��� ��������� ������
#define sound_output 7     // ����������� ������ ��� ��������� ������
#define sound_reset 0      // ����������� ������ ��� ��������� ������
volatile unsigned int action,pause2;   
 ISR  (TIMER0_COMP_vect)  //���������� ������� T0
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
	   if (!( action &(1<<0) ))     // ������������� ������������ �������
	    {		  	
		  if(!(PINB & (1<<IRsensor)))  // ��� ������ ������ ���� ���� ( ��� �������� )
		   {
	         action |=(1<<0);       // ����� � ������������
			 PORTD &=~(1<<5);          
		   }			 
		  else
		     PORTD |=(1<<5);		    	
	    }
	 //===================//		   				   
	  else
		{
		  if (!( action & (1 << 1)))         // ���� ��������� 
		   {
		     if ( PINB & (1 << IRsensor))    // ������ �������� ������������ 
		      {			  		    			
			    pause2 = 0;                  // ����� ������ ���������
			    PORTA |=(1 << Servo);        // ������ ������ 
				PORTC |=(1<<sound_output);  
				PORTC &=~(1<<sound_reset); 		        
			    action |=(1<<1);
		      }
			  else
			  {	
				if (!(action & (1 << 2))) 
				 {
				   if (!( PINC & (1<<sound_input))) // ������ ����� 
					  PORTC &=~(1<<sound_output); 					  	
					else
					{
					  PORTC |=(1<<sound_output);    // ���� ���������������
					  action |= (1 << 2); 
					  pause2 = 0;	
					}										   	 
				 }
				else
				 if (!( PINC & (1<<sound_input))) //  ��������������� ����� ���������
				  {		  
					if ( pause2 > 50000 )   //����� �� ��������� �����
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
		      PORTA &=~(1 << Servo);   // ��������� ������
			  PORTC |=(1<<sound_reset);
		      pause2 = 0; 
			  if ( PIND & (1<<5))      // � ����� ��������� ��������� ����������
				PORTD &=~(1<<5);      
			  else
				PORTD |=(1<<5);   			   	
	        }	   							   		 			 
		   if (!( PIND & (1<<button) ))  // ������ ����� ������� (������)
			 action = 0;             	  		  				
		 }		 
       }
	 }	   
}