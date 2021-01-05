#ifndef _PORT_H_
#define _PORT_H_
#include "mbport.h"
#include "usart.h"
#include "FreeRTOS.h"
#include "Timers.h"
#include "cmsis_os.h"
#define  assert(x) Dummy(x)
extern void  Dummy(bool x);

#define ENTER_CRITICAL_SECTION() portENTER_CRITICAL()
#define EXIT_CRITICAL_SECTION() portEXIT_CRITICAL()
#endif