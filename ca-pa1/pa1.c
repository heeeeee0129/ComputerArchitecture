/**********************************************************************
 * Copyright (c) 2021
 *  Sang-Hoon Kim <sanghoonkim@ajou.ac.kr>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTIABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 **********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

/* To avoid security error on Visual Studio */
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)

/*====================================================================*/
/*          ****** DO NOT MODIFY ANYTHING FROM THIS LINE ******       */
#define MAX_NR_TOKENS	32	/* Maximum length of tokens in a command */
#define MAX_TOKEN_LEN	64	/* Maximum length of single token */
#define MAX_ASSEMBLY	256 /* Maximum length of assembly string */

typedef unsigned char bool;
#define true	1
#define false	0
/*          ****** DO NOT MODIFY ANYTHING UP TO THIS LINE ******      */
/*====================================================================*/


/***********************************************************************
 * translate()
 *
 * DESCRIPTION
 *   Translate assembly represented in @tokens[] into a MIPS instruction.
 *   This translate should support following 13 assembly commands
 *
 *    - add
 *    - addi
 *    - sub
 *    - and
 *    - andi
 *    - or
 *    - ori
 *    - nor
 *    - lw
 *    - sw
 *    - sll
 *    - srl
 *    - sra
 *
 * RETURN VALUE
 *   Return a 32-bit MIPS instruction
 *
 */
static char* registTranslation(char* tokens){
	char* str = {'\0'};
	if(strncmp(tokens, "zero", 4)==0){
		str =  "00000";
	}
	else if(strncmp(tokens, "at", 2)==0){
		str =  "00001";
	}
	else if(strncmp(tokens, "v0", 2)==0){
		str =  "00010";
	}
	else if(strncmp(tokens, "v1", 2)==0){
		str =  "00011";
	}
	else if(strncmp(tokens, "a0", 2)==0){
		str =  "00100";
	}
	else if(strncmp(tokens, "a1", 2)==0){
		str =  "00101";
	}
	else if(strncmp(tokens, "a2", 2)==0){
		str =  "00110";
	}
	else if(strncmp(tokens, "a3", 2)==0){
		str =  "00111";
	}
	else if(strncmp(tokens, "t0", 2)==0){
		str =  "01000";
	}
	else if(strncmp(tokens, "t1", 2)==0){
		str =  "01001";
	}
	else if(strncmp(tokens, "t2", 2)==0){
		str =  "01010";
	}
	else if(strncmp(tokens, "t3", 2)==0){
		str =  "01011";
	}
	else if(strncmp(tokens, "t4", 2)==0){
		str =  "01100";
	}
	else if(strncmp(tokens, "t5", 2)==0){
		str =  "01101";
	}
	else if(strncmp(tokens, "t6", 2)==0){
		str =  "01110";
	}
	else if(strncmp(tokens, "t7", 2)==0){
		str =  "01111";
	}
	else if(strncmp(tokens, "s0", 2)==0){
		str =  "10000";
	}
	else if(strncmp(tokens, "s1", 2)==0){
		str =  "10001";
	}
	else if(strncmp(tokens, "s2", 2)==0){
		str =  "10010";
	}
	else if(strncmp(tokens, "s3", 2)==0){
		str =  "10011";
	}
	else if(strncmp(tokens, "s4", 2)==0){
		str =  "10100";
	}
	else if(strncmp(tokens, "s5", 2)==0){
		str =  "10101";
	}
	else if(strncmp(tokens, "s6", 2)==0){
		str =  "10110";
	}
	else if(strncmp(tokens, "s7", 2)==0){
		str =  "10111";
	}
	else if(strncmp(tokens, "t8", 2)==0){
		str =  "11000";
	}
	else if(strncmp(tokens, "t9", 2)==0){
		str =  "11001";
	}
	else if(strncmp(tokens, "k1", 2)==0){
		str =  "11010";
	}
	else if(strncmp(tokens, "k2", 2)==0){
		str =  "11011";
	}
	else if(strncmp(tokens, "gp", 2)==0){
		str =  "11100";
	}
	else if(strncmp(tokens, "sp", 2)==0){
		str =  "11101";
	}
	else if(strncmp(tokens, "fp", 2)==0){
		str =  "11110";
	}
	else if(strncmp(tokens, "ra", strlen("ra"))==0){
		str =  "11111";
	}
 return str;
}



