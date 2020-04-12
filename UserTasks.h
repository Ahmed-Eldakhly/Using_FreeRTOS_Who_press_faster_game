/*****************************************************************************
 * Module 	  : FreeRTOS User Story3 Tasks Prototypes
 *
 * File name  : UserStory3.h
 *
 * Created on : Oct 7, 2019
 *
 * Author     : Ahmed Eldakhly & Hesham Hafez
 ******************************************************************************/

#ifndef USERTASKS_H_
#define USERTASKS_H_

/*******************************************************************************
 *                       	Included Libraries                                 *
 *******************************************************************************/
#include "FreeRTOS.h"
#include "task.h"
#include "std_types.h"
#include "Macro.h"
#include "DIO.h"
#include "PushButton.h"
#include "lcd.h"


/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/
extern TaskHandle_t  InitTask_Flag;

/*******************************************************************************
 *                      Functions Prototypes        	                       *
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
void init_Task(void * a_Task_ptr);

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
void All_Hardware_Init_Task(void * a_Task_ptr);

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
void PushButtonA_Task(void * a_Task_ptr);

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
void PushButtonB_Task(void * a_Task_ptr);

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
void LCD_Task(void * a_Task_ptr);


#endif /* USERTASKS_H_ */
