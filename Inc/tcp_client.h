// Reference: https://github.com/54zorb/stm32-lwip/tree/master/Src
#ifndef __tcp_client_H
#define __tcp_client_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "main.h"
#include "stm32f7xx_hal.h"
#include "lwip/tcp.h"


// define server IP
#define SERVER_IP1 169
#define SERVER_IP2 254
#define SERVER_IP3 212
#define SERVER_IP4 187

// define port number
#define TCP_REMOTE_PORT 8881 // remote port
#define TCP_LOCAL_PORT 8880 // local port

struct tcp_pcb* tcp_client_init(void);

#ifdef __cplusplus
}
#endif
#endif

