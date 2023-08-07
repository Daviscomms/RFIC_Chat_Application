#ifndef PAL_CONFIG_H
#define PAL_CONFIG_H
#include <unistd.h>
#include <pthread.h>

static pthread_mutex_t critical_mutex = PTHREAD_MUTEX_INITIALIZER;
static bool interrupt_enable = false;

/*
 * This macro is used to mark the start of a critical region. It saves the
 * current status register and then disables the interrupt.
 */
#define ENTER_CRITICAL_REGION() \
    do { \
        pthread_mutex_lock(&critical_mutex); \
    } while(0)
/*
 * This macro is used to mark the end of a critical region. It restores the
 * current status register.
 */
#define LEAVE_CRITICAL_REGION() \
    do { \
        pthread_mutex_unlock(&critical_mutex); \
    } while(0)

/*
 * This macro saves the trx interrupt status and disables the trx interrupt.
 */
#define ENTER_TRX_REGION() \
    do { \
        interrupt_enable = true; \
        pal_disable_interrupt();\
    } while (0)

/*
 *  This macro restores the transceiver interrupt status
 */
#define LEAVE_TRX_REGION() \
    do { \
        if (interrupt_enable) \
        { \
            interrupt_enable = false; \
            pal_enable_interrupt(); \
        } \
    } while (0)

/*
 * This board uses an SPI-attached transceiver.
 */
#define PAL_USE_SPI_TRX                 (1)


/**
 * Multiple transceivers are supported
 */
#define MULTI_TRX_SUPPORT


/**
 * Attached transceiver uses SPI block mode
 
 */
#define PAL_SPI_BLOCK_MODE


#define PAL_WAIT_1_US()					usleep(1)

#endif

