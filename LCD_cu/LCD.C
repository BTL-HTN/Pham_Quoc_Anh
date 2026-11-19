#include "LCD.h"
#include "i2c.h"

// Hàm delay n?i b? (U?c tính cho chip ch?y 72MHz)
// GI?I THÍCH: Dùng vòng l?p tr?ng d? t?o kho?ng tr?. 
// Ðã s?a l?i: Khai báo bi?n 'i' ra ngoài vòng l?p for.
static void delay_ms(uint32_t ms) {
    uint32_t i; 
    for (i = 0; i < ms * 8000; i++) {
        __NOP();
    }
}

// 1. Hàm g?i 1 byte thô t?i chip PCF8574 qua I2C
// GI?I THÍCH: Giao ti?p tr?c ti?p v?i chip m? r?ng trên module I2C.
static void lcd_write_i2c(uint8_t data) {
    i2c_start(1);
    i2c_add(1, LCD_ADDR, 0); // G?i d?a ch? LCD ? ch? d? Ghi (Write)
    i2c_data(1, data);       // G?i byte d? li?u
    i2c_stop(1);
}

// 2. Hàm g?i d? li?u theo co ch? 4-bit
// GI?I THÍCH: LCD 16x2 qua I2C nh?n d? li?u 8-bit b?ng cách chia dôi: 
// G?i 4 bit cao tru?c, sau dó g?i 4 bit th?p.
// M?i l?n g?i ph?i t?o 1 xung "Enable" (b?t r?i t?t chân EN) d? LCD nh?n d? li?u.
static void lcd_send_4bit(uint8_t val, uint8_t rs) {
    uint8_t high_nib = (val & 0xF0) | rs | BL_BIT;
    uint8_t low_nib  = ((val << 4) & 0xF0) | rs | BL_BIT;

    // G?i 4 bit cao
    lcd_write_i2c(high_nib | EN_BIT); // B?t EN
    delay_ms(1);
    lcd_write_i2c(high_nib);          // T?t EN (Ch?t d? li?u)
    
    // G?i 4 bit th?p
    lcd_write_i2c(low_nib | EN_BIT);  // B?t EN
    delay_ms(1);
    lcd_write_i2c(low_nib);           // T?t EN (Ch?t d? li?u)
}

// 3. Hàm g?i L?nh (Ví d?: Xóa màn hình, di chuy?n con tr?)
// GI?I THÍCH: RS = 0 có nghia là d? li?u g?i di là m?t L?nh di?u khi?n.
void lcd_send_cmd(char cmd) {
    lcd_send_4bit(cmd, 0);
}

// 4. Hàm g?i D? li?u/Ký t? (Ví d?: Ch? 'A', 'H', 'E'...)
// GI?I THÍCH: RS = RS_BIT (thu?ng là 1) có nghia là d? li?u g?i di là ký t? d? hi?n th?.
void lcd_send_data(char data) {
    lcd_send_4bit(data, RS_BIT);
}

// 5. Kh?i t?o LCD
// GI?I THÍCH: Th?c hi?n chu?i l?nh b?t bu?c theo Datasheet d? "dánh th?c" LCD
// và chuy?n nó sang ch? d? ho?t d?ng 4-bit qua giao ti?p I2C.
void lcd_init(void) {
    delay_ms(50);
    lcd_write_i2c(0x30 | EN_BIT | BL_BIT);
    lcd_write_i2c(0x30 | BL_BIT);
    delay_ms(5);
    lcd_write_i2c(0x30 | EN_BIT | BL_BIT);
    lcd_write_i2c(0x30 | BL_BIT);
    delay_ms(1);
    lcd_write_i2c(0x20 | EN_BIT | BL_BIT);
    lcd_write_i2c(0x20 | BL_BIT);

    lcd_send_cmd(0x28); // C?u hình: 4-bit, 2 dòng, font 5x8
    lcd_send_cmd(0x0C); // B?t màn hình, t?t con tr? nh?p nháy
    lcd_send_cmd(0x06); // T? d?ng d?ch con tr? sang ph?i khi vi?t ch?
    lcd_send_cmd(0x01); // L?nh xóa toàn b? màn hình
    delay_ms(2);
}

// 6. Hàm g?i m?t chu?i ký t?
void lcd_send_string(char *str) {
    while (*str) lcd_send_data(*str++);
}

// 7. Hàm d?t v? trí con tr?
// GI?I THÍCH: Row 0 là dòng 1, Row 1 là dòng 2. Col t? 0 d?n 15.
void lcd_set_cursor(uint8_t row, uint8_t col) {
    uint8_t addr = (row == 0) ? (0x80 + col) : (0xC0 + col);
    lcd_send_cmd(addr);
}

// 8. Hàm xóa màn hình
void lcd_clear(void) {
    lcd_send_cmd(0x01); // G?i mã l?nh xóa màn hình
    delay_ms(2);
}
// Chú ý: Ðã thêm dòng tr?ng cu?i file d? h?t c?nh báo.