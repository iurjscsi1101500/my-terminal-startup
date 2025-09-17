#ifndef COLOR_ART_H
#define COLOR_ART_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void color_art_print(const char *art);

static int supports_truecolor(void) {
    const char *ct = getenv("COLORTERM");
    return ct && (strstr(ct, "truecolor") || strstr(ct, "24bit"));
}

static int supports_256color(void) {
    const char *t = getenv("TERM");
    return t && strstr(t, "256color");
}

static void col_to_rgb(int col, int width, int *r, int *g, int *b) {
    if (width <= 1) { *r = *g = *b = 255; return; }
    double t = (double)col / (double)(width - 1), p, rr = 0, gg = 0, bb = 0;
    if (t < 0.25)      { p = t / 0.25;         rr = 0;     gg = 255 * p; bb = 255; }
    else if (t < 0.5)  { p = (t - 0.25) / 0.25; rr = 0;     gg = 255;    bb = 255 * (1 - p); }
    else if (t < 0.75) { p = (t - 0.5) / 0.25;  rr = 255 * p; gg = 255;  bb = 0; }
    else               { p = (t - 0.75) / 0.25; rr = 255;   gg = 255 * (1 - p); bb = 0; }
    *r = (int)rr; *g = (int)gg; *b = (int)bb;
}

static int rgb_to_256(int r, int g, int b) {
    int ri = (r * 5) / 255, gi = (g * 5) / 255, bi = (b * 5) / 255;
    return 16 + 36 * ri + 6 * gi + bi;
}

static void print_border(int inner_width) {
    putchar('+');
    for (int i = 0; i < inner_width + 2; ++i) putchar('-');
    putchar('+'); putchar('\n');
}

static void print_framed_line(const char *line, int maxw, int use_true, int use_256) {
    putchar('|'); putchar(' ');
    int col = 0, len = (int)strlen(line);
    for (int i = 0; i < len; ++i) {
        char c = line[i];
        int r, g, b; col_to_rgb(col, maxw, &r, &g, &b);
        if (c == ' ' || c == '\t') putchar(c);
        else if (use_true) printf("\x1b[38;2;%d;%d;%dm%c\x1b[0m", r, g, b, c);
        else if (use_256)  printf("\x1b[38;5;%dm%c\x1b[0m", rgb_to_256(r, g, b), c);
        else {
            const int basic[] = {31, 32, 33, 34, 35, 36};
            printf("\x1b[%dm%c\x1b[0m", basic[col % 6], c);
        }
        ++col;
    }
    for (int p = col; p < maxw; ++p) putchar(' ');
    putchar(' '); putchar('|'); putchar('\n');
}

static void color_art_print(const char *art) {
    if (!art) return;
    size_t len = strlen(art), lines_count = 1;
    for (size_t i = 0; i < len; ++i) if (art[i] == '\n') ++lines_count;
    char *buf = malloc(len + 1); if (!buf) return;
    memcpy(buf, art, len + 1);
    for (size_t i = 0; i < len; ++i) if (buf[i] == '\r') buf[i] = '\n';
    char **lines = malloc(sizeof(char *) * (lines_count + 1)); if (!lines) { free(buf); return; }
    size_t li = 0; lines[li++] = buf;
    for (size_t i = 0; i < len; ++i) if (buf[i] == '\n') { buf[i] = '\0'; if (i + 1 < len) lines[li++] = &buf[i+1]; }
    int maxw = 0; for (size_t i = 0; i < li; ++i) { int w = (int)strlen(lines[i]); if (w > maxw) maxw = w; }
    int use_true = supports_truecolor(), use_256 = !use_true && supports_256color();
    print_border(maxw);
    for (size_t i = 0; i < li; ++i) print_framed_line(lines[i], maxw, use_true, use_256);
    print_border(maxw);
    free(lines); free(buf);
}

#endif /* COLOR_ART_H */

