/*
 * Log.h
 *
 *  Created on: 13 Feb 2015
 *      Author: Charlie
 *
 *	Include this in the source files only (never from a header).
 *	You can specify the LOGLEVEL before including this file, to
 *	suppress logging information from code that has been tested.
 *	By default, all logging information will be sent.
 *
 *	I haven't tested this macro witchcraft, but it should work...
 *
 */

#ifndef LOG_H_
#define LOG_H_

#include <iostream>

#define LL_TRACE 5
#define LL_DEBUG 4
#define LL_INFO 3
#define LL_WARN 2
#define LL_ERR 1
#define LL_NONE 0


//Print all debugging messages, if not specified in the source file.
#ifndef LOGLEVEL
#define LOGLEVEL LL_DEBUG
#endif


#if (LOGLEVEL >= LL_TRACE)
#define TRACE(x) std::cout << x << std::endl;
#else
#define TRACE(x)
#endif

#if (LOGLEVEL >= LL_DEBUG)
#define DEBUG(x) std::cout << x << std::endl;
#else
#define DEBUG(x)
#endif

#if (LOGLEVEL >= LL_INFO)
#define INFO(x) std::cout << x << std::endl;
#else
#define INFO(x)
#endif

#if (LOGLEVEL >= LL_WARN)
#define WARN(x) std::cerr << x << std::endl;
#else
#define WARN(x)
#endif

#if (LOGLEVEL >= LL_ERR)
#define ERR(x) std::cerr << x << std::endl;
#else
#define ERR(x)
#endif



#endif /* LOG_H_ */
