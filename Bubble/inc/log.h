#ifndef __LOG_H__
#define __LOG_H__

#include <unistd.h>
#include <dlog.h>

#undef LOG_TAG
#define LOG_TAG "BLUETOOTHCHAT"

#if !defined(_D)
#define _D(fmt, arg...) dlog_print(DLOG_DEBUG, LOG_TAG, fmt"\n", ##arg)
#endif

#if !defined(_I)
#define _I(fmt, arg...) dlog_print(DLOG_INFO, LOG_TAG, fmt"\n", ##arg)
#endif

#if !defined(_W)
#define _W(fmt, arg...) dlog_print(DLOG_WARN, LOG_TAG, fmt"\n", ##arg)
#endif

#if !defined(_E)
#define _E(fmt, arg...) dlog_print(DLOG_ERROR, LOG_TAG, fmt"\n", ##arg)
#endif

#define retvm_if(expr, val, fmt, arg...) do { \
	if(expr) { \
		_E(fmt, ##arg); \
		_E("(%s) -> %s() return", #expr, __FUNCTION__); \
		return val; \
	} \
} while (0)

#define retv_if(expr, val) do { \
	if(expr) { \
		_E("(%s) -> %s() return", #expr, __FUNCTION__); \
		return (val); \
	} \
} while (0)

#define retm_if(expr, fmt, arg...) do { \
	if(expr) { \
		_E(fmt, ##arg); \
		_E("(%s) -> %s() return", #expr, __FUNCTION__); \
		return; \
	} \
} while (0)

#define ret_if(expr) do { \
	if(expr) { \
		_E("(%s) -> %s() return", #expr, __FUNCTION__); \
		return; \
	} \
} while (0)

#define goto_if(expr, val) do { \
	if(expr) { \
		_E("(%s) -> goto", #expr); \
		goto val; \
	} \
} while (0)

#define break_if(expr) { \
	if(expr) { \
		_E("(%s) -> break", #expr); \
		break; \
	} \
}

#define continue_if(expr) { \
	if(expr) { \
		_E("(%s) -> continue", #expr); \
		continue; \
	} \
}

#endif				/* __LOG_H__ */
