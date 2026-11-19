/* ===================== TASK 2: RFID (Priority 2) ===================== */
static void Task_RFID(void *pvParameters)
{
    Event_t ev;
    uint8_t uid[4];
    uint8_t chk = 0;
    uint8_t ver;
    (void)pvParameters;

    memset(&ev, 0, sizeof(ev));
    ev.type = EV_CARD;                       // Co dinh loai su kien cua Task nay la EV_CARD

    while (1) {
        if (!g_rc522_ok) {                   // Neu chip RC522 chua san sang hoac bi mat ket noi
            g_rc522_ok = RC522_Init();       // Thu khoi tao lai giao tiep SPI voi RC522
            if (!g_rc522_ok) {
                vTaskDelay(pdMS_TO_TICKS(2000)); // Neu van loi, doi 2s sau thu lai de tiet kiem CPU
                continue;
            }
        }

        if (RC522_Request() == RC522_OK) {           // Phat song RF tim xem co the nao trong vung quet khong
            if (RC522_Anticoll(uid) == RC522_OK) {   // Chong xung dot (neu nhieu the) va doc ma UID 4 byte
                RC522_Halt();                        // Ra lenh cho the ngu dong -> tranh doc lap lai lien tuc
                memcpy(ev.uid, uid, 4);              // Chep ma UID vao goi tin su kien
                xQueueSend(xEventQueue, &ev, 0);     // Day su kien the vao hang doi
                vTaskDelay(pdMS_TO_TICKS(500));      // Nghi 500ms sau khi doc thanh cong
            }
        }

        // Co che tu phuc hoi (Watchdog phan mem): Cu 50 lan quet (~5s) kiem tra thanh ghi phien ban RC522
        chk++;
        if (chk >= 50) {
            chk = 0;
            ver = RC522_ReadVersion();
            if (ver == 0x00 || ver == 0xFF) g_rc522_ok = 0; // Neu doc ra 0x00/0xFF tuc la dut day SPI -> reset co
        }
        vTaskDelay(pdMS_TO_TICKS(100));      // Chu ky quet the 100ms (10 lan/giay)
    }
}
