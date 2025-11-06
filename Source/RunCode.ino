#define ENCODER_DO_NOT_USE_INTERRUPTS
#include <avr/io.h>
#include <avr/interrupt.h>
#include <Encoder.h>
//-----------------------------------------------------------------------------------------------

void IO_Init(){
  //SET PORTB AS INPUT  FOR KEYPAD  3*2
  DDRB  |= (1<<PB0)|(1<<PB1)|(1<<PB2); //PB0:2 = OUTPUT
  DDRB  &=  ~( (1<<PB3)|(1<<PB4) );    //PB3:4 = INPUT
  PORTB = 0XFF;
  //SET PORTD AS INPUT FOR ROTATE ENCODER 
  //PD2: INT0 FOR CLK ENCODER 1
  //PD3: INT1 FOR CLK ENCODER 2
  //PD4:PD5 FOR DW ENCODER 1:2
  //PD6:PD7 FOR BUTTON ENCODER  1:2
  DDRD &= ~( (1<<PD2)|(1<<PD3)|(1<<PD4)|(1<<PD5)|(1<<PD6)|(1<<PD7) );
  PORTD = 0XFF;
}
//-------------------------------------------------------------
uint8_t last_key_state = 0; // Trạng thái cuối cùng của phím
uint8_t key_state = 0; // Trạng thái hiện tại của phím

int8_t Keypad_Scan() {
	
	uint8_t col_mask = 0b11111011; // initial row mask
	int8_t scan_col;			//scanning col index
	
	//DDRB = 0x07; // set upper 4 bits of PORTA as input with pull-up, lower 4 bits as output
	//PORTB = 0xFF;//enable pullup resistor on the input pin

	for (scan_col=2;scan_col>=0;scan_col--) {
		PORTB = col_mask; // scan current col
		_delay_us(1); // wait for stable input

		if ((PINB & (1 << PINB3)) == 0) { // check row 0
			return  (scan_col) ; // calculate key value as (row index * 4) + column index
			} 
		else if ((PINB & (1 << PINB4)) == 0) { // check row 1
			return  (3 + scan_col) ; // calculate key value as (row index * 4) + column index
			} 
		col_mask = ((col_mask >> 1) | (1<<7)) ; // shift row mask to scan next row
		}

	return -1;
}
//-------------------------------------------------------------
void UART_Init() {
    // Baudrate = 9600
    UBRR0H = 0;
    UBRR0L = 103;

    // Kích hoạt truyền và nhận
    UCSR0B = (1 << TXEN0);

    // Cấu hình định dạng khung: 8 bit dữ liệu, 1 bit dừng
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
            if (last_key == -1) { // Nếu đây là lần đầu tiên phím được nhấn
                last_key = key;
                key_press_time = 0;
            } else if (key == last_key) { // Nếu phím vẫn đang được giữ
                key_press_time++;
                if (key_press_time >= 1000) { // Nếu phím được giữ trong 1s
                    key_press_time = 0;
                    UART_Trans('0'  + key);
                    UART_Trans('\n');
                }
            } else { // Nếu một phím khác được nhấn
                last_key = key;
                key_press_time = 0;
            }
        } else { // Nếu không có phím nào được nhấn
            last_key = -1;
        }
        _delay_ms(1); // Đợi 1ms trước khi quét lại


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