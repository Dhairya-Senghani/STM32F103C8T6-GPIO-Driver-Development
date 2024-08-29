#include "stm32f10x.h"                  // Device header
#include "gpio_drive.h"

void Delay(int);

int signal = 0;

int main(void)
{
	
	Init_GPIO(PORT_A, 0, IN, I_PP);        // For interrupt pin
	Init_GPIO(PORT_C, 13, OUT50, O_GP_PP); // Blinking inbuilt LED
	Init_GPIO(PORT_A, 12, OUT50, O_GP_PP); // For interrupt LED
	
	__disable_irq();
	AFIO->EXTICR[0] = 0x00;
	EXTI->IMR |= (1 << 0);  // Unmask interrupt 0
	EXTI->RTSR |= (1 << 0);  // Set interrupt at rising edge
	//EXTI->FTSR |= (1 << 0);  // Set interrupt at falling edge
	NVIC_EnableIRQ(EXTI0_IRQn);
	__enable_irq();
	
	while(1)
	{
		
		Write_GPIO(PORT_A, 12, signal);

		if(signal == 1)
		{
			Delay(10);
			Toggle_GPIO(PORT_C, 13);
			Delay(10);
		}
		else
		{
			Write_GPIO(PORT_C, 13, HIGH);
		}
	}
}


void Delay(int time)
{
	for(; time > 0; time--){
		int i;
		for(i = 0; i < 100000; i++)
		{
		}
	}
}


void EXTI0_IRQHandler()
{
	EXTI->PR |= 1;
	
	if(signal == 1)
	{
		signal = 0;
	}
	else{
		signal = 1;
	}

}
