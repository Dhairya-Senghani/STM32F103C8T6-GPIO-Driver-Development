
#include "gpio_drive.h"

void Init_GPIO(unsigned short port, unsigned short pin, unsigned short direction, unsigned short option)
{
	volatile unsigned long *CR;
	unsigned short tPIN = pin;
	unsigned short offset = 0x00;
	
	if(pin > 7)
	{
		tPIN -= 8;
		offset = 0x01;
	}
	
	if(port == PORT_A)
	{
		RCC_APB2ENR |= (1 << 2);  // Enable GPIO A Clock
		CR = (volatile unsigned long *) (&GPIO_A + offset);
	}
	else if(port == PORT_B)
	{
		RCC_APB2ENR |= (1 << 3);  // Enable GPIO_B  Clock
		CR = (volatile unsigned long *) (&GPIO_B + offset);
	}
	else if(port == PORT_C)
	{
		RCC_APB2ENR |= (1 << 4);  // Enable GPIO_C Clock
		CR = (volatile unsigned long *) (&GPIO_C + offset);
	}

	*CR &= ~(0xF << (tPIN * 4));   // Reset the target pin
	*CR |= ((direction << (tPIN * 4)) | (option << ((tPIN * 4) + 2)));  // Setup the direction and options of the pin
	
}



int Read_GPIO(unsigned short port, unsigned short pin)
{
	volatile unsigned long *IDR;
	unsigned long offset = 0x02;  // 0x08 offset in hexadecimal
	int state;
	if(port == PORT_A)
	{
		IDR = (volatile unsigned long *)(&GPIO_A + offset);
	}
	else if(port == PORT_B)
	{
		IDR = (volatile unsigned long *)(&GPIO_B + offset);
	}
	else if(port == PORT_C)
	{
		IDR = (volatile unsigned long *)(&GPIO_C + offset);
	}
	
	state = (((*IDR) &  (1 << pin)) >> pin); 
	return state;
}



void Write_GPIO(unsigned short port, unsigned short pin, unsigned short state)
{
	volatile unsigned long *ODR;
	unsigned long offset = 0x03;  // 0x0C offset in hexadecimal
	
	if(port == PORT_A)
	{
		ODR = (volatile unsigned long *)(&GPIO_A + offset);
	}
	else if(port == PORT_B)
	{
		ODR = (volatile unsigned long *)(&GPIO_B + offset);
	}
	else if(port == PORT_C)
	{
		ODR = (volatile unsigned long *)(&GPIO_C + offset);
	}
	
	(state == HIGH) ? (*ODR |= (1 << pin)) : (*ODR &= ~(1 << pin));

}


void Toggle_GPIO(unsigned short port, unsigned short pin)
{
	if(Read_GPIO(port, pin) == HIGH)
	{
		Write_GPIO(port, pin, LOW);
	}
	else
	{
		Write_GPIO(port, pin, HIGH);
	}
}
