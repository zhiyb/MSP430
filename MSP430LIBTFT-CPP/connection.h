/*
 * Author: Yubo Zhi (yz39g13@soton.ac.uk)
 */

#ifndef LIBTFT_CPP_CONNECTION_H
#define LIBTFT_CPP_CONNECTION_H

#include "msp430.h"

// 8-bit parallel interface I

#define TFT_PCTRL	P6DIR
#define TFT_WCTRL	P6OUT
#define TFT_RCTRL	P6IN
#define TFT_PDATA	P2DIR
#define TFT_WDATA	P2OUT
#define TFT_RDATA	P2IN

#define TFT_CS	(1 << 2)	// Chip-select (0)
#define TFT_BLC	(1 << 3)	// Background light control (1)
#define TFT_RST	(1 << 4)	// Reset (0)
#define TFT_WR	(1 << 5)	// Parallel data write strobe (Rising)
#define TFT_RS	(1 << 6)	// Data(1) / Command(0) selection
#define TFT_RD	(1 << 7)	// Parallel data read strobe (Rising)
#define TFT_VSY	(1 << 2)	// VSYNC
#define TFT_FMK	(1 << 3)	// Frame mark

#endif
