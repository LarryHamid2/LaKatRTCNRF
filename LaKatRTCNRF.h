
#ifndef LAKAT_RTC_NRF_H
#define LAKAT_RTC_NRF_H

//
// Class for implementing a Real Time Counter based on the NRF52840
// It uses RTC1
//
//

#include <Arduino.h>
#include <nrf_rtc.h>

#define RTC1_IRQ_PRI            3      /**< Priority of the RTC1 interrupt (used for checking for timeouts and executing timeout handlers). */
#define RTC2_IRQ_PRI            3      /**< Priority of the RTC1 interrupt (used for checking for timeouts and executing timeout handlers). */
#define MAX_RTC_TASKS_DELAY     47     /**< Maximum delay until an RTC task is executed. */

#define SMARTBOX_RTC            NRF_RTC2
#define SMARTBOX_RTC_IRQ_PRI    RTC2_IRQ_PRI
#define SMARTBOX_RTC_IRQn       RTC2_IRQn
//#define SMARTBOX_RTC        NRF_RTC0
//#define SMARTBOX_IRQ_PRI    RTC0_IRQ_PRI
//#define SMARTBOX_IRQn       RTC0_IRQn

class LaKatRTCNRF
{
private:

public:

  	LaKatRTCNRF( void );
    int begin( uint32_t prescaler ); 
    uint32_t getCounter( int rtcNum );
    uint32_t getPrescaler( int rtcNum );
    int32_t toSeconds( int32_t rtcValue );
    void enableInterrupts( void );
    void disableInterrupts( void );

protected:
};

#endif
