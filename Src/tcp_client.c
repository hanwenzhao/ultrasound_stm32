// Reference: https://github.com/54zorb/stm32-lwip/tree/master/Src

#include "tcp_client.h"


static err_t tcp_client_connected(void *arg, struct tcp_pcb *tpcb, err_t err);
static err_t tcp_client_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);

// call back function for connect server
static err_t tcp_client_connected(void *arg, struct tcp_pcb *tpcb, err_t err)
{
    printf("tcp client connected\r\n");

    tcp_write(tpcb, "tcp client connected", strlen("tcp client connected"), 0);

    // call back function for receiving
    tcp_recv(tpcb, tcp_client_recv);

    return ERR_OK;
}

struct tcp_pcb* tcp_client_init(void)
{
    struct tcp_pcb *tpcb;
    ip_addr_t serverIp;

    // set server ip
    IP4_ADDR(&serverIp, SERVER_IP1, SERVER_IP2, SERVER_IP3, SERVER_IP4);

    // create tcp control module
    tpcb = tcp_new();

    if (tpcb != NULL)
    {
        err_t err;

        // bind local ip and port number
        err = tcp_bind(tpcb, IP_ADDR_ANY, TCP_LOCAL_PORT);

        if (err == ERR_OK)
        {
            // connect to server
            tcp_connect(tpcb, &serverIp, TCP_REMOTE_PORT, tcp_client_connected);
        }
        else
        {
            memp_free(MEMP_TCP_PCB, tpcb);

            printf("can not bind pcb\r\n");
        }
    }
    return tpcb;
}

static err_t tcp_client_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
{
    uint32_t i;

    // send data back
    tcp_write(tpcb, p->payload, p->len, 1);

    if (p != NULL)
    {
        struct pbuf *ptmp = p;

        // print received data
        printf("get msg from %d:%d:%d:%d port:%d:\r\n",
            *((uint8_t *)&tpcb->remote_ip.addr),
            *((uint8_t *)&tpcb->remote_ip.addr + 1),
            *((uint8_t *)&tpcb->remote_ip.addr + 2),
            *((uint8_t *)&tpcb->remote_ip.addr + 3),
            tpcb->remote_port);

        while(ptmp != NULL)
        {
            for (i = 0; i < p->len; i++)
            {
                printf("%c", *((char *)p->payload + i));
            }

            ptmp = p->next;
        }

        printf("\r\n");

        tcp_recved(tpcb, p->tot_len);

        pbuf_free(p);
    }
    else if (err == ERR_OK)
    {
        printf("tcp client closed\r\n");

        tcp_recved(tpcb, p->tot_len);

        return tcp_close(tpcb);
    }

    return ERR_OK;
}
