#ifndef _DEBUG_H
#define _DEBUG_H

#define _DEBUG_CALLS

#ifdef _DEBUG

struct _FUNC_STUB {
		const char * funcname;
		struct _FUNC_STUB* previous;
};

#define CALLER_ARG0 struct _FUNC_STUB* _caller
#define CALLER_ARG CALLER_ARG0, 
#define CALL_ARG0 &_this_func
#define CALL_ARG &_this_func,

#define STUB_COMMON _this_func.previous=_caller;

#define STUB_CALLS \
	struct _FUNC_STUB* _func=&_this_func;\
	STUB_COMMON \
	while (_func) \
	{ \
		printf("%s", _func->funcname); \
		if (_func->previous) printf("<"); \
		_func=_func->previous; \
	}\
	printf("\n");

#ifdef _DEBUG_CALLS
	#define STUB STUB_CALLS
#else
	#define STUB STUB_COMMON
#endif

#else
#define CALLER_ARG0
#define CALLER_ARG
#define CALL_ARG0
#define CALL_ARG
#define STUB
#endif

#define DEFUN(fn, arglist) fn arglist \
{ \
	static struct _FUNC_STUB _this_func={#fn};

#endif
