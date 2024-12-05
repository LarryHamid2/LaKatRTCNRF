
//
// Implementation of the RTC
//

#include "LaKatRTCNRF.h"

#define DEBUG_RTC   1

#if DEBUG_RTC == 1
#define debug_rtc(x)    Serial.print(x)
#define debug_rtc_ln(x)  Serial.println(x)
#define debug_rtc_2(x,y)   Serial.print(x,y)
#define debug_rtc_2ln(x,y) Serial.println(x,y)
#else
#define debug_rtc(x)
#define debug_rtc_ln(x)
#define debug_rtc_2(x,y)
#define debug_rtc_2ln(x,y)
#endif


// NOTE: It is EXTREMELY important to use extern "C", otherwise things hang. 
extern "C" void RTC2_IRQHandler(void)
{
    // Clear all events
    nrf_rtc_event_clear( SMARTBOX_RTC, NRF_RTC_EVENT_COMPARE_0 );
    nrf_rtc_event_clear( SMARTBOX_RTC, NRF_RTC_EVENT_TICK );
}


//void RTC0_IRQHandler( void )
//{
    //digitalWrite( A3, HIGH );
//}

void LaKatRTCNRF::enableInterrupts( void )
{
    // RTC1
    nrf_rtc_int_enable( NRF_RTC1, NRF_RTC_INT_TICK_MASK);

    // SMARTBOX
    nrf_rtc_event_clear( SMARTBOX_RTC, NRF_RTC_EVENT_TICK );
    nrf_rtc_int_enable( SMARTBOX_RTC, NRF_RTC_INT_TICK_MASK);

    //NRF_RTC1->EVTENSET = RTC_EVTEN_COMPARE0_Msk | RTC_EVTEN_OVRFLW_Msk;
    //NRF_RTC1->INTENSET = RTC_INTENSET_COMPARE0_Msk;

    delayMicroseconds( 750 );

    return;
}

void LaKatRTCNRF::disableInterrupts( void )
{
    nrf_rtc_int_disable( NRF_RTC1, NRF_RTC_INT_TICK_MASK);
    nrf_rtc_int_disable( SMARTBOX_RTC, NRF_RTC_INT_TICK_MASK);

    return;
}


LaKatRTCNRF::LaKatRTCNRF( void )
{
}

/**@brief Function for initializing the RTC0 counter.
 *
 * @param[in] prescaler   Value of the RTC0 PRESCALER register. Set to 0 for no prescaling.
 */
int LaKatRTCNRF::begin( uint32_t prescaler )
{
    disableInterrupts();
    SMARTBOX_RTC ->PRESCALER = prescaler;
    enableInterrupts();

    nrf_rtc_task_trigger( SMARTBOX_RTC, NRF_RTC_TASK_CLEAR );
    nrf_rtc_task_trigger( SMARTBOX_RTC, NRF_RTC_TASK_START );  // This causes a hang

    NVIC_ClearPendingIRQ( SMARTBOX_RTC_IRQn );
    NVIC_SetPriority( SMARTBOX_RTC_IRQn, SMARTBOX_RTC_IRQ_PRI );
    NVIC_EnableIRQ( SMARTBOX_RTC_IRQn );

    return 0;
}

uint32_t LaKatRTCNRF::getCounter( int rtcNum )
{
    //return nrf_rtc_counter_get( SMARTBOX_RTC );
    if ( 0 == rtcNum ) return nrf_rtc_counter_get( NRF_RTC1 );
    return nrf_rtc_counter_get( SMARTBOX_RTC );
}

uint32_t LaKatRTCNRF::getPrescaler( int rtcNum )
{
    //return nrf_rtc_prescaler_get( SMARTBOX_RTC );
    if ( 0 == rtcNum ) return nrf_rtc_prescaler_get( NRF_RTC1 );
    return nrf_rtc_prescaler_get( SMARTBOX_RTC );
}

//
// This should depend on the prescaler value too.
//
int32_t LaKatRTCNRF::toSeconds( int32_t rtcValue )
{
    // Assuming an 8Hz counter
    return rtcValue >> 3;
}


