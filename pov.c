// POV project
// Basic POV Sketch - Prints static text
// techydiy.org
// https://creativecommons.org/licenses/by-nc-sa/4.0/
// 
// Hall effect sensor is connected to ICP1 digital pin 8
// LEDS connected to port D
 
const byte inputCapturePin = 8;  //  Hall effect sensor is connected to this pin
volatile unsigned int povDisplayColumnWidthCount = 0;         // The count width of a column 
 
volatile byte timer1CountOverflows = 0;     // Timer 1 counter overflows
volatile unsigned int povDisplayColumn = 0;         // The next pov display column
 
// Data to be displayed on the pov display
// initialised with static text
// Look at the pattern of 1s sideways
//
volatile byte povDisplayData[64] ={
  B11111111,     // H
  B00011000,
  B11111111,
  B01000000,
  B01111110,     // O
  B10000001,
  B01111110,
  B00000000,
  B11101110,     // B
  B10010001,
  B11111111,
  B00000000,
  B11101110,     // B
  B10010001,
  B11111111,
  B00000000,
  B11100000,     // y
  B00011111,
  B11100000,
  B00000000,
  B10000001,     // I
  B11111111,
  B10000001,
  B00000000,
  B10010111,     // S
  B10010001,
  B11110001,
  B00000000,     
  B10000000,
  B11111111,      //T
  B10000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000
  

  };
 
                                   
void setup()
{
  noInterrupts();         // Turn off interrupts
 
  DDRD = 0b11111111;      // Set all PORTD pins to outputs
  PORTD = 0;              // Turn the leds off
 
  pinMode(inputCapturePin, INPUT_PULLUP);   //   Hall effect sensor connected to this pin
 
// ----------------------------------------------------
// TIMER 1 CONFIGURATION
//
 
  TCCR1A = 0;    // Normal wave generation mode
  TCCR1B = 0;
 
  TCNT1 = 0;          // Reset timer 1 counter
  OCR1A = 0xFFFE;     // Initialise timer 1 output compare match
  // Timer counter contol register
    //  ICNC1 - Input capture noise canceller
    //  ICES1 - Input capture edge select. 0 negative edge, 1 positive edge.
    //
    // TCCR1B = bit(ICNC1) | bit(CS12) | bit(CS10);     //   /1024 prescale
    // TCCR1B = bit(ICNC1) | bit(CS12) ;                //   /256 prescale
    // TCCR1B = bit(ICNC1) | bit(CS11) | bit(CS10);     //   /64 prescale
    //TCCR1B = bit(ICNC1) | bit(CS11);                  //   /8 prescale
     TCCR1B = bit(ICNC1) | bit(CS10);                 //     /1 prescale
 
  // Timer interrupt mask register
    // Input capture interrupt enable
    // Output compare A match interrupt enable
    // Timer overflow interrupt enable
 
  TIMSK1 = bit(ICIE1) | bit(OCIE1A)  |  bit(TOIE1);     
       
  interrupts();       // enable interrupts
   
}
 
 
void loop() {         // Empty main program loop
}
 
 
// Timer 1 - Input capture interrupt routine - digital input 8
ISR(TIMER1_CAPT_vect) {
 
  povDisplayColumnWidthCount = ( ICR1 + timer1CountOverflows * 65535 ) / 64;    // Calculate the time count for a column 
  TCNT1 = 0;                                    // Reset timer 1 counter
  povDisplayColumn=0;                           // Reset display to the first column
  OCR1A  = povDisplayColumnWidthCount-1;        // Set timer 1 OCR1A interrupt count duration        
  timer1CountOverflows = 0;                     // Reset timer1 overflows
     
}
 
// Timer 1 overflow interrupt routine
ISR(TIMER1_OVF_vect) {
  timer1CountOverflows++;      // Increment the timer1 overflows counter
}
 
 
ISR(TIMER1_COMPA_vect){ // Timer 1 Output compare A match interrupt routine
  OCR1A  = TCNT1 + povDisplayColumnWidthCount-1;       // Update timer1 with the next time interval
 
  if (povDisplayColumn < 64){
    PORTD = povDisplayData[povDisplayColumn];             // Output data to the leds
  } else { 
    PORTD = 0;                                            // Turn off the leds  
  }
 
  povDisplayColumn++;                        // increment the display column
}