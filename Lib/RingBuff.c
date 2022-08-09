#include "RingBuff.h"

void RingBuff_Init(RingBuff_t *dstBuf)
{
    dstBuf->pW = 0;
    dstBuf->pR = 0;
}

void RingBuff_Write(unsigned char c, RingBuff_t *dstBuf)
{
    unsigned char i = 0;

    i = (dstBuf->pW + 1) % RING_BUFF_SIZE;
    if (i != dstBuf->pR)
    {
        dstBuf->buffer[dstBuf->pW] = c;
        dstBuf->pW = i;
    }
}

int RingBuff_Read(unsigned char *c, RingBuff_t *dstBuf)
{
    if (dstBuf->pR == dstBuf->pW)
    {
        return -1;
    }
    else
    {
        *c = dstBuf->buffer[dstBuf->pR];
        dstBuf->pR = (dstBuf->pR + 1) % RING_BUFF_SIZE;
        return 0;
    }
}
