/*
 * binary_functions.c
 *
 *  Created on: Jun 18, 2016
 *      Author: flo
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "binary_calc.h"


long exp_long(int base, unsigned int potenz){
	if(base == 0 && potenz == 0){
		return -1;
	}
	int i;
	long result = 1;
	for(i=0;i<potenz;i++){
		result = result * base;
	}
	return result;
}
//shifting long values
long bit_shift(long origin, long shift){
	long result;
	if(shift < 0){
		shift = shift - (2 * shift);
		result = origin / exp_long(2,shift);
	}else{
		result = origin * exp_long(2,shift);
	}
	return result;
}
//returning of relevent value of hex number (deleting unnecessary tail)
char * access_bits(int offset_size, int size, char segment_descr[size]){
	int length_bits = 0;
	long bit_mask;
	long lowest_addr;
	long segment_size;
	char * buffer_str = malloc(sizeof(char)*size);
	if(buffer_str == 0){
		return -1;
	}
	segment_size= strtol(segment_descr,NULL,16);
	length_bits = strlen(segment_descr) * 4; //hex quantity to bits
	//creating full mask for example : F = 1111
	//creating mask for offset: offsetsize = 12 -> mask = F-3 = 1100
	long first = (exp_long(2,length_bits)-1);
	long second = (exp_long(2,length_bits-offset_size)-1);
	bit_mask = first -second;
	lowest_addr = segment_size&bit_mask;
	sprintf(buffer_str, "%x",lowest_addr);//int -> hex
	return buffer_str;
}
//returning first valid physical address
char * create_phyadr(int shift, int offset_size, int size,char v_adr[size]){
	if(v_adr < 1 || size < 1 || offset_size < 0){
		return -1;
	}
	int shift_diff;
	//shift = 12;
	//int offset_size = 12; // 12 bit 3 byte example
	char * first_addr;
	if((first_addr = access_bits(offset_size,size,v_adr))==0){
		return (-2);
	}
	shift_diff = shift - (size*4 - offset_size);
	sprintf(first_addr, "%x",bit_shift(strtol(first_addr,NULL,16),shift_diff));
	return first_addr;
}
