/*
 * ibcom-cv.h
 *
 *  Created on: Sep 12, 2009
 *      Author: rob
 */

#ifndef IBCOMCV_H_
#define IBCOMCV_H_
#include "rocs/public/rocs.h"

int makeIBComCVPacket(int cv, int value, byte* buffer, Boolean write);
int startIBComPT(byte* buffer);
int stopIBComPT(byte* buffer);

#endif /* IBCOMCV_H_ */