static char* numberTranslation(char* tokens, int bitLength){
	//fprintf(stderr, "함수 들어옴\n");
	int num;
	bool complement = false;

	if(strncmp(tokens, "-0x", 3)==0){
		num = strtol(tokens+1, NULL, 16);
		//fprintf(stderr, "넘버 ㄱㅄ2 : %d\n", num);
		complement = true;//num = num * (-1);
		//fprintf(stderr, "넘버 ㄱㅄ2 : %d\n", num);
	}
	else if(strncmp(tokens, "0x", 2)==0){
		num = strtol(tokens,NULL,16);
		//fprintf(stderr, "넘버 값 1: %d\n", num);
	} else{
		num = strtol(tokens, NULL, 10);
		if(num<0) complement = true;
		//fprintf(stderr, "넘버 값: %d\n", num);
	}
	

	//fprintf(stderr, "boolean : %d \n",complement);

	char* temp = (char*)malloc(bitLength+1);
	int index = bitLength-1;
	while(num > 0 && index >= 0){
		if(num % 2 == 1){
			temp[index] = '1';
		} else{
			temp[index] = '0';
		}
		index--;
		num /= 2;
	}
	//fprintf(stderr, "이진수 계산  : %s\n", temp);/// 일단 양수로 생각하고 2진수로 변환,
	for(index; index >= 0; index--){
		temp[index] = '0';
	}
	//fprintf(stderr, "빈자리 채움 : %s\n" ,temp);
	if(complement == true){

		//fprintf(stderr, "---2의 보수 시작\n");
		for(int i=0; i<bitLength; i++){
			if(temp[i]=='0') temp[i] = '1';
			else temp[i] = '0';
		}
		//fprintf(stderr, "---토글 완료 : %s\n", temp);
		for(int i=(bitLength-1); i>=0; i--){
		if(temp[i]=='1')temp[i]='0';
		else{
			temp[i]='1';
			break;
		}}

		//fprintf(stderr, "---1 더함-->> %s\n", temp);
	
	temp[bitLength] = '\0';
	//fprintf(stderr, "arr : 1 : %s\n", temp);
	return temp;
	}
		
	
	temp[bitLength] = '\0';	
	return temp;
} 

static unsigned int translate(int nr_tokens, char *tokens[])
{
	char istr[MAX_TOKEN_LEN] = {'\0',};
//printf(stderr, "함수 들어옹 %s\n", tokens[0]);


	if(strncmp(tokens[0], "addi",4)==0){
		strcpy(istr,"001000");
		//fprintf(stderr, "성공\n");
		strcat(istr, registTranslation(tokens[2]));
		//fprintf(stderr, "성공\n");
		strcat(istr, registTranslation(tokens[1]));
		//fprintf(stderr, "성공\n");
		strcat(istr, numberTranslation(tokens[3], 16));
		//fprintf(stderr, "성공\n");
		return strtol(istr, NULL, 2);
	}
	else if(strncmp(tokens[0], "andi",4)==0){
		strcpy(istr,"001100");
		strcat(istr, registTranslation(tokens[2]));
		strcat(istr, registTranslation(tokens[1]));
		strcat(istr, numberTranslation(tokens[3], 16));
		return strtol(istr, NULL, 2);
	}
	else if(strncmp(tokens[0], "ori", 3)==0){
		strcpy(istr,"001101");
		strcat(istr, registTranslation(tokens[2]));
		strcat(istr, registTranslation(tokens[1]));
		strcat(istr, numberTranslation(tokens[3], 16));
		return strtol(istr, NULL, 2);
	}
	else if(strncmp(tokens[0], "add", 3)==0){
		strcpy(istr,"000000");
		strcat(istr, registTranslation(tokens[2]));
		strcat(istr, registTranslation(tokens[3]));
		strcat(istr, registTranslation(tokens[1]));
 		strcat(istr, "00000100000");
		return strtol(istr, NULL, 2);
	}
	else if(strncmp(tokens[0], "sub", 3)==0){
		strcpy(istr,"000000");
		strcat(istr, registTranslation(tokens[2]));
		strcat(istr, registTranslation(tokens[3]));
		strcat(istr, registTranslation(tokens[1]));
 		strcat(istr, "00000100010");
		return strtol(istr, NULL, 2);
	}
	else if(strncmp(tokens[0], "and", 3)==0){
		strcpy(istr,"000000");
		strcat(istr, registTranslation(tokens[2]));
		strcat(istr, registTranslation(tokens[3]));
		strcat(istr, registTranslation(tokens[1]));
 		strcat(istr, "00000100100");
		return strtol(istr, NULL, 2);		
	}
	else if(strncmp(tokens[0], "or", 2)==0){
		strcpy(istr,"000000");
		strcat(istr, registTranslation(tokens[2]));
		strcat(istr, registTranslation(tokens[3]));
		strcat(istr, registTranslation(tokens[1]));
 		strcat(istr, "00000100101");
		return strtol(istr, NULL, 2);		
	}
	else if(strncmp(tokens[0], "nor", 3)==0){
		strcpy(istr,"000000");
		strcat(istr, registTranslation(tokens[2]));
		strcat(istr, registTranslation(tokens[3]));
		strcat(istr, registTranslation(tokens[1]));
 		strcat(istr, "00000100111");
		return strtol(istr, NULL, 2);			
	}
	else if(strncmp(tokens[0], "sll", 3)==0){
		strcpy(istr,"00000000000");
		strcat(istr, registTranslation(tokens[2]));
		strcat(istr, registTranslation(tokens[1]));
		strcat(istr, numberTranslation(tokens[3],5));
		strcat(istr, "000000");
		return strtol(istr, NULL, 2);	
	}
	else if(strncmp(tokens[0], "srl", 3)==0){
		strcpy(istr,"00000000000");
		strcat(istr, registTranslation(tokens[2]));
		strcat(istr, registTranslation(tokens[1]));
		strcat(istr, numberTranslation(tokens[3], 5));
		strcat(istr, "000010");
		return strtol(istr, NULL, 2);	
	}
	else if(strncmp(tokens[0], "sra", 3)==0){
		strcpy(istr,"00000000000");
		strcat(istr, registTranslation(tokens[2]));
		strcat(istr, registTranslation(tokens[1]));
		strcat(istr, numberTranslation(tokens[3], 5));
		strcat(istr, "000011");
		return strtol(istr, NULL, 2);			
	}
	else if(strncmp(tokens[0], "lw", 2)==0){
		strcpy(istr,"100011");
		strcat(istr, registTranslation(tokens[2]));
		strcat(istr, registTranslation(tokens[1]));
		strcat(istr, numberTranslation(tokens[3], 16));
		return strtol(istr, NULL, 2);
	}
	else if(strncmp(tokens[0], "sw", 2)==0){
		strcpy(istr,"101011");
		strcat(istr, registTranslation(tokens[2]));
		strcat(istr, registTranslation(tokens[1]));
		strcat(istr, numberTranslation(tokens[3], 16));
		return strtol(istr, NULL, 2);
	}
	else{return 0xFFFFFFFF; }

}


