/*
 * binary_calc.h
 *
 *  Created on: Jun 18, 2016
 *      Author: flo
 */

#ifndef BINARY_CALC_H_
#define BINARY_CALC_H_

long exp_long(int base, unsigned int potenz);
long bit_shift(long origin, long shift);
char * segment_calc(int offset_size, int size, char segment_descr[size]);
char * create_phyadr(int shift, int offset_size, int size,char v_adr[size]);

#endif /* BINARY_CALC_H_ */
