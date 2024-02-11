/*******************************************************
This program was created by the
CodeWizardAVR V3.14 Advanced
Automatic Program Generator
© Copyright 1998-2014 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

Project : Tic-Tac-Toe
Version : 1.0.0
Date    : 2/10/2024
Author  : Jani, Heidari and Anvari
Company : 
Comments: 
This project demonstrates how to implement a tic tac toe game using an atmega32 microcontroller and a 4x4 keypad as input.
The game logic is programmed in C language and simulated in Proteus software. 
The game board is displayed on a 20x4 LCD module and the player can select a cell by pressing the corresponding key.
The game alternates between two players, X and O, until one of them wins or the board is full.

Contributors:
Sina Jani
Arman Heidari
Amir Mohammad Anvari

License:
MIT


Chip type               : ATmega32
Program type            : Application
AVR Core Clock frequency: 8.000000 MHz
Memory model            : Small
External RAM size       : 0
Data Stack size         : 512
*******************************************************/

#include <mega32.h>
#include <delay.h>
#include <string.h>
#include <stdio.h>

// Alphanumeric LCD functions
#include <alcd.h>

// Declare your global variables here
int board[3][3], player, winner, row_number, column_number, selected_key, t, moves, counter;
char row[4]={0xFE, 0xFD, 0xFB, 0xF7}, text[20];
//           1110  1101  1100  0111


// Keypad:
// [id:00|1] [id:01|2] [id:02|3]
// [id:03|4] [id:04|5] [id:05|6]
// [id:06|7] [id:07|8] [id:08|9]
// [id:09|*] [id:10|0] [id:11|#]

void keypad() {
    while(1) {
        for (row_number = 0; row_number < 4; row_number++) {
            column_number = 3;
            PORTB = row[row_number];

            if (PINB.4 == 0) {
                column_number = 0;
            }

            if (PINB.5 == 0) {
                column_number = 1;
            }

            if (PINB.6 == 0) {
                column_number = 2;
            }

            if (!(column_number == 3)) {
                while(PINB.4 == 0);
                while(PINB.5 == 0);
                while(PINB.6 == 0);
                return;
            }
            delay_ms(1);
        }
    }
}

void init_game() {
    player = 1;
    moves = 0;
    for (row_number = 0; row_number < 3; row_number++) {
        for (column_number = 0; column_number < 3; column_number++) {
            board[row_number][column_number] = 0;
        }
    }
    
    row_number = 0;
    column_number = 0;
    
    lcd_clear();
    lcd_puts("Tic-Tac-Toe Game\nPress * to Start...\n");
    
    do {
        keypad();
        selected_key = row_number * 3 + column_number;
    } while(selected_key != 9);

    lcd_clear();
    
    for(t = 3; t > 0; t--) {
        sprintf(text, "Game Starts in:%d", t);
        lcd_puts(text);
        delay_ms(100);
        lcd_clear();
    }
    PORTA = 0x01;
}

void print_board()
{
    lcd_clear();
    for (row_number = 0; row_number < 3; row_number++) {
        lcd_gotoxy(8, row_number + 1);
        lcd_putchar('|');
        for(column_number = 1; column_number < 4; column_number++) {
            lcd_gotoxy(column_number + 8, row_number + 1);

            if(board[row_number][column_number - 1] == 1) {
                lcd_putchar('X');
            }
            else if(board[row_number][column_number - 1] == -1) {
                lcd_putchar('O');
            }
            else {
                lcd_putchar('-');
            }
        }
        lcd_gotoxy(12, row_number + 1);
        lcd_putchar('|');
    }

    if (player == 1) {
        lcd_gotoxy(0, 0);
        sprintf(text, "Turn of Player 1, X");
        lcd_puts(text);

    }
    else {
        lcd_gotoxy(0, 0);
        sprintf(text, "Turn of Player 2, O");
        lcd_puts(text);
    }
}

void print_final_board(int winner)
{
    lcd_clear();
    for (row_number = 0; row_number < 3; row_number++) {
        lcd_gotoxy(8, row_number + 1);
        lcd_putchar('|');
        for(column_number = 1; column_number < 4; column_number++) {
            lcd_gotoxy(column_number + 8, row_number + 1);

            if(board[row_number][column_number - 1] == 1) {
                lcd_putchar('X');
            }
            else if(board[row_number][column_number - 1] == -1) {
                lcd_putchar('O');
            }
            else {
                lcd_putchar('-');
            }
        }
        lcd_gotoxy(12, row_number + 1);
        lcd_putchar('|');
    }
    

    if (winner == 1) {
        lcd_gotoxy(0, 0);
        sprintf(text, "Player 1 won!, X :)");
        lcd_puts(text);
        counter = 0;
        while (counter < 10) {
            PORTA = 0x01;
            delay_ms(25);
            PORTA = 0x00;
            delay_ms(25);
            counter += 1;
        }
    }
    else if (winner == -1) {
        lcd_gotoxy(0, 0);
        sprintf(text, "Player 2 won!, O :)");
        lcd_puts(text);
        counter = 0;
        while (counter < 10) {
            PORTA = 0x02;
            delay_ms(25);
            PORTA = 0x00;
            delay_ms(25);
            counter += 1;
        }
    }
    else {
        lcd_gotoxy(0, 0);
        lcd_puts("It's Draw !");
        counter = 0;
        while (counter < 10) {
            PORTA = 0x03;
            delay_ms(25);
            PORTA = 0x00;
            delay_ms(25);
            counter += 1;
        }
    }
}

int check_win()
{
    winner = 0;
    //- horizontal match
    if (board[0][0] == board[0][1] && board[0][0] == board[0][2] && board[0][0] != 0) {
        winner = board[0][0];
    }
    else if (board[1][0] == board[1][1] && board[1][0] == board[1][2] && board[1][0] != 0) {
        winner = board[1][0];
    }
    else if (board[2][0] == board[2][1] && board[2][0] == board[2][2] && board[2][0] != 0) {
        winner = board[2][0];
    }

    //- vartical match
    else if (board[0][0] == board[1][0] && board[0][0] == board[2][0] && board[0][0] != 0) {
        winner = board[0][0];
    }
    else if (board[0][1] == board[1][1] && board[0][1] == board[2][1] && board[0][1] != 0) {
        winner = board[0][1];
    }
    else if (board[0][2] == board[1][2] && board[0][2] == board[2][2] && board[0][2] != 0) {
        winner = board[0][2];
    }

    //- diagonal match
    else if (board[0][0] == board[1][1] && board[0][0] == board[2][2] && board[0][0] != 0) {
        winner = board[0][0];
    }
    else if (board[0][2] == board[1][1] && board[0][2] == board[2][0] && board[0][2] != 0) {
        winner = board[0][2];
    }

    return winner;
}

void main(void)
{
    // Declare your local variables here
    int winner_result, reset = 0;

    // Input/Output Ports initialization
    // Port A initialization
    // Function: Bit7=In Bit6=In Bit5=In Bit4=In Bit3=In Bit2=In Bit1=Out Bit0=Out 
    DDRA=(0<<DDA7) | (0<<DDA6) | (0<<DDA5) | (0<<DDA4) | (0<<DDA3) | (0<<DDA2) | (1<<DDA1) | (1<<DDA0);
    // State: Bit7=T Bit6=T Bit5=T Bit4=T Bit3=T Bit2=T Bit1=T Bit0=T 
    PORTA=(0<<PORTA7) | (0<<PORTA6) | (0<<PORTA5) | (0<<PORTA4) | (0<<PORTA3) | (0<<PORTA2) | (0<<PORTA1) | (0<<PORTA0);

    // Port B initialization
    // Function: Bit7=In Bit6=In Bit5=In Bit4=In Bit3=Out Bit2=Out Bit1=Out Bit0=Out 
    DDRB=(0<<DDB7) | (0<<DDB6) | (0<<DDB5) | (0<<DDB4) | (1<<DDB3) | (1<<DDB2) | (1<<DDB1) | (1<<DDB0);
    // State: Bit7=0 Bit6=0 Bit5=0 Bit4=0 Bit3=T Bit2=T Bit1=T Bit0=T 
    PORTB=(0<<PORTB7) | (0<<PORTB6) | (0<<PORTB5) | (0<<PORTB4) | (0<<PORTB3) | (0<<PORTB2) | (0<<PORTB1) | (0<<PORTB0);

    // Port C initialization
    // Function: Bit7=In Bit6=In Bit5=In Bit4=In Bit3=In Bit2=In Bit1=In Bit0=In 
    DDRC=(0<<DDC7) | (0<<DDC6) | (0<<DDC5) | (0<<DDC4) | (0<<DDC3) | (0<<DDC2) | (0<<DDC1) | (0<<DDC0);
    // State: Bit7=T Bit6=T Bit5=T Bit4=T Bit3=T Bit2=T Bit1=T Bit0=T 
    PORTC=(0<<PORTC7) | (0<<PORTC6) | (0<<PORTC5) | (0<<PORTC4) | (0<<PORTC3) | (0<<PORTC2) | (0<<PORTC1) | (0<<PORTC0);

    // Port D initialization
    // Function: Bit7=Out Bit6=Out Bit5=Out Bit4=Out Bit3=Out Bit2=Out Bit1=Out Bit0=Out 
    DDRD=(1<<DDD7) | (1<<DDD6) | (1<<DDD5) | (1<<DDD4) | (1<<DDD3) | (1<<DDD2) | (1<<DDD1) | (1<<DDD0);
    // State: Bit7=0 Bit6=0 Bit5=0 Bit4=0 Bit3=0 Bit2=0 Bit1=0 Bit0=0 
    PORTD=(0<<PORTD7) | (0<<PORTD6) | (0<<PORTD5) | (0<<PORTD4) | (0<<PORTD3) | (0<<PORTD2) | (0<<PORTD1) | (0<<PORTD0);

    // Timer/Counter 0 initialization
    // Clock source: System Clock
    // Clock value: Timer 0 Stopped
    // Mode: Normal top=0xFF
    // OC0 output: Disconnected
    TCCR0=(0<<WGM00) | (0<<COM01) | (0<<COM00) | (0<<WGM01) | (0<<CS02) | (0<<CS01) | (0<<CS00);
    TCNT0=0x00;
    OCR0=0x00;

    // Timer/Counter 1 initialization
    // Clock source: System Clock
    // Clock value: Timer1 Stopped
    // Mode: Normal top=0xFFFF
    // OC1A output: Disconnected
    // OC1B output: Disconnected
    // Noise Canceler: Off
    // Input Capture on Falling Edge
    // Timer1 Overflow Interrupt: Off
    // Input Capture Interrupt: Off
    // Compare A Match Interrupt: Off
    // Compare B Match Interrupt: Off
    TCCR1A=(0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<WGM11) | (0<<WGM10);
    TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (0<<WGM12) | (0<<CS12) | (0<<CS11) | (0<<CS10);
    TCNT1H=0x00;
    TCNT1L=0x00;
    ICR1H=0x00;
    ICR1L=0x00;
    OCR1AH=0x00;
    OCR1AL=0x00;
    OCR1BH=0x00;
    OCR1BL=0x00;

    // Timer/Counter 2 initialization
    // Clock source: System Clock
    // Clock value: Timer2 Stopped
    // Mode: Normal top=0xFF
    // OC2 output: Disconnected
    ASSR=0<<AS2;
    TCCR2=(0<<PWM2) | (0<<COM21) | (0<<COM20) | (0<<CTC2) | (0<<CS22) | (0<<CS21) | (0<<CS20);
    TCNT2=0x00;
    OCR2=0x00;

    // Timer(s)/Counter(s) Interrupt(s) initialization
    TIMSK=(0<<OCIE2) | (0<<TOIE2) | (0<<TICIE1) | (0<<OCIE1A) | (0<<OCIE1B) | (0<<TOIE1) | (0<<OCIE0) | (0<<TOIE0);

    // External Interrupt(s) initialization
    // INT0: Off
    // INT1: Off
    // INT2: Off
    MCUCR=(0<<ISC11) | (0<<ISC10) | (0<<ISC01) | (0<<ISC00);
    MCUCSR=(0<<ISC2);

    // USART initialization
    // USART disabled
    UCSRB=(0<<RXCIE) | (0<<TXCIE) | (0<<UDRIE) | (0<<RXEN) | (0<<TXEN) | (0<<UCSZ2) | (0<<RXB8) | (0<<TXB8);

    // Analog Comparator initialization
    // Analog Comparator: Off
    // The Analog Comparator's positive input is
    // connected to the AIN0 pin
    // The Analog Comparator's negative input is
    // connected to the AIN1 pin
    ACSR=(1<<ACD) | (0<<ACBG) | (0<<ACO) | (0<<ACI) | (0<<ACIE) | (0<<ACIC) | (0<<ACIS1) | (0<<ACIS0);
    SFIOR=(0<<ACME);

    // ADC initialization
    // ADC disabled
    ADCSRA=(0<<ADEN) | (0<<ADSC) | (0<<ADATE) | (0<<ADIF) | (0<<ADIE) | (0<<ADPS2) | (0<<ADPS1) | (0<<ADPS0);

    // SPI initialization
    // SPI disabled
    SPCR=(0<<SPIE) | (0<<SPE) | (0<<DORD) | (0<<MSTR) | (0<<CPOL) | (0<<CPHA) | (0<<SPR1) | (0<<SPR0);

    // TWI initialization
    // TWI disabled
    TWCR=(0<<TWEA) | (0<<TWSTA) | (0<<TWSTO) | (0<<TWEN) | (0<<TWIE);

    // Alphanumeric LCD initialization
    // Connections are specified in the
    // Project|Configure|C Compiler|Libraries|Alphanumeric LCD menu:
    // RS - PORTD Bit 0
    // RD - PORTD Bit 1
    // EN - PORTD Bit 2
    // D4 - PORTD Bit 4
    // D5 - PORTD Bit 5
    // D6 - PORTD Bit 6
    // D7 - PORTD Bit 7
    // Characters/line: 20
    
    lcd_init(20);

    while(1) {
        init_game();
        print_board();

        while(1) {
            keypad();
            if (row_number != 3 && column_number != 3) {
                if (board[row_number][column_number] == 0) {
                    if (player == 1){
                        board[row_number][column_number] = 1;
                        PORTA = 0x02;
                    }
                    else {
                        board[row_number][column_number] = -1;
                        PORTA = 0x01;
                    }

                    player = (player == 1) ? -1 : 1;
                    moves++;
                }
            }
            
            selected_key = row_number * 3 + column_number;
            
            if (selected_key == 10) {
                reset = 1;
                break;
            }
            
            winner_result = check_win();

            if (winner_result == 1) {
                print_final_board(1);
                break;
            }
            else if (winner_result == -1) {
                print_final_board(-1);
                break;
            }
            else {
                if (moves == 9) {
                    print_final_board(2);
                    break;
                }
                else {
                    print_board();
                }
            }
        }

        if (reset) {
            reset = 0;
            counter = 0;
            while (counter < 10) {
                PORTA = 0x01;
                delay_ms(25);
                PORTA = 0x02;
                delay_ms(25);
                counter += 1;
            }
            PORTA = 0x00;
            continue;
        }

        while(1) {
            keypad();
            selected_key = row_number * 3 + column_number;
            if (selected_key == 10) {
                break;
            }
        }
    }
}
