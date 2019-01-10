#ifndef __LIM_H
#define __LIM_H

typedef unsigned int uint;
typedef unsigned char uchar;
typedef unsigned long ulong;
typedef unsigned char ubool;

#ifndef cast
#define cast(t, exp)	((t)(exp))
#endif

/**
 * String memory growing step
 */
#define STR_GROW_STEP	20

#endif
