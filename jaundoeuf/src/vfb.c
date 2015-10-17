#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "kernel.h"
#include "io/vfb.h"

/* not thread safe */
static struct s_vfb g_vfb = {
    .pos = { .x = 0, .y = 0 },
    .color = 0x07,
    .base = VID_FRAME_BUF + VIRTUAL_BASE,
    .is_esc_seq = 0,
    .esc_buf_ndx = 0,
};

int vfb_foregrd_val[] = {
    0x00, /* 30 - black */
    0x04, /* 31 - red */
    0x02, /* 32 - green */
    0x06, /* 33 - yellow */
    0x01, /* 34 - blue */
    0x05, /* 35 - magenta */
    0x03, /* 36 - cyan */
    0x07, /* 37 - white */
};

int vfb_backgrd_val[] = {
    0x00, /* 40 - black */
    0x40, /* 41 - red */
    0x20, /* 42 - green */
    0x60, /* 43 - yellow */
    0x10, /* 44 - blue */
    0x50, /* 45 - magenta */
    0x30, /* 46 - cyan */
    0x70, /* 47 - white */
};

static inline void _handle_color_code(int color)
{
    if (color >= 0 && color <= 8)
    {
        if (color == 0)
            g_vfb.color &= 0x77;
        if (color == 1)
            g_vfb.color |= 0x88;
    }
    else if (color >= 30 && color <= 37) /* foreground */
        g_vfb.color = (g_vfb.color & 0x78) | (vfb_foregrd_val[color - 30] & 0x7);
    else if (color >= 40 && color <= 47) /* background */
        g_vfb.color = (vfb_backgrd_val[color - 40] & 0x70) | (g_vfb.color & 0x8F);
}

static inline char _handle_color_seq(void)
{
    char * str = g_vfb.esc_buf;
    int codes[3] = { 0, 0, 0 };
    int i = 1, j = 0;

    for (char stop = 0; !stop; ++j)
    {
        if (j >= 3)
            return (0);
        codes[j] = atoi(str + i);
        while (i < g_vfb.esc_buf_ndx && str[i] != ';' && str[i] != 'm')
            ++i;
        if (i >= g_vfb.esc_buf_ndx)
            return (0);
        else if (str[i] == 'm')
            stop = 1;
        else if (str[i] == ';')
            ++i;
    }
    for (i = 0; i < j; ++i)
        _handle_color_code(codes[i]);

    return (1);
}

static inline char _handle_escape_seq()
{
    char status = 0;

    /* color sequence */
    if (g_vfb.esc_buf[0] == '[')
        status = _handle_color_seq();

    return (status);
}

/* \r */
static inline void _carriage_return(void)
{
    g_vfb.pos.x = 0;
}

/* \b */
static inline void _back_cursor(void)
{
    if (g_vfb.pos.x > 0)
        --g_vfb.pos.x;
}

static inline void _scroll_up(void)
{
    for (int i = 0; i < NB_LINE * NB_COLUMN; ++i)
    {
        if (i < (NB_LINE - 1) * NB_COLUMN)
            g_vfb.base[i] = g_vfb.base[i + NB_COLUMN];
        else
            g_vfb.base[i] = ' ' | (((0 << 4) | (15 & 0x0F)) << 8);
    }

    g_vfb.pos.y = NB_LINE - 1;
    g_vfb.pos.x = 0;
}

/* \n */
static inline void _add_newline(void)
{
    g_vfb.pos.x = 0;

    if (g_vfb.pos.y < NB_LINE - 1)
        ++g_vfb.pos.y;
    else
        _scroll_up();
}

static inline void _putc(char c)
{
    g_vfb.base[g_vfb.pos.y * NB_COLUMN + g_vfb.pos.x] = (g_vfb.color << 8) | c;

    if (++g_vfb.pos.x == NB_COLUMN)
        _add_newline();
}

ssize_t
vfb_write(const char * s, size_t count)
{
    ssize_t i;

    for (i = 0; i < (ssize_t) count; ++i)
    {
        if (g_vfb.is_esc_seq)
        {
            g_vfb.esc_buf[g_vfb.esc_buf_ndx++] = s[i];

            if (_handle_escape_seq())
            {
                g_vfb.is_esc_seq = 0;
                g_vfb.esc_buf_ndx = 0;
            }
        }
        else if (s[i] == 27) /* ESC */
            g_vfb.is_esc_seq = 1;
        else if (s[i] == '\n')
            _add_newline();
        else if (s[i] == '\b')
            _back_cursor();
        else if (s[i] == '\r')
            _carriage_return();
        else
            _putc(s[i]);
    }

    return (i);
}
