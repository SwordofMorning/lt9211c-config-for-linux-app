#ifndef _INCLUDE_H
#define _INCLUDE_H
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include "type.h"

#include "misc.h"

//App
#include "config.h"

//Ocm
#include "OcmDelay.h"
#include "OcmUart0.h"
//Driver
#include "DrvI2cMaster.h"
#include "DrvCsc.h"
#include "DrvGpio.h"
#include "DrvMipiRx.h"
#include "DrvMipiTx.h"
#include "DrvLvdsRx.h"
#include "DrvLvdsTx.h"
#include "DrvTtlRx.h"
#include "DrvTtlTx.h"
#include "DrvDcsCmd.h"
#include "DrvSystem.h"
#include "DrvMipiRpt.h"
#include "DrvMipiLs.h"

//Module
#include "ModTtlRx.h"
#include "ModTtlTx.h"
#include "ModLvdsTx.h"
#include "ModLvdsRx.h"
#include "ModMipiTx.h"
#include "ModMipiRx.h"
#include "ModSystem.h"
#include "ModPattern.h"
#include "ModMipiRpt.h"
#include "ModMipiLs.h"


#define RDATA
#endif

