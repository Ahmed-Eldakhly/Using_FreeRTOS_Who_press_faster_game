/*****************************************************************************
 * Module 	  : FreeRTOS User Story3 Tasks Prototypes
 *
 * File name  : UserStory3.c
 *
 * Created on : Oct 7, 2019
 *
 * Author     : Ahmed Eldakhly & Hesham Hafez
 ******************************************************************************/

/*******************************************************************************
 *                       	Included Libraries                                 *
 *******************************************************************************/
#include "UserTasks.h"
#include "semphr.h"

/*******************************************************************************
 *                           static Global Variables                           *
 *******************************************************************************/
static TaskHandle_t  AllHardwareInit_Flag = NULL;

/*Queue between PushButtonA Task & LCD Task*/
static xQueueHandle Queue_PushButton_A;

/*Queue between PushButtonB Task & LCD Task*/
static xQueueHandle Queue_PushButton_B;

/*Mutex Handler*/
xSemaphoreHandle(FirstButtonPressed);

/*******************************************************************************
 *                           Global Variables                    		       *
 *******************************************************************************/
TaskHandle_t  InitTask_Flag = NULL;

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*******************************************************************************
 * Function Name:	init_Task
 *
 * Description: 	Create all tasks in the system
 *
 * Inputs:			pointer to void
 *
 * Outputs:			NULL
 *
 * Return:			NULL
 *******************************************************************************/
void init_Task(void * a_Task_ptr)
{
	/*Create Queues*/
	Queue_PushButton_A = xQueueCreate( 3 , sizeof(uint8) );
	Queue_PushButton_B = xQueueCreate( 3 , sizeof(uint8) );

	/*Create Mutex to make one Push Button is pressed*/
	FirstButtonPressed = xSemaphoreCreateMutex();



	/*Create 4 Tasks*/
	/*Task for initialize Hardware components*/
	xTaskCreate(All_Hardware_Init_Task  , "InitTasks" , configMINIMAL_STACK_SIZE ,
			NULL , (6 | portPRIVILEGE_BIT) , &AllHardwareInit_Flag);

	/*Task of Push Button 1*/
	xTaskCreate(PushButtonA_Task  , "PushButtonA_Task" , configMINIMAL_STACK_SIZE ,
			NULL , (4 | portPRIVILEGE_BIT) , NULL);

	/*Task of Push Button 2*/
	xTaskCreate(PushButtonB_Task  , "PushButtonB_Task" , configMINIMAL_STACK_SIZE ,
			NULL , (3 | portPRIVILEGE_BIT) , NULL);

	/*Task for LCD*/
	xTaskCreate(LCD_Task  , "LCD_Task" , configMINIMAL_STACK_SIZE ,
			NULL , (2 | portPRIVILEGE_BIT) , NULL);

	/*Suspend this task from the system*/
	vTaskSuspend( InitTask_Flag);
}

/*******************************************************************************
 * Function Name:	All_Hardware_Init_Task
 *
 * Description: 	Initialize LCD & KeyPad
 *
 * Inputs:			pointer to void
 *
 * Outputs:			NULL
 *
 * Return:			NULL
 *******************************************************************************/
void All_Hardware_Init_Task(void * a_Task_ptr)
{

	while(1)
	{
		/*initialize Hardware components*/
		PushButton_Init();
		LCD_init();
		DIO_SetPinDirection(DIO_PIN13,OUTPUT);

		/*Suspend this task from the system*/
		vTaskSuspend( AllHardwareInit_Flag);
	}
}

/*******************************************************************************
 * Function Name:	PushButtonA_Task
 *
 * Description: 	Push Button ATask
 *
 * Inputs:			pointer to void
 *
 * Outputs:			NULL
 *
 * Return:			NULL
 *******************************************************************************/
