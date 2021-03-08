#include "Portenta_Video.h"
#include "image_320x240_argb8888.h"
#include "SDRAM.h"
#include "mbed.h"

struct edid recognized_edid;

mbed::DigitalOut video_on(PK_2);
mbed::DigitalOut video_rst(PJ_3);
mbed::DigitalOut otg_on(PJ_6);

void setup() {
  // put your setup code here, to run once:
  printf("OTG_ON = 1 -> VBUS OFF\n");
  otg_on = 1;

  delay(1000);
  video_on = 1;
  delay(10);
  video_rst = 1;
  delay(10);

  int ret = -1;
  video_on = 0;
  delay(10);
  video_rst = 0;
  delay(100);

  video_on = 1;
  delay(100);
  video_rst = 1;

  ret = anx7625_init(0);
  if(ret < 0) {
    printf("Cannot continue, anx7625 init failed.\n");
    while(1);
  }

  if(anx7625_is_power_provider(0)) {
    printf("OTG_ON = 0 -> VBUS ON\n");
    otg_on = 0;
    delay(1000); // Wait for device to be stable
  }

  anx7625_wait_hpd_event(0);

  anx7625_dp_get_edid(0, &recognized_edid);
  anx7625_dp_start(0, &recognized_edid, EDID_MODE_640x480_60Hz);

  SDRAM.begin(getFramebufferEnd());
  while (1) {
    stm32_LCD_DrawImage((void*)texture_raw, (void *)getNextFrameBuffer(), 300, 300, DMA2D_INPUT_RGB565);
    stm32_LCD_DrawImage((void*)texture_raw, (void *)getNextFrameBuffer(), 300, 300, DMA2D_INPUT_RGB565);
  }
}

int i = 0;

void loop() {
  printf("Now: %d\n", millis());

  delay(1000);
}
