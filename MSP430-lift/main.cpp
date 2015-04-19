#include <stdio.h>
#include "tft.h"
#include "colours.h"
#include "rgbconv.h"
#include "lift.h"

#define OFFSET	30

using namespace conv;
using namespace colours::b16;

tft_t tft;

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

	PIDIR = 0x00;
	PIDS = 0x00;
	PIOUT = 0xFF;
	PIREN = 0xFF;

	PODIR = 0xFF;
	PODS = 0xFF;
	POOUT = 0xFF;
	POREN = 0x00;

	tft.init();
	tft.setOrient(tft.Portrait);
	tft.setBackground(0x0000);
	tft.setForeground(0x667F);

	tft.setBGLight(true);
	tft.clean();
}

void draw(Lift& l)
{
	uint16_t y = OFFSET;
	tft.setX(0);
	tft.setY(y);
	tft.setForeground(c32to16(0x7F7F7F));
	tft.putString("TOP");
	tft.rectangle(80, y, 40, 2 * l.lift_ht, c32to16(0xFFFFFF));
	y += (l.top_ht - l.mid_p_ht) * 2;
	tft.setX(0);
	tft.setY(y);
	tft.setForeground(c32to16(0x7F7F7F));
	tft.putString("MIDDLE");
	tft.rectangle(80, y, 40, 2 * l.lift_ht, c32to16(0xFFFFFF));
	y += (l.mid_p_ht - l.bottom_ht) * 2 - l.lift_ht * 2;
	tft.setX(0);
	tft.setY(y);
	tft.setForeground(c32to16(0x7F7F7F));
	tft.putString("BOTTOM");
	tft.rectangle(80, y, 40, 2 * l.lift_ht, c32to16(0xFFFFFF));

	/*static const uint16_t colours[7] = {White, Magenta, Cyan, Yellow, Blue, Green, Red};
	for (uint8_t i = 0; i < 7; i++)
		tft.rectangle(tft.width() - 8, tft.height() * i / 7, 8, tft.height() / 7, colours[i]);*/
}

void update(Lift& l)
{
	uint16_t y = OFFSET + (l.top_ht - l.lift_ht - l.height()) * 2;
	uint16_t ly = y;
	tft.rectangle(130, OFFSET, 20, y - OFFSET, c32to16(0x3F3F3F));
	tft.rectangle(130, y + l.lift_ht * 2, 20, OFFSET + l.top_ht * 2 - y - l.lift_ht * 2, c32to16(0x3F3F3F));

	y = OFFSET;
	tft.rectangle(160, y, 20, l.lift_ht * 2, c32to16(l.indicator(2) ? 0xFFFF00 : 0x0000FF));
	tft.rectangle(190, y, 20, l.lift_ht * 2, c32to16(l.top() ? 0x00FF00 : 0xFF0000));
	y += (l.top_ht - l.mid_p_ht) * 2;
	tft.rectangle(160, y, 20, l.lift_ht * 2, c32to16(l.indicator(1) ? 0xFFFF00 : 0x0000FF));
	tft.rectangle(190, y, 20, l.lift_ht, c32to16(l.middlePlus() ? 0x00FF00 : 0xFF0000));
	tft.rectangle(190, y + l.lift_ht, 20, l.lift_ht, c32to16(l.middleMinus() ? 0x00FF00 : 0xFF0000));
	y += (l.mid_p_ht - l.bottom_ht) * 2 - l.lift_ht * 2;
	tft.rectangle(160, y, 20, l.lift_ht * 2, c32to16(l.indicator(0) ? 0xFFFF00 : 0x0000FF));
	tft.rectangle(190, y, 20, l.lift_ht * 2, c32to16(l.bottom() ? 0x00FF00 : 0xFF0000));
	
	bool m = l.move();
	tft.rectangle(130, ly, 20, l.lift_ht * 2, c32to16(m ? 0x00FF00 : 0xFF0000));
}

void set(Lift &l)
{
	if (!l.top())
		POOUT |= TOP;
	else
		POOUT &= ~TOP;
	if (!l.middlePlus())
		POOUT |= MPLUS;
	else
		POOUT &= ~MPLUS;
	if (!l.middleMinus())
		POOUT |= MMINUS;
	else
		POOUT &= ~MMINUS;
	if (!l.bottom())
		POOUT |= BOTTOM;
	else
		POOUT &= ~BOTTOM;
	__delay_cycles(MCLK / 1000);
	l.setEnable(!(PIIN & EN));
	l.setDirection(PIIN & DIREC);
	l.setIndicator(0, !(PIIN & IND0));
	l.setIndicator(1, !(PIIN & IND1));
	l.setIndicator(2, !(PIIN & IND2));
}

int main(void)
{
	init();
	Lift l;

	tft.clean();
	tft.setZoom(2);
	tft.putString("*** Lift module ***");
	draw(l);

start:
	update(l);
	set(l);
	__delay_cycles(MCLK * 50 / 1000);
	goto start;

	return 1;
}
