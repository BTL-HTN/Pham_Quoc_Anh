#ifndef __LCD_H    // Ki?m tra xem file này dã du?c d?nh nghia chua (tránh trùng l?p)
#define __LCD_H    // Ð?nh nghia tên file d? trình biên d?ch ghi nh?

#include "stm32f10x.h" // Thu vi?n h? th?ng c?a STM32F103

/* --- C?U HÌNH Ð?A CH? VÀ CÁC BIT ÐI?U KHI?N --- */
#define LCD_ADDR 0x4E  // Ð?a ch? I2C c?a module (0x27 d?ch trái 1 bit thành 0x4E)

#define RS_BIT   0x01  // Bit RS (P0): 0 là g?i L?nh, 1 là g?i D? li?u
#define EN_BIT   0x04  // Bit EN (P2): T?o xung d? LCD ch?t (nh?n) d? li?u
#define BL_BIT   0x08  // Bit BL (P3): Ði?u khi?n b?t (1) ho?c t?t (0) dèn n?n

/* --- CÁC MÃ L?NH LCD CO B?N --- */
#define LCD_CLEAR 0x01 // Mã l?nh d? xóa s?ch toàn b? màn hình

/* --- CÁC HÀM GIAO TI?P LCD (PROTOTYPES) --- */

// Hàm kh?i t?o LCD: Thi?t l?p ch? d? 4-bit, b?t màn hình, xóa màn hình lúc d?u
void lcd_init(void);

// Hàm g?i L?nh: Dùng khi mu?n di?u khi?n LCD (Ví d?: Chuy?n dòng, t?t con tr?)
void lcd_send_cmd(char cmd);

// Hàm g?i D? li?u: Dùng khi mu?n hi?n th? 1 ký t? lên màn hình (Ví d?: ch? 'A')
void lcd_send_data(char data);

// Hàm g?i Chu?i: Dùng d? in c? m?t dòng ch? (Ví d?: "Hello STM32")
void lcd_send_string(char *str);

// Hàm d?t con tr?: Di chuy?n v? trí vi?t ch? (row: 0-1 là dòng, col: 0-15 là c?t)
void lcd_set_cursor(uint8_t row, uint8_t col);

// Hàm xóa màn hình: Th?c hi?n nhanh vi?c xóa s?ch n?i dung dang hi?n th?
void lcd_clear(void);

#endif // K?t thúc file .h