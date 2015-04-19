/*
 * Author: Yubo Zhi (yz39g13@soton.ac.uk)
 */

#ifndef ASCII_H
#define ASCII_H

#ifdef __cplusplus
extern "C" {
#endif

#define FONT_WIDTH 6
#define FONT_HEIGHT 8

#define ASCII_SIZE	99

extern const unsigned char ascii[ASCII_SIZE][8];
#ifdef ASCII_STRIPPED
extern const unsigned char ascii_stripped[96][6];
#endif

#ifdef __cplusplus
}
#endif

#endif
