#include <c8051F000.h>

/*
    global definitions
*/
#define OFF     0
#define ON      1
#define PRESSED 1
#define ITER    200                     // 1 for loop = 10 inst.(2.0MHz - 1ms) 

/*
    function prototypes
*/
void vPort0_Init(void);
void vWatchDog(bit);
void vDebounce(const unsigned int);

sbit LED0 = P0^0;
sbit LED1 = P0^1;
sbit LED2 = P0^2;
sbit LED3 = P0^3;
sbit SW1  = P0^4;                       // 1 = PRESSED       

/*
    main
*/
void main(void){
    // note: C8051F005 default internal oscillator frequency is 2.0 MHz

    int count=0;                          // number of times switch pressed

    vWatchDog(OFF);                     // turn-off watchdog timer
    vPort0_Init();                      // initialize port 0
    
    while(1){
        // read switch input
        if(SW1==PRESSED){
            vDebounce(200);             // wait for switch to settle down
            while(SW1==PRESSED)         // loop until switch is released
                ;
            count++;                    // increment counter                 
        }

        // update LED display
        switch(count){
            case 0:
                LED0=OFF;
                LED1=OFF;
                LED2=OFF;
                LED3=OFF;
                break;

            case 1:
                LED0=ON;
                LED1=OFF;
                LED2=OFF;
                LED3=OFF;
                break;

            case 2:
                LED0=ON;
                LED1=ON;
                LED2=OFF;
                LED3=OFF;
                break;

            case 3:
                LED0=ON;
                LED1=ON;
                LED2=ON;
                LED3=OFF;
                break;

            case 4:
                LED0=ON;
                LED1=ON;
                LED2=ON;
                LED3=ON;
                break;

            default:                    // 5 switch presses or invalid count
                count=0;                // reset press counter           
                LED0=OFF;
                LED1=OFF;
                LED2=OFF;
                LED3=OFF;

        }
    }
}

/*
    vPort0_Init: 
    configure Port0.0-3 as push-pull and Port0.4 as open-drain. Turn outputs
    P0.0-3 to low (LED0-3 OFF) initially. 
*/
void vPort0_Init(void){
    // enable crossbar (XBARE), to designate pins as output,
    // and enable weak-pullup
    XBR2=0x40;

    // config. Port0.0-3 as push-pull and Port0.4 as open-drain
    PRT0CF=0x0F; 

    // turn outputs P0.0-3 to low (LED0-3 OFF)
    P0&=0xF0;                  

}

/*
    vWatchDog: 
    Turn watchdog timer On or Off.
*/
void vWatchDog(bit status){

    if(status==ON)
        return;                         // watchdog is enabled on power-on
    
    else{
        WDTCN=0xDE;                     // disable watchdog timer
        WDTCN=0xAD;
    }
}

/*
    vDebounce:
    simple delay loop to "debounce" mechanical switch. calibrate inner loop.
*/
void vDebounce(const unsigned int ms){
    // calibrate j value.
    unsigned int i, j;

    for(i=0; i<ms; i++)
        for(j=0; j<ITER; j++)
            ;

}