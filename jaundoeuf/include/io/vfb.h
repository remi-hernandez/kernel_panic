#ifndef __VFB__
#define __VFB__

#include "types.h"

#define VID_FRAME_BUF (void *) 0xB8000
#define NB_LINE       25
#define NB_COLUMN     80

struct s_vfb
{
  struct
  {
    int       x;
    int       y;
  } pos;

  uint8_t     color;
  uint16_t *  base;
  char        is_esc_seq;
  char        esc_buf[16];
  int         esc_buf_ndx;
};

ssize_t       vfb_write(const char * str, size_t count);
void          vfb_set_color(uint8_t color);
void          vfb_set_backgrd_color(uint8_t color);
void          vfb_set_foregrd_color(uint8_t color);
void          vfb_clear(void);

#endif