/***********************************************************************
 * parse_command()
 *
 * DESCRIPTION
 *   Parse @assembly, and put each assembly token into @tokens[] and the number of
 *   tokes into @nr_tokens. You may use this implemention or your own from PA0.
 *
 *   A assembly token is defined as a string without any whitespace (i.e., *space*
 *   and *tab* in this programming assignment). For exmaple,
 *     command = "  add t1   t2 s0 "
 *
 *   then, nr_tokens = 4, and tokens is
 *     tokens[0] = "add"
 *     tokens[1] = "t0"
 *     tokens[2] = "t1"
 *     tokens[3] = "s0"
 *
 *   You can assume that the input string is all lowercase for testing.
 *
 * RETURN VALUE
 *   Return 0 after filling in @nr_tokens and @tokens[] properly
 *
 */
static bool __is_separator(char *c)
{
	char *separators = " \t\r\n,.";

	for (size_t i = 0; i < strlen(separators); i++) {
		if (*c == separators[i]) return true;	
	}

	return false;
}
static int parse_command(char *assembly, int *nr_tokens, char *tokens[])
{
	char *curr = assembly;
	int token_started = false;
	*nr_tokens = 0;

	while (*curr != '\0') {  
		if (__is_separator(curr)) {  
			*curr = '\0';
			token_started = false;
		} else {
			if (!token_started) {
				tokens[*nr_tokens] = curr;
				*nr_tokens += 1;
				token_started = true;
			}
		}
		curr++;
	}

	return 0;
}



/*====================================================================*/
/*          ****** DO NOT MODIFY ANYTHING FROM THIS LINE ******       */

/***********************************************************************
 * The main function of this program.
 */
int main(int argc, char * const argv[])
{
	char assembly[MAX_ASSEMBLY] = { '\0' };
	FILE *input = stdin;

	if (argc > 1) {
		input = fopen(argv[1], "r");
		if (!input) {
			fprintf(stderr, "No input file %s\n", argv[0]);
			return EXIT_FAILURE;
		}
	}

	if (input == stdin) {
		printf("*********************************************************\n");
		printf("*          >> SCE212 MIPS translator  v0.01 <<          *\n");
		printf("*                                                       *\n");
		printf("*                                       .---.           *\n");
		printf("*                           .--------.  |___|           *\n");
		printf("*                           |.------.|  |=. |           *\n");
		printf("*                           || >>_  ||  |-- |           *\n");
		printf("*                           |'------'|  |   |           *\n");
		printf("*                           ')______('~~|___|           *\n");
		printf("*                                                       *\n");
		printf("*                                   Fall 2021           *\n");
		printf("*********************************************************\n\n");
		printf(">> ");
	}

	while (fgets(assembly, sizeof(assembly), input)) {
		char *tokens[MAX_NR_TOKENS] = { NULL };
		int nr_tokens = 0;
		unsigned int machine_code;

		for (size_t i = 0; i < strlen(assembly); i++) {
			assembly[i] = tolower(assembly[i]);
		}

		if (parse_command(assembly, &nr_tokens, tokens) < 0)
			continue;

		machine_code = translate(nr_tokens, tokens);

		fprintf(stderr, "0x%08x\n", machine_code);

		if (input == stdin) printf(">> ");
	}

	if (input != stdin) fclose(input);

	return EXIT_SUCCESS;
}
