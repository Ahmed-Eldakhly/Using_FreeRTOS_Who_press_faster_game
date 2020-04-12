



#include "UserTasks.h"


void main(void)
{
	xTaskCreate(init_Task  , "Init_Task" , configMINIMAL_STACK_SIZE ,
			NULL , (5 | portPRIVILEGE_BIT) , &InitTask_Flag);

	/* Start Scheduler */
	DIO_SetPinDirection(DIO_PIN12,OUTPUT);
	vTaskStartScheduler();

	while(1)
	{

	}
}

