/*
 * shell.c
 *
 *  Created on: 7 juin 2019
 *      Author: Laurent Fiack
 */

#include "shell.h"

#include <stdio.h>

#include "usart.h"
#include "gpio.h"

#define ARGC_MAX 8
#define BUFFER_SIZE 40
#define NBR_MAX 10
#define LONG_MAX 100

char help[] = "help";

typedef struct
{
	char c;
	int (* func)(int argc, char ** argv);
	char * description;
} Shell_Func_t;

static int Shell_Func_List_Size = 0;
static Shell_Func_t Shell_Func_List[_SHELL_FUNC_LIST_MAX_SIZE];

static int Shell_Data_Ready = 0;

QueueHandle_t Shell_Queue;


int __io_putchar(int ch)
{
	HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
	return ch;
}

char Shell_Uart_Read()
{
	char c;

	HAL_UART_Receive(&UART_DEVICE, (uint8_t*)(&c), 1, 0xFFFFFFFF);

	return c;
}

int Shell_Uart_Write(char * s, uint16_t size)
{
	HAL_UART_Transmit(&UART_DEVICE, (uint8_t*)s, size, 0xFFFF);
	return size;
}

void Shell_Uart_Data_Ready()
{
	Shell_Data_Ready = 1;
}

int Shell_Help(int argc, char ** argv)
{
	int i;
	for(i = 0 ; i < Shell_Func_List_Size ; i++)
	{
		printf("%c %s\r\n", Shell_Func_List[i].c, Shell_Func_List[i].description);
	}

	return 0;
}


/**
 * @brief  Initialisation Shell
 * @retval void
 *
 */

void Shell_Init()
{
	printf("\r\n\r\n===== Shell v0.2 =====\r\n");

	Shell_Add('h', Shell_Help, help);

	for (int i = 0 ; i < 3 ; i++)
	{

		HAL_Delay(200);
	}

	Shell_Queue = xQueueCreate(1,sizeof(char));
}

int Shell_Add(char c, int (* pfunc)(int argc, char ** argv), char * description)
{
	if (Shell_Func_List_Size < _SHELL_FUNC_LIST_MAX_SIZE)
	{
		Shell_Func_List[Shell_Func_List_Size].c = c;
		Shell_Func_List[Shell_Func_List_Size].func = pfunc;
		Shell_Func_List[Shell_Func_List_Size].description = description;
		Shell_Func_List_Size++;
		return 0;
	}

	return -1;
}

int Shell_Exec(char c, char * buf)
{
	int i;
	int argc;
	char * argv[ARGC_MAX];
	char *p;

	for(i = 0 ; i < Shell_Func_List_Size ; i++)
	{
		if (Shell_Func_List[i].c == c)
		{
			argc = 1;
			argv[0] = buf;

			for(p = buf ; *p != '\0' && argc < ARGC_MAX ; p++)
			{
				if(*p == ' ')
				{
					*p = '\0';
					argv[argc++] = p+1;
				}
			}

			return Shell_Func_List[i].func(argc, argv);
		}
	}
	printf("%c: no such command\r\n", c);
	return -1;
}

static char buf[BUFFER_SIZE];
static char backspace[] = "\b \b";
static char prompt[] = "> ";


int Shell_Run()
{
	int reading = 0;
	int pos = 0;

	while (1)
	{
		Shell_Uart_Write(prompt, 2);
		reading = 1;

		while(reading)
		{
			//char c = Shell_Uart_Read();
			char c = 0;
			xQueueReceive(Shell_Queue, &c, portMAX_DELAY);
			//Shell_Uart_Write(&c,1);

			switch (c)
			{

			case '\r': //process RETURN key
				//case '\n':
				printf("\r\n");    //finish line
				buf[pos++] = 0;     //to use cprintf...
				printf(":%s\r\n", buf);
				reading = 0;        //exit read loop
				pos = 0;            //reset buffer
				break;

			case '\b': //backspace
				if (pos > 0)       //is there a char to delete ?
				{
					pos--;          //remove it in buffer
					Shell_Uart_Write(backspace, 3);
				}
				break;


			default: //other characters
				//only store characters if buffer has space
				if (pos < BUFFER_SIZE)
				{
					Shell_Uart_Write(&c, 1);
					buf[pos++] = c; //store
				}
			}
		}
		Shell_Exec(buf[0], buf);
	}
	return 0;
}
