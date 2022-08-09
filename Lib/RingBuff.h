#ifndef _RING_BUFF_H
#define _RING_BUFF_H

#define RING_BUFF_SIZE  1024

typedef struct
{
    volatile unsigned int pW;
    volatile unsigned int pR;
    unsigned char buffer[RING_BUFF_SIZE];
} RingBuff_t;

extern void RingBuff_Init(RingBuff_t *dstBuf);
extern void RingBuff_Write(unsigned char c, RingBuff_t *dstBuf);
extern int RingBuff_Read(unsigned char *c, RingBuff_t *dstBuf);

#endif
