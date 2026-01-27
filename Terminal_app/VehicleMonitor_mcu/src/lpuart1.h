#ifndef LPUART1_INC_LPUART1_H_
#define LPUART1_INC_LPUART1_H_
/*******************************************************************************
* Include
*******************************************************************************/
#include "S32K144.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
/*******************************************************************************
* define
*******************************************************************************/
#define S32_NVIC_ISER_COUNT                      8u
#define S32_NVIC_ICER_COUNT                      8u
#define S32_NVIC_ISPR_COUNT                      8u
#define S32_NVIC_ICPR_COUNT                      8u
#define S32_NVIC_IABR_COUNT                      8u
#define S32_NVIC_IP_COUNT                        240u

typedef struct {
	   volatile uint32_t ISER[S32_NVIC_ISER_COUNT];         /*offset: 0x0*/
       uint8_t RESERVED_0[96];
       volatile uint32_t ICER[S32_NVIC_ICER_COUNT];         /*offset: 0x80*/
       uint8_t RESERVED_1[96];
       volatile uint32_t ISPR[S32_NVIC_ISPR_COUNT];         /*offset: 0x100*/
       uint8_t RESERVED_2[96];
       volatile uint32_t ICPR[S32_NVIC_ICPR_COUNT];         /*offset: 0x180*/
       uint8_t RESERVED_3[96];
       volatile uint32_t IABR[S32_NVIC_IABR_COUNT];         /*offset: 0x200*/
       uint8_t RESERVED_4[224];
       volatile uint8_t IP[S32_NVIC_IP_COUNT];              /*offset: 0x300*/
       uint8_t RESERVED_5[2576];
       volatile  uint32_t STIR;                             /*offset: 0xE00*/
} S32_NVIC_Type;
#define S32_NVIC_BASE                            (0xE000E100u)
#define S32_NVIC                                 ((S32_NVIC_Type *)S32_NVIC_BASE)

typedef void (*callback)(char _argument);
/*******************************************************************************
* API
*******************************************************************************/
void init_uart(const uint32_t _baud_rate);

void send_character(char character);

void register_take_data_from_uart(callback _callback_func);

void LPUART1_send_string(const char *data_string);

void LPUART1_print_number(uint32_t value);

void LPUART1_send_string_v2(char *data_string, uint16_t length);

#endif /* LPUART1_INC_LPUART1_H_ */
/*******************************************************************************
* EOF
*******************************************************************************/
