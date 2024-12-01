
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

/* IRQ prototype */
//void RTC1_IRQHandler( void )
//{
    //digitalWrite( A3, HIGH );
//}

/*
void RTC2_IRQHandler( void )
{
static uint8_t flag = 0;
    if ( SMARTBOX_RTC->EVENTS_COMPARE[0] )
    {
        SMARTBOX_RTC->EVENTS_COMPARE[0] = 0;
        SMARTBOX_RTC->TASKS_CLEAR = 1;
        
        //m_time += m_rtc_increment;
        //if(cal_event_callback) cal_event_callback();
        flag = 1 - flag;
        if ( flag ) digitalWrite( A3, HIGH );
        else digitalWrite( A3, LOW );
    }
}
*/

//void RTC0_IRQHandler( void )
//{
    //digitalWrite( A3, HIGH );
//}

void LaKatRTCNRF::enableInterrupts( void )
{
    nrf_rtc_int_enable( NRF_RTC1, NRF_RTC_INT_TICK_MASK);
    return;

    //NVIC_SetPriority( SMARTBOX_RTC_IRQn, SMARTBOX_RTC_IRQ_PRI);
    //NVIC_ClearPendingIRQ( SMARTBOX_RTC_IRQn );
    //NVIC_EnableIRQ( SMARTBOX_RTC_IRQn );  

    NVIC_SetPriority( RTC1_IRQn, RTC1_IRQ_PRI);
    NVIC_ClearPendingIRQ( RTC1_IRQn );
    NVIC_EnableIRQ( RTC1_IRQn );  

    //SMARTBOX_RTC->TASKS_START = 1;
    NRF_RTC1 ->TASKS_START = 1;
}

void LaKatRTCNRF::start(void)
{
    // starting the default RTC1 timer.
    //NRF_RTC1 ->EVTENSET = RTC_EVTEN_COMPARE0_Msk | RTC_EVTEN_OVRFLW_Msk;
    //NRF_RTC1 ->INTENSET = RTC_INTENSET_OVRFLW_Msk;
    //NVIC_ClearPendingIRQ( RTC1_IRQn );
    //NVIC_EnableIRQ( RTC1_IRQn );
    //NRF_RTC1 ->TASKS_START = 1;

    //SMARTBOX_RTC->EVTENSET = RTC_EVTENSET_COMPARE0_Msk;
    //SMARTBOX_RTC->INTENSET = RTC_INTENSET_COMPARE0_Msk;
    //SMARTBOX_RTC->CC[0] = 60 * 8;

    //NRF_RTC1 ->EVTENSET = RTC_EVTENSET_TICK_Msk;
    //NRF_RTC1 ->INTENSET = RTC_INTENSET_TICK_Msk;
    //NRF_RTC1 ->CC[0] = 60 * 8;

    enableInterrupts();

    //nrf_rtc_event_clear( NRF_RTC1, NRF_RTC_EVENT_TICK );
    //nrf_rtc_event_clear( NRF_RTC1, NRF_RTC_EVENT_COMPARE_0 );
    //nrf_rtc_int_enable( NRF_RTC1, NRF_RTC_INT_TICK_MASK );

    //nrf_rtc_event_clear( SMARTBOX_RTC, NRF_RTC_EVENT_TICK );
    //nrf_rtc_event_clear( SMARTBOX_RTC, NRF_RTC_EVENT_COMPARE_0 );
    //nrf_rtc_int_enable( SMARTBOX_RTC, NRF_RTC_INT_TICK_MASK );

    delayMicroseconds( 750 );
}

void LaKatRTCNRF::disableInterrupts( void )
{

    nrf_rtc_int_disable( NRF_RTC1, NRF_RTC_INT_TICK_MASK);
    return;

    uint32_t x = nrf_rtc_int_enable_check( NRF_RTC1, 0xFFFFFFFF );
    debug_rtc( "RTC1 ints: " );
    debug_rtc_2ln( x, HEX );
    //x = nrf_rtc_int_enable_check( SMARTBOX_RTC, 0xFFFFFFFF );
    //debug_rtc( "SMARTBOX_RTC ints: " );
    //debug_rtc_2ln( x, HEX );

    //nrf_rtc_int_disable( SMARTBOX_RTC, NRF_RTC_INT_COMPARE0_MASK );
    //nrf_rtc_int_disable( SMARTBOX_RTC, NRF_RTC_INT_TICK_MASK );
    nrf_rtc_int_disable( NRF_RTC1,     NRF_RTC_INT_TICK_MASK );
    nrf_rtc_event_clear( NRF_RTC1, NRF_RTC_EVENT_TICK );
    //nrf_rtc_event_clear( SMARTBOX_RTC, NRF_RTC_EVENT_TICK );

}


/**@brief Function for stopping the RTC1 timer.
 */
void LaKatRTCNRF::stop(void)
{
    //NVIC_ClearPendingIRQ( SMARTBOX_RTC_IRQn );
    //NVIC_DisableIRQ( SMARTBOX_RTC_IRQn );
    //NVIC_ClearPendingIRQ( RTC1_IRQn );
    //NVIC_DisableIRQ( RTC1_IRQn );  

    //SMARTBOX_RTC ->EVTENCLR = -1; // clear everything
    //SMARTBOX_RTC ->INTENCLR = -1; // clear everything

    NRF_RTC1 ->EVTENCLR = -1; // clear everything
    NRF_RTC1 ->INTENCLR = -1; // clear everything

    //SMARTBOX_RTC ->TASKS_STOP = 1;
    NRF_RTC1 ->TASKS_STOP = 1;
    delayMicroseconds( MAX_RTC_TASKS_DELAY );

    //SMARTBOX_RTC ->TASKS_CLEAR = 1;
    NRF_RTC1 ->TASKS_CLEAR = 1;
    delayMicroseconds( MAX_RTC_TASKS_DELAY );

    disableInterrupts();

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
    return 0;
    //stop();
    disableInterrupts();
    SMARTBOX_RTC->PRESCALER = prescaler;
    NRF_RTC1->PRESCALER = 31;
    //start();
    enableInterrupts();

    return 0;
}

uint32_t LaKatRTCNRF::getCounter( void )
{
    //return nrf_rtc_counter_get( SMARTBOX_RTC );
    return nrf_rtc_counter_get( NRF_RTC1 );
}

uint32_t LaKatRTCNRF::getPrescaler( void )
{
    //return nrf_rtc_prescaler_get( SMARTBOX_RTC );
    return nrf_rtc_prescaler_get( NRF_RTC1 );
}

//
// This should depend on the prescaler value too.
//
int32_t LaKatRTCNRF::toSeconds( int32_t rtcValue )
{
    return rtcValue >> 10;
}


