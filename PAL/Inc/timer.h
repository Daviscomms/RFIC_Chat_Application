#ifndef _TIMER_H_
#define _TIMER_H_

#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "pal_timer.h"
#include "imx6sxtypes.h"

typedef void (*timer_cb_t)(timer_instance_id_t timer_instance_id);
typedef void* timer_argv_t;
typedef uint32_t timer_tick_ms_t;

typedef enum {
    TIMER_TYPE_ONESHORT = 0u,
    TIMER_TYPE_PERIODIC,
    TIMER_TYPE_MAX
} timer_type_t;

typedef struct _timer_handle_t {
    timer_t timerid; ///< timer id
    timer_type_t type; ///< timer options
} timer_handle_t;

timer_handle_t* fn_timer_create(timer_type_t type, FUNC_PTR(timer_cb), timer_instance_id_t timer_instance_id);

void fn_timer_start(timer_handle_t* timer_handle, timer_tick_ms_t tick);

void fn_timer_stop(timer_handle_t* timer_handle);

void fn_timer_delete(timer_handle_t* timer_handle);

#endif /* _TIMER_H_ */