//#define F_CPU 3333333UL // Internal clock speed of ATtiny202
#include <avr/io.h>
#include <util/delay.h>
#include <ctype.h>
#include <string.h>

const char* message = "KF7QXR"; 

#define DOT_DURATION 30 // Duration of a dot in milliseconds
#define DASH_DURATION (3 * DOT_DURATION) // Duration of a dash in milliseconds
#define SYMBOL_SPACE DOT_DURATION // Space between symbols in milliseconds
#define LETTER_SPACE (3 * DOT_DURATION) // Space between letters in milliseconds
#define WORD_SPACE (7 * DOT_DURATION) // Space between words in milliseconds

#define LED_PIN PIN1_bm

// Morse code lookup table
const char* morse_table[36] = {
    ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-", ".-..", "--", "-.",
    "---", ".--.", "--.-", ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..", // A-Z
    "-----", ".----", "..---", "...--", "....-", ".....", "-....", "--...", "---..", "----." // 0-9
};

void blink_dot() {
    PORTA.OUTSET = LED_PIN; // Turn on LED
    _delay_ms(DOT_DURATION);
    PORTA.OUTCLR = LED_PIN; // Turn off LED
    _delay_ms(SYMBOL_SPACE);
}

void blink_dash() {
    PORTA.OUTSET = LED_PIN; // Turn on LED
    _delay_ms(DASH_DURATION);
    PORTA.OUTCLR = LED_PIN; // Turn off LED
    _delay_ms(SYMBOL_SPACE);
}

void blink_char(char c) {
    if (c == ' ') {
        _delay_ms(WORD_SPACE - LETTER_SPACE);
        return;
    }
    const char* morse_code = NULL;
    if (c >= 'A' && c <= 'Z') {
        morse_code = morse_table[c - 'A'];
    } else if (c >= '0' && c <= '9') {
        morse_code = morse_table[c - '0' + 26];
    }
    if (morse_code) {
        while (*morse_code) {
            if (*morse_code == '.') {
                blink_dot();
            } else if (*morse_code == '-') {
                blink_dash();
            }
            morse_code++;
        }
        _delay_ms(LETTER_SPACE - SYMBOL_SPACE);
    }
}

void blink_string(const char* str) {
    while (*str) {
        blink_char(toupper(*str));
        str++;
    }
}

int main(void) {
    PORTA.DIRSET = LED_PIN; 
    while (1) {
       blink_string("CQ CQ CQ");
        _delay_ms(WORD_SPACE - LETTER_SPACE);
        blink_string(message);
        _delay_ms(WORD_SPACE);
    }

    return 0;
}