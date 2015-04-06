#include <msp430.h>
#include <cstring>
#include "tft.h"

tft_t tft;

// Output to stdout
extern "C" {
	int puts(const char *_ptr)
	{
		tft.putString(_ptr);
		tft << '\n';
		return std::strlen(_ptr);
	}

	int putc(int _c, register FILE *_fp)
	{
		tft << (char)_c;
		return _c;
	}

	int fputs(const char *_ptr, register FILE *_fp)
	{
		tft.putString(_ptr);
		return std::strlen(_ptr);
	}

	int fputc(int _c, register FILE *_fp)
	{
		tft << (char)_c;
		return _c;
	}
}

void init(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

	// Initialise Power Management Module
	PMMCTL0 = PMMPW | PMMCOREV_3;

	// Initialise clock sources
	// [DCO] & [MOD] modified automatically by FLL (in UCSCTL0)
	// [DCORSEL] DCO frequency range (6, 10.7-39MHz), [DISMOD] enable modulation
	UCSCTL1  = (6 << 4) | 0;
	// [FLLD] FLL loop divider (0, f(DCOCLK)/1), [FLLN] DCO multiplier (15, N + 1)
	UCSCTL2 = FLLD__1 | 19;
	// [SELREF] FLL reference (5, XT2CLK), [FLLREFDIV] reference divider (4, f(FLLREFCLK)/12)
	UCSCTL3 = SELREF__XT2CLK | FLLREFDIV__12;
	// [SELA] ACLK source (2, REFOCLK), [SELS] SMCLK source (3, DCOCLK), [SELM] MCLK source (3, DCOCLK)
	UCSCTL4 = SELA__REFOCLK | SELS__DCOCLK | SELM__DCOCLK;
	// [DIVPA] (5, 32), [DIVA], [DIVS], [DIVM]
	UCSCTL5 = DIVPA__32 | DIVA__1 | DIVS__4 | DIVM__1;
	UCSCTL6 = XT2DRIVE_1 /*| XT2BYPASS*/ /*| XT2OFF*/ | XT1DRIVE_0 /*| XTS*/ | XT1BYPASS | XT1OFF;
	// UCSCTL7 is fault flags
	UCSCTL8 = MODOSCREQEN | SMCLKREQEN | MCLKREQEN | ACLKREQEN;
	// UCSCTL9 is bypass settings

	tft.init();
	tft.setOrient(tft.FlipLandscape);
	tft.setBackground(0x667F);
	tft.setForeground(0x0000);
	tft.clean();
	tft.setBGLight(true);
}

int main(void)
{
	init();
	
	start:
		tft.clean();
		tft.setZoom(1);
		puts("*** TFT library testing ***");
		puts("STDOUT output, orientation, FG/BG colour, BG light");
		tft.setZoom(3);
		puts("Font size test");
		tft.setZoom(1);
		tft.setXY(300, 38);
		puts("Change text position & word warp test");
		tft.frame(115, 56, 200, 10, 2, 0xF800);
		puts("Draw frame test");
		tft.rectangle(118, 68, 180, 6, 0x07E0);
		puts("Draw rectangle test");
		tft.point(120, 76, 0x001F);
		tft.point(122, 76, 0x001F);
		tft.point(124, 76, 0x001F);
		tft.point(126, 76, 0x001F);
		tft.point(128, 76, 0x001F);
		tft.point(130, 76, 0x001F);
		puts("Draw points test");
		tft.line(200, 100, 300, 200, 0xF800);
		tft.line(300, 210, 200, 110, 0x001F);
		tft.line(200, 200, 300, 100, 0xF800);
		tft.line(300, 110, 200, 210, 0x001F);

		tft.line(100, 100, 300, 200, 0xF800);
		tft.line(300, 210, 100, 110, 0x001F);
		tft.line(100, 200, 300, 100, 0xF800);
		tft.line(300, 110, 100, 210, 0x001F);

		tft.line(200, 0, 300, 200, 0xF800);
		tft.line(300, 210, 200, 10, 0x001F);
		tft.line(200, 200, 300, 0, 0xF800);
		tft.line(300, 10, 200, 210, 0x001F);

		tft.line(100, 150, 300, 150, 0xF800);
		tft.line(300, 160, 100, 160, 0x001F);
		tft.line(250, 0, 250, 200, 0xF800);
		tft.line(260, 200, 260, 0, 0x001F);
		puts("Draw lines test");

	#ifdef TFT_VERTICALSCROLLING
		tft.setVerticalScrollingArea(64, 32);
		uint16_t max = 320 - 32;
		uint16_t v = 64;
	loop:
		tft.setVerticalScrolling(v);
		if (max == ++v)
			v = 64;
		_delay_ms(10);
	#else
	loop:
	#endif
		goto loop;
		goto start;

		return 1;
}