void PushButtonA_Task(void * a_Task_ptr)
{
	vTaskDelay(50);
	/* Data is sent to LCD by Queue*/
	uint8 SendData = 1;
	/*de_bouncing Flag to make sure key is pressed*/
	uint8 Debouncing = 0;

	/*comparing value to check if key is pressed*/
	uint8 OldPressedValue = 0;

	while(1)
	{
		/*Check if Button is pressed and enable de_bouncing*/
		if(Debouncing == 0 && Buttons_getPressedButton()==2)
		{
			OldPressedValue =  Buttons_getPressedButton();
			Debouncing++;
			/*de_bouncing Delay*/
			vTaskDelay(10);
		}
		else if(Debouncing == 1)
		{
			/*check if button is still pressed after de_bouncing time*/
			while(OldPressedValue == Buttons_getPressedButton())
			{
				/*Check if other push Button is pressed and if not take Mutex*/
				if (xSemaphoreTake(FirstButtonPressed , 10))
				{
					SendData = 1;
					/*Send push Button was pressed to Lcd by queue*/
					xQueueSend(Queue_PushButton_A , &SendData , 100);
					vTaskDelay(20);
				}
			}
			Debouncing = 2;
			vTaskDelay(20);
		}
		/*chech if button is released*/
		else if(Debouncing == 2)
		{
			/*released Mutex*/
			xSemaphoreGive(FirstButtonPressed);
			SendData = 0;
			/*Send push Button was released to Lcd by queue*/
			xQueueSend(Queue_PushButton_A , &SendData , 100);
			Debouncing = 0;
			vTaskDelay(20);
		}
		else
		{
			/*Send push Button is in Idle state to Lcd by queue*/
			SendData = 3;
			xQueueSend(Queue_PushButton_A , &SendData , 100);
			vTaskDelay(20);
		}
	}
}

/*******************************************************************************
 * Function Name:	PushButtonB_Task
 *
 * Description: 	Push Button B Task
 *
 * Inputs:			pointer to void
 *
 * Outputs:			NULL
 *
 * Return:			NULL
 *******************************************************************************/
void PushButtonB_Task(void * a_Task_ptr)
{
	vTaskDelay(60);
	/* Data is sent to LCD by Queue*/
	uint8 SendData = 1;
	/*de_bouncing Flag to make sure key is pressed*/
	uint8 Debouncing = 0;

	/*comparing value to check if key is pressed*/
	uint8 OldPressedValue = 0;

	while(1)
	{
		/*Check if Button is pressed and enable de_bouncing*/
		if(Debouncing == 0 && Buttons_getPressedButton()==1)
		{
			OldPressedValue =  Buttons_getPressedButton();
			Debouncing++;
			/*de_bouncing Delay*/
			vTaskDelay(10);
		}
		else if(Debouncing == 1)
		{
			/*check if button is still pressed after de_bouncing time*/
			while(OldPressedValue == Buttons_getPressedButton())
			{
				/*Check if other push Button is pressed and if not take Mutex*/
				if (xSemaphoreTake(FirstButtonPressed , 10))
				{
					SendData = 1;
					/*Send push Button was pressed to Lcd by queue*/
					xQueueSend(Queue_PushButton_B , &SendData , 100);
					vTaskDelay(20);
				}
			}
			Debouncing = 2;
			vTaskDelay(20);
		}
		/*chech if button is released*/
		else if(Debouncing == 2)
		{
			/*released Mutex*/
			xSemaphoreGive(FirstButtonPressed);
			SendData = 0;
			/*Send push Button was released to Lcd by queue*/
			xQueueSend(Queue_PushButton_B , &SendData , 100);
			Debouncing = 0;
			vTaskDelay(20);
		}
		else
		{
			/*Send push Button is in Idle state to Lcd by queue*/
			SendData = 3;
			xQueueSend(Queue_PushButton_B , &SendData , 100);
			vTaskDelay(20);
		}
	}
}
/*******************************************************************************
 * Function Name:	LCD_Task
 *
 * Description: 	LCD Task
 *
 * Inputs:			pointer to void
 *
 * Outputs:			NULL
 *
 * Return:			NULL
 *******************************************************************************/
