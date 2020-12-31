#ifndef _LCOM_CHARS_H
#define _LCOM_CHARS_H


#define CHAR_W  10 /** Char xpm width */
#define CHAR_H  14 /** Char xpm height */

/* Keys indexes */
#define SPACE     62      /** Code to space key scancode */
#define NOTHING   100     /** Code to scancode of non effect key */
#define BACKSPACE 200     /** Code to backscpace key scancode */
#define L_ARROW   101     /** Code to left arrow key scancode */
#define R_ARROW   102     /** Code to right arrow key scancode */
#define T_ARROW   103     /** Code to top arrow key scancode */
#define D_ARROW   104     /** Code to down arrow key scancode */
#define ESC       105     /** Code to esc key scancode */

/**
 * @brief Indicate the color that Char need to be drawn
 * 
 */
typedef enum {
  NORMAL,     /** Draw Char in black */
  WRONG,      /** Draw Char in red */
  RIGHT       /** Draw Char in green */
} Char_state;

/**
 * @brief Represents a drawn char
 * 
 */
typedef struct {
    uint8_t index;    /** Index in array list_chars that indentify the char to be drawn */
    uint16_t posx;    /** x position of top-left corner where the Char is drawn */
    uint16_t posy;    /** y position of top-left corner where the Char is drawn */
    Char_state state; /** Indirect color of Char */
} Char;

/**
 * @brief Read chars xpms and keep their maps in uint8_t ** letters_maps
 * 
 * @return int 0 if success
 */
int Chars_init();

/**
 * @brief Free letters_maps
 * 
 * @return int 
 */
int Chars_end();


/**
 * @brief Convert text of Chars structure to text of normal chars
 * 
 * @param text Place to write converted text
 * @param text_Char Text to be converted
 * @param len Length of the text to be converted
 * @return 0 if success
 */
int convert_text_Char_to_text(char* text, Char* text_Char, size_t len);

/**
 * @brief Convert text of normal chars to text of Chars structure
 * 
 * @param text Text to be converted
 * @param text_Char Place to write converted text
 * @param len Length of the text to be converted
 * @param x_position X position of first Char top-left corner
 * @param y_position Y position of first Char top-left corner
 * @return Number of lines occupied by converted text
 */
uint16_t convert_text_to_text_char(const char* text, Char* text_Char, size_t len, uint16_t x_position, uint16_t y_position);

/**
 * Convert text of normal chars to text of Chars structure and display it
 * @param text Text to be converted
 * @param text_Char Place to write converted text
 * @param len Length of the text to be converted
 * @param x_position X position of first Char top-left corner
 * @param y_position Y position of first Char top-left corner
 * @return Number of lines occupied by converted text
 */
int display_text(const char* text, Char* text_Char, size_t len, uint16_t x_position, uint16_t y_position);
/**
 * @brief Display text of Chars
 * 
 * @param text_Char Text to be displayed
 * @param len Lemgth of text to be displayed
 * @return int 
 */
int display_text_Char(Char* text_Char, size_t len);

/**
 * @brief Converts an integer number to text and displays it
 * 
 * @param integer: number to convert to text and to display
 * @param x_position: x position where start writing the text
 * @param y_position: y position where start writing the text
 */
void display_integer(int integer, uint16_t x, uint16_t y);

/**
 * @brief Converts a float number to text and displays it
 * @param decimal: number to convert to text and to display
 * @param x_position: x position where start writing the text
 * @param y_position: y position where start writing the text
 */
void display_float(float decimal, uint16_t x, uint16_t y);
/**
 * @brief Converts seconds to text and display in format min:sec
 * @param seconds: number of seconds 
 * @param x_position: x position where start writing the text
 * @param y_position: y position where start writing the text
 */
void display_time(uint16_t seconds, uint16_t x, uint16_t y);

/**
 * @brief Display Char
 * 
 * @param c Char to be displayed
 */
void display_Char(Char* c);
/**
 * @brief Get the index of letters maps corresponding to the scancode passed as argument
 * 
 * @param bytes Scancode to index
 * @return Index of letters maps
 */
uint8_t get_scancode_char(uint8_t *bytes);



#endif
