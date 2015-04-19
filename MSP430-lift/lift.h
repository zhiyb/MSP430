#ifndef LIFT_H
#define LIFT_H

#include <inttypes.h>

#define _BV(p)	(1 << p)

// P1, input
#define PIIN	P1IN
#define PIOUT	P1OUT
#define PIDIR	P1DIR
#define PIREN	P1REN
#define PIDS	P1DS
#define IND0	_BV(0)
#define IND1	_BV(1)
#define IND2	_BV(2)
#define EN	_BV(3)
#define DIREC	_BV(4)

// P3, output
#define POIN	P3IN
#define POOUT	P3OUT
#define PODIR	P3DIR
#define POREN	P3REN
#define PODS	P3DS
#define BOTTOM	_BV(3)
#define MMINUS	_BV(4)
#define MPLUS	_BV(5)
#define TOP	_BV(6)

class Lift
{
public:
	enum Directions {Up = 1, Down = 0};
	
	Lift(void);
	bool top(void) const;
	bool middlePlus(void) const;
	bool middleMinus(void) const;
	bool bottom(void) const;

	void setEnable(bool e) {data.enable = e;}
	void setDirection(uint8_t dir) {data.direction = dir;}
	void setIndicator(uint8_t idx, bool e) {data.indicator[idx] = e;}

	uint8_t height(void) const {return data.height;}
	bool move(void);
	bool indicator(uint8_t idx) const {return data.indicator[idx];}
	
	static const uint8_t top_ht, bottom_ht, mid_p_ht, mid_m_ht;
	static const uint8_t lift_ht;

private:
	struct {
		bool enable, indicator[3];
		uint8_t height, direction;
	} data;
};

#endif
