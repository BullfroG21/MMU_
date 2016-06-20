/*
 ============================================================================
 Name        : logphysadr.c
 Author      : Gwechenberger Florian
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

/*******************************************************************
 * Next step:
 * 	*	ERROR -> using Headers
 * 	*	completing function
 * 	* 	exporting function
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error_return.h"
#include "binary_calc.h"

struct tokenizer_input{
	char * filename;
	char * address;
	int verbose;		//show calculation process
	int segment;				//segment based address translation
	int page;				//page based address translation
}typedef TOKEN_INPUT;

int get_value(FILE * address_table, int size, char value_type[size]){
	if(address_table <= 0){
		return (-1);
	}
	if(size <= 0){
		return (-2);
	}
	if(value_type <= 0){
		return (-3);
	}
	fseek(address_table, 0L, SEEK_SET);
	char * indicator_offset;
	char tmp_string[100];
	int return_value;
	char * buffer_str = malloc(sizeof(char)*128);
	do{
		if(fgets(buffer_str,100,address_table)==0){
			return ERROR_READING_FGETS;
		}
	}while(strncmp(buffer_str,value_type,size)!=0);
	if((indicator_offset=strchr(buffer_str,'='))==0){
		return ERROR_READING_VAR;
	}
	if(strcpy(tmp_string,indicator_offset +1)==0){
		return ERROR_STRCPY;
	}
	return_value = strtol(tmp_string,NULL,16);
	free(buffer_str);
	return return_value;
}
int analyse_input(TOKEN_INPUT * input, int argc, char * argv[]){
	if(input < 1 || argv < 2){
		return -1;
	}
	int counter;
	for(counter = 1; counter < argc; counter++){
		if(strcmp("-t", argv[counter])==0){
			input->filename = malloc(sizeof(argv[counter+1]));
			if(input->filename == 0){
				return ERROR_ALOCATING;
			}
			if(strcpy(input->filename,argv[counter +1])==0){
				return ERROR_STRCPY;
			}
			counter++;
		}else if(strcmp("-a", argv[counter])==0){
			if(argc == (counter+1)){
				return (-3);
			}
			input->address = malloc(sizeof(argv[counter+1]));
			if(input->address == 0){
				return ERROR_ALOCATING;
			}
			printf("%i %i",counter,argc);
			if(strcpy(input->address,argv[counter +1])==0){
				return ERROR_STRCPY;
			}
			counter++;
		}else if(strcmp("-s", argv[counter])==0){
			input->segment = 1;
		}else if(strcmp("-p", argv[counter])==0){
			input->page = 1;
		}else if(strcmp("-v", argv[counter])==0){
			input->verbose = 1;
		}else if((strcmp("-h", argv[counter])==0) || (argc < 2)){
			return HELP_INFORMATION;
		}
		else{
			printf("invalid_input");
			return INVALID_INPUT;
		}
	}
}
int replace_char(char * origin, char to_delete){
	char * char_pos;
	printf("%s",origin);
	while((char_pos = strchr(origin,to_delete)) > 0){
		strcpy(char_pos,char_pos+1);
	}
	return 1;
}

char * get_segement_descr(int size, char buffer_str[size], int length,char segm_nr[length], char limit){
	char * char_pos;
	char * segment_descr;
	if((char_pos = strchr(buffer_str,limit)) > 0){
		if(strncmp(segm_nr,buffer_str,((int)char_pos)-((int)buffer_str))==0){
			buffer_str = char_pos +1;
			if((char_pos = strchr(buffer_str,limit)) > 0){
				segment_descr = malloc(sizeof(char)* (((int)char_pos)-((int)buffer_str)) );
				if(strncpy(segment_descr,buffer_str,((int)char_pos)-((int)buffer_str))>0){
				//	printf("-----%s %i<=%i",segment_descr, strlen(segment_descr),(((int)char_pos)-((int)buffer_str)));
					return segment_descr;
				}
				else{
					return (-3);
				}
			}
			//buffer_str[strlen(buffer_str)-1] = '\0';
		}
		else{
			return (-2);
		}
	}
	else{
		return (-1);
	}
}

char * segm_str;
char * addr_info;
char * shift;
char * home_dir;
char splitter;
int constuctor(){
	segm_str = malloc(sizeof(char)*strlen("offset_length="));
	addr_info = malloc(sizeof(char)*strlen("addr_info="));
	shift = malloc(sizeof(char)*strlen("bit_shift="));
	home_dir = malloc(sizeof(char)*strlen("/home/flo/workspace/logphysadr/Debug/")+50);
	if((segm_str == 0) || (addr_info == 0) || (shift == 0)){
		return ERROR_ALOCATING;
	}
	strcpy(segm_str,"offset_length=");
	strcpy(addr_info,"addr_info=");
	strcpy(shift,"bit_shift=");
	strcpy(home_dir,"/home/flo/workspace/logphysadr/Debug/");
	splitter = ';';
	return 0;
}
//-h -p/-s -t -a -v
int main(int argc, char * argv[]) {
	char segm_nr[10];
	long int segm_nr_number;
	long offset;
	char * offset_str;
	offset_str = malloc(sizeof(char)*100);
	constuctor();
	TOKEN_INPUT * input = malloc(sizeof(TOKEN_INPUT));
	analyse_input(input, argc, argv);
	if(input->address < 1){
		return (-1);
	}
	int fitting = 0; //true then correct line is selected
	int offset_size;
	int segm_addr_size;
	int shift_size;
	char * buffer_str = malloc(sizeof(char)*100);
	char * segment_descr = malloc(sizeof(char)*64);
	FILE * address_table;
	//input->filename
	strcat(home_dir,input->filename);

	address_table = fopen(home_dir,"r");
	if(address_table == 0){
		free(input->address); free(input->filename); free(input);
		return ERROR_FOPEN;
	}
	if( ( offset_size = get_value(address_table,strlen(segm_str),segm_str))<0 ){
		printf("ERROR");
		return ERROR_GETVALUE;
	}
	//quantity of bits which get used for the first physicle address
	if((segm_addr_size = get_value(address_table,strlen(addr_info),addr_info))<0){
		printf("ERROR");
		return ERROR_GETVALUE;
	}
	if((shift_size = get_value(address_table,strlen(shift),shift))<0){
		printf("ERROR");
		return ERROR_GETVALUE;
	}
	//create_phyadr(shift_size,offset_size, strlen(input->address),input->address);
	char bit_string[50];
	strcpy(bit_string,access_bits(offset_size,strlen(input->address),input->address));
	// bit_string = first address
	printf("\n%s\n",bit_string);
	// first address in long int
	long int bits = strtol(access_bits(offset_size,strlen(input->address),input->address),NULL,16);
	segm_nr_number = bit_shift(bits,(-1)*(strlen(input->address)*4-offset_size));
	sprintf(segm_nr, "%x", segm_nr_number);
	offset = strtol(input->address,NULL,16) - bits;
	sprintf(offset_str, "%x", offset);
	printf("-%s-",segm_nr);
	fseek(address_table, 0L, SEEK_SET);
	char * table_value;
	while((fitting ==0) && (fgets(buffer_str,100,address_table) != 0)){
		if(0 < (int)(table_value = get_segement_descr(strlen(buffer_str),buffer_str,strlen(segm_nr),segm_nr,splitter))){
			printf("\n%s\n",table_value);
			replace_char(table_value,':');
			printf("\n%s\n",table_value);
		}
	}

	return EXIT_SUCCESS;
}


//printf("%i",strtol("B",buffer_str,16));//hex -> int
//sprintf(buffer_str, "%x", var);//int -> hex
//printf("#%s#",buffer_str);
