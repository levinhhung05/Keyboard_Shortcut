#define ENCODER_DO_NOT_USE_INTERRUPTS
#include <avr/io.h>
#include <avr/interrupt.h>
#include "lib/Encoder/Encoder.h"
//-----------------------------------------------------------------------------------------------

void IO_Init(){
  //SET PORTB AS INPUT  FOR KEYPAD  3*2
  DDRB  |= (1<<PB0) | (1<<PB1) | (1<<PB2) | (1<<PB3);         //PB0:3 = OUTPUT
  DDRC  &=  ~((1<<PC0) | (1<<PC1) | (1<<PC2) | (1<<PC3));     //PC0:3 = INPUT
  PORTC |= (1<<PC0) | (1<<PC1) | (1<<PC2) | (1<<PC3);
  //SET PORTD AS INPUT FOR ROTATE ENCODER 
  //PD2: INT0 FOR CLK ENCODER 1
  //PD3: INT1 FOR CLK ENCODER 2
  //PD4:PD5 FOR DW ENCODER 1:2
  //PD6:PD7 FOR BUTTON ENCODER  1:2
  DDRD &= ~( (1<<PD2)|(1<<PD3)|(1<<PD4)|(1<<PD5)|(1<<PD6)|(1<<PD7) );
  PORTD = 0XFF;
}
//-------------------------------------------------------------
uint8_t last_key_state = 0; 
uint8_t key_state = 0; 

int8_t Keypad_Scan() {
    // COL = PB0..PB3 (output)
    // ROW = PC0..PC3 (input + pullup)

    // Set all COL pins HIGH
    PORTB |= 0x0F;  // PB0..PB3 = 1

    for (uint8_t col = 0; col < 4; col++) {

        // Pull COL down LOW
        PORTB &= ~(1 << col);
        _delay_us(5);

        uint8_t rowPins = PINC & 0x0F; // đọc PC0..PC3

        if (!(rowPins & (1 << PC0))) return col +  0;  // row 0
        if (!(rowPins & (1 << PC1))) return col +  4;  // row 1
        if (!(rowPins & (1 << PC2))) return col +  8;  // row 2
        if (!(rowPins & (1 << PC3))) return col + 12;  // row 3

        // Pull up
        PORTB |= (1 << col);
    }

    return -1;
}
//-------------------------------------------------------------
void UART_Init() {
    // Baudrate = 9600
    UBRR0H = 0;
    UBRR0L = 103;

    // Active receive and transmit
    UCSR0B = (1 << TXEN0);

    // 8 bit data, 1 bit stop
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void UART_Trans(uint8_t data) {
    while (!(UCSR0A & (1 << UDRE0)));
    // Transmit the character over UART
    UDR0 = data;
}

//-------------------------------------------------------------
Encoder upDown(2,4);
Encoder leftRight(3,5);
long posUpDown = 0; //Position counter for up/down encoder
long posLeftRight = 0; //Position counter for left/right encoder
long posUpDownPrev = 0; //Previous position of up/down encoder
long posLeftRightPrev = 0; //Previous position of left/right encoder

int counterUpDown = 0;
int counterLeftRight = 0;

//-------------------------------------------------------------
int main() {
    int8_t key;
    int8_t last_key = -1;
    uint32_t key_press_time = 0;

    IO_Init();
    UART_Init();
    sei();

    while (1) {
        
		key = Keypad_Scan();

        if (key != -1) { // Nếu có phím được nhấn
            if (last_key == -1) { // Phím mới vừa được nhấn lần đầu
                last_key = key;
                key_press_time = 0;

            } else if (key == last_key) { // Phím đang được giữ
                key_press_time++;

                if (key_press_time >= 500) { // Giữ 1 giây
                    key_press_time = 0;

                    // ----- MAPPING OUTPUT -----
                    if (key >= 0 && key <= 9) {
                        UART_Trans('0' + key);  // 0–9
                    }
                    else if (key >= 10 && key <= 15) {
                        UART_Trans('A' + (key - 10)); // 10→A ... 15→F
                    }
                    UART_Trans('\n');
                    // ---------------------------
                }

            } else { // Phím khác được nhấn
                last_key = key;
                key_press_time = 0;
            }

        } else { // Không có phím nào
            last_key = -1;
        }

        _delay_ms(1);
        

  // ENCODERS
  posUpDown = upDown.read(); //Up/down encoder
  if (posUpDown!=posUpDownPrev) //If new position doesn't equal previous position
  {
    if ((posUpDown-posUpDownPrev)>0)
    {
      counterUpDown = counterUpDown+1;
      if (counterUpDown>3) // 3 counts per physical encoder "click"
      {
        UART_Trans('L');
        UART_Trans('\n');
        counterUpDown = 0;
      }
    }
    else if ((posUpDown-posUpDownPrev)<0)
    {
      counterUpDown = counterUpDown-1;
      if (counterUpDown<-3)
      {
        UART_Trans('X');
        UART_Trans('\n');
        counterUpDown = 0;
      }
    }
  }
  posUpDownPrev = posUpDown;

  posLeftRight = leftRight.read(); // Left/right encoder
  if (posLeftRight!=posLeftRightPrev) //If new position doesn't equal previous position
  {
    if ((posLeftRight-posLeftRightPrev)>0)
    {
      counterLeftRight = counterLeftRight+1;
      if (counterLeftRight>3) // 3 counts per physical encoder "click"
      {
        UART_Trans('P');
        UART_Trans('\n');
        counterLeftRight = 0;
      }
    }
    else if ((posLeftRight-posLeftRightPrev)<0)
    {
      counterLeftRight = counterLeftRight-1;
      if (counterLeftRight<-3)
      {
        UART_Trans('T');
        UART_Trans('\n');
        counterLeftRight = 0;
      }
    }
  }
  posLeftRightPrev = posLeftRight;
  }
  return 0;
}