void LCD_Task(void * a_Task_ptr)
{

	vTaskDelay(100);
	/*received data from Push Button A Task to display button state on LCD*/
	uint8 PushButton_A_Task_Data = 0;

	/*received message from PushButton_B Task to display button state on LCD*/
	uint8 PushButton_B_Task_Data = 0;

	/*Synchronize between displaying of Push Button task and hello task*/
	uint8 Flag = 0;
	uint8 PlayerOne = 0;
	uint8 PlayerTwo = 0;

	/*LED States & Buttons Strings*/
	uint8 ButtonA[] = "Player One ";
	uint8 ButtonB[] = "Player Two ";
	uint8 Player1_Win_Message[] = "Player 1 Won";
	uint8 Player2_Win_Message[] = "Player 2 Won";
	uint8 Congrat_Message[] = "Congratulations";

	while(1)
	{
		/*check if any player have 50 points*/
		if(PlayerOne ==50 || PlayerTwo==50)
		{
			/*if Player 1 get 50 points*/
			if(PlayerOne == 50)
			{
				LCD_clearScreen();
				LCD_displayString(Congrat_Message);
				LCD_displayStringRowColumn(1 , 0 , Player1_Win_Message);
			}
			/*if Player 2 get 50 points*/
			else
			{
				LCD_clearScreen();
				LCD_displayString(Congrat_Message);
				LCD_displayStringRowColumn(1 , 0 , Player2_Win_Message);
			}
			vTaskDelay(5000);
			PlayerOne = 0;
			PlayerTwo = 0;
			LCD_clearScreen();
		}
		else
		{
			/*receive messages from other tasks by queues*/
			xQueueReceive(Queue_PushButton_A , &PushButton_A_Task_Data , 10);
			xQueueReceive(Queue_PushButton_B , &PushButton_B_Task_Data , 10);

			/*Increment points of Player 1 if Push Button A was pressed*/
			if(PushButton_A_Task_Data == 1)
			{
				if(Flag == 1)
				{
					LCD_clearScreen();
				}
				PlayerOne++;
				/*Decrement points of Player 2 if Push Button A was pressed*/
				if(PlayerTwo>0)
				{
					PlayerTwo--;
				}
				/*display result on LCD*/
				LCD_displayStringRowColumn(0 , 0 , ButtonA);
				LCD_displayCharacter((PlayerOne/10)+48);
				LCD_displayCharacter((PlayerOne % 10)+48);
				LCD_displayStringRowColumn(1 , 0 , ButtonB);
				LCD_displayCharacter((PlayerTwo/10)+48);
				LCD_displayCharacter((PlayerTwo % 10)+48);
				PushButton_A_Task_Data = 0;
				Flag = 0;
			}
			/*Increment points of Player 2 if Push Button B was pressed*/
			else if(PushButton_B_Task_Data == 1)
			{
				if(Flag == 1)
				{
					LCD_clearScreen();
				}
				PlayerTwo++;
				/*Decrement points of Player 1 if Push Button B was pressed*/
				if(PlayerOne>0)
				{
					PlayerOne--;
				}
				/*display result on LCD*/
				LCD_displayStringRowColumn(0 , 0 , ButtonA);
				LCD_displayCharacter((PlayerOne/10)+48);
				LCD_displayCharacter((PlayerOne % 10)+48);
				LCD_displayStringRowColumn(1 , 0 , ButtonB);
				LCD_displayCharacter((PlayerTwo/10)+48);
				LCD_displayCharacter((PlayerTwo % 10)+48);
				PushButton_B_Task_Data = 0;
				Flag = 0;
			}
			else
			{
				/*Do Nothing*/
			}
			vTaskDelay(20);
		}
	}
}

