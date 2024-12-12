#ifndef _PORTS_H_
#define _PORTS_H_

#include "board.h"

#ifdef PORT_A
#define PORT_A_CNT 1
#else
#define PORT_A_CNT 0
#endif

#ifdef PORT_B
#define PORT_B_CNT 1
#else
#define PORT_B_CNT 0
#endif

#if defined (PORT_C) && !defined (TIVA)
#define PORT_C_CNT 1
#else
#define PORT_C_CNT 0
#endif

#ifdef PORT_D
#define PORT_D_CNT 1
#else
#define PORT_D_CNT 0
#endif

#ifdef PORT_E
#define PORT_E_CNT 1
#else
#define PORT_E_CNT 0
#endif

#ifdef PORT_F
#define PORT_F_CNT 1
#else
#define PORT_F_CNT 0
#endif

#ifdef PORT_G
#define PORT_G_CNT 1
#else
#define PORT_G_CNT 0
#endif

#ifdef PORT_H
#define PORT_H_CNT 1
#else
#define PORT_H_CNT 0
#endif

#ifdef PORT_I
#define PORT_I_CNT 1
#else
#define PORT_I_CNT 0
#endif

#ifdef PORT_J
#define PORT_J_CNT 1
#else
#define PORT_J_CNT 0
#endif

#ifdef PORT_K
#define PORT_K_CNT 1
#else
#define PORT_K_CNT 0
#endif

#ifdef PORT_L
#define PORT_L_CNT 1
#else
#define PORT_L_CNT 0
#endif

#ifdef PORT_M
#define PORT_M_CNT 1
#else
#define PORT_M_CNT 0
#endif

#ifdef PORT_N
#define PORT_N_CNT 1
#else
#define PORT_N_CNT 0
#endif

#ifdef PORT_O
#define PORT_O_CNT 1
#else
#define PORT_O_CNT 0
#endif

#ifdef PORT_P
#define PORT_P_CNT 1
#else
#define PORT_P_CNT 0
#endif

#ifdef PORT_Q
#define PORT_Q_CNT 1
#else
#define PORT_Q_CNT 0
#endif

#ifdef PORT_R
#define PORT_R_CNT 1
#else
#define PORT_R_CNT 0
#endif

#ifdef PORT_S
#define PORT_S_CNT 1
#else
#define PORT_S_CNT 0
#endif

#ifdef PORT_T
#define PORT_T_CNT 1
#else
#define PORT_T_CNT 0
#endif

#define PORT_NUM (PORT_A_CNT+PORT_B_CNT+PORT_C_CNT+PORT_D_CNT+PORT_E_CNT+PORT_F_CNT+PORT_G_CNT+ \
                  PORT_H_CNT+PORT_I_CNT+PORT_J_CNT+PORT_K_CNT+PORT_L_CNT+PORT_M_CNT+PORT_N_CNT+ \
                  PORT_O_CNT+PORT_P_CNT+PORT_Q_CNT+PORT_R_CNT+PORT_S_CNT+PORT_T_CNT)

int PORT_ARRAY[PORT_NUM] = {
    #ifdef PORT_A
    PORT_A,
    #endif
    #ifdef PORT_B
    PORT_B,
    #endif
    #if defined (PORT_C) && !defined (TIVA)  // PORTC contains programming pins for TIVA MCU's
    PORT_C,
    #endif
    #ifdef PORT_D
    PORT_D,
    #endif
    #ifdef PORT_E
    PORT_E,
    #endif
    #ifdef PORT_F
    PORT_F,
    #endif
    #ifdef PORT_G
    PORT_G,
    #endif
    #ifdef PORT_H
    PORT_H,
    #endif
    #ifdef PORT_I
    PORT_I,
    #endif
    #ifdef PORT_J
    PORT_J,
    #endif
    #ifdef PORT_K
    PORT_K,
    #endif
    #ifdef PORT_L
    PORT_L,
    #endif
    #ifdef PORT_M
    PORT_M,
    #endif
    #ifdef PORT_N
    PORT_N,
    #endif
    #ifdef PORT_O
    PORT_O,
    #endif
    #ifdef PORT_P
    PORT_P,
    #endif
    #ifdef PORT_Q
    PORT_Q,
    #endif
    #ifdef PORT_R
    PORT_R,
    #endif
    #ifdef PORT_S
    PORT_S,
    #endif
    #ifdef PORT_T
    PORT_T
    #endif
};

#endif // _PORTS_H_
