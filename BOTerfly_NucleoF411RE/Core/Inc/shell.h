/*
 * shell.h
 *
 *  Created on: 7 juin 2019
 *      Author: laurent
 */

#ifndef INC_LIB_SHELL_SHELL_H_
#define INC_LIB_SHELL_SHELL_H_

#include <stdint.h>
#include "main.h"
#include "cmsis_os.h"

#define UART_DEVICE huart2

#define _SHELL_FUNC_LIST_MAX_SIZE 64

char Shell_Uart_Read();
int  Shell_Uart_Write(char * s, uint16_t size);
void Shell_Uart_Data_Ready();
void Shell_Char_Received();
void Shell_Init();
int  Shell_Add(char c, int (* pfunc)(int argc, char ** argv), char * description);
int  Shell_Exec(char c, char * buf);
int  Shell_Run();

extern QueueHandle_t Shell_Queue;

#endif /* INC_LIB_SHELL_SHELL_H_ */
