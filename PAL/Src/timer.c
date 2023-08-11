#include "timer.h"

timer_handle_t* fn_timer_create(timer_type_t type, FUNC_PTR(timer_cb), timer_instance_id_t timer_instance_id)
{
    timer_handle_t* p_timer
        = (timer_handle_t*)calloc(sizeof(timer_handle_t), sizeof(uint8_t));

    if (NULL == p_timer) {
        printf("Timer creation failed, insufficient memory\r\n");
        return NULL;
    }

    p_timer->type = type;

    struct sigevent evp = { .sigev_notify = SIGEV_THREAD,
                            //.sigev_value.sival_ptr = (void*)p_timer,
                            .sigev_value.sival_int = timer_instance_id,
                            //.sigev_notify_function = __timer_handle };
                            .sigev_notify_function = timer_cb };

    if (-1 == timer_create(CLOCK_REALTIME, &evp, &(p_timer->timerid))) {
        printf("Timer create failed!!!\r\n");
        free(p_timer);
        p_timer = NULL;
        return NULL;
    }

    return p_timer;
}

void fn_timer_start(timer_handle_t* p_timer, timer_tick_ms_t tick)
{
    struct itimerspec it;

    if (TIMER_TYPE_PERIODIC == p_timer->type) {
        it.it_interval.tv_sec = tick / 1000000;
        it.it_interval.tv_nsec = tick * 1000 - it.it_interval.tv_sec * 1000000;
    } else {
        it.it_interval.tv_sec = 0x00;
        it.it_interval.tv_nsec = 0x00;
    }

    it.it_value.tv_sec = tick / 1000000;
    it.it_value.tv_nsec = tick * 1000 - it.it_value.tv_sec * 1000000;

    if (-1 == timer_settime(p_timer->timerid, 0, &it, NULL)) {
        printf("Timer start failed\r\n");
        free(p_timer);
        p_timer = NULL;
    }
}

void fn_timer_stop(timer_handle_t* p_timer)
{
    struct itimerspec it;

    it.it_interval.tv_sec = 0x00;
    it.it_interval.tv_nsec = 0x00;

    it.it_value.tv_sec = 0x00;
    it.it_value.tv_nsec = 0x00;

    if (-1 == timer_settime(p_timer->timerid, 0, &it, NULL)) {
        printf("Timer stop failed\r\n");
        free(p_timer);
        p_timer = NULL;
    }
}

void fn_timer_delete(timer_handle_t* p_timer)
{
    if (p_timer == NULL)
    {
        printf("cannot delete due to NULL.\n");
        return;
    }

    if (-1 == timer_delete(p_timer->timerid))
        printf("Timer delete failed\r\n");

    free(p_timer);
    p_timer = NULL;
}