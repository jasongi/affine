#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

/*  
    Copyright (C) 2013 Jason Giancono (jasongiancono@gmail.com)

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
*/

void eea(int a, int b, int* gcd, int*xi, int* yi);
char encrypt(char plaintext, int a, int b);
char decrypt(char ciphertext, int a, int b);
int main(int argc, char* argv[])
{
	char usage[] = "affine by Jason Giancono\nUSAGE: affine <FLAG> <input filename> <output filename> <key a> <key b>\nFLAGS:\t\t-d: decrypt\n\t\t-e: encrypt\n";
	if (argc != 6)
		printf("%s", usage);
	else
	{
		int error, a, b;
		FILE *output, *input;
		char temp;
		a = atoi(argv[4]);
		b = atoi(argv[5]);
		error = 0;
		/*opening files for read/writing, check for errors*/
		input = fopen(argv[2], "r");
		if ( (input != 0) && (( output = fopen(argv[3], "w") ) != 0))
		{
			/*am I encrypting or decrypting (changes function call)*/
			if (!strcmp(argv[1], "-e"))
			{
				printf("\n-----\nEncoding\n-----\n");
				/*continue until error or EOF*/
				while ((feof(input) == 0) && (ferror(input) == 0) && (ferror(output) == 0))
				{
					/*encrypt using affine with the two keys (ignore non-letters)*/
					temp = fgetc(input);
					temp =( ( ( temp > 65 ) && (temp < 91 ) ) || ( ( temp > 96 ) && ( temp < 123 ) ) ) ? encrypt(temp, a, b) : temp;
					/*write it to file (unless it was EOF)*/
					if(feof(input) == 0)
					{
						fputc(temp, output);
					}
				}
			}
			else if(!strcmp(argv[1], "-d"))
			{
				printf("\n-----\nDecoding\n-----\n");
				/*continue until error or EOF*/
				while ((feof(input) == 0) && (ferror(input) == 0) && (ferror(output) == 0))
				{
					/*encrypt using affine with the two keys*/
					temp = fgetc(input);
					temp =( ( ( temp > 65 ) && (temp < 91 ) ) || ( ( temp > 96 ) && ( temp < 123 ) ) ) ? (decrypt(temp, a, b)) : temp;
					/*write it to file (unless it is EOF)*/
					if(feof(input) == 0)
						fputc(temp, output);
				}
			}
			else printf("%s", usage);
			printf("\n-----\nDone\n-----\n");
		}
		else 
		{
			error = 1;
			printf("%s", usage);
		}
		/*print errors*/
		if ((error == 1) || (ferror(input) != 0) || (ferror(output) != 0))
			perror("A read/write error occurred");
		/*close files (if they were open)*/
		if(output != 0) 
			fclose(output);
		if(input != 0)
			fclose(input);
	}
	return 0;
}

char encrypt(char plaintext, int a, int b)
{
	int offset = 97;
	if (plaintext < 97)
		offset = 65;
	return (( (a*(plaintext-offset)+b) % 26)+offset);
}

char decrypt(char ciphertext, int a, int b)
{
	int gcd,x,y;
	char plaintext = 0;
	int aInv = 0;
	int offset = 97;
	if (ciphertext < 97)
		offset = 65;
	/*find aInverse (aInv will be 0 if it doesn't exist)*/
	eea(26,a,&gcd,&x,&y);
	if (gcd == 1)
		aInv = (y+26)%26; /*add 26 to cancel negetives*/
	plaintext = ( (aInv*(ciphertext-offset-b+26)) % 26)+offset;
	return plaintext;
}

void eea(int a, int b, int* gcd, int*xi, int* yi)
{
	int xiPlus1, yiPlus1, xiPlus2, yiPlus2, quotient, remainder;
	*xi = 0;
	*yi = 1;
	xiPlus1 = 1;
	yiPlus1 = 0;
	*gcd = b;
	while (a!=0)
	{
		quotient = *gcd/a;
		remainder = *gcd%a;
		xiPlus2 = *xi-xiPlus1*quotient;
		yiPlus2 = *yi-yiPlus1*quotient;
		*gcd = a;
		a = remainder;
		*xi = xiPlus1;
		*yi = yiPlus1;
		xiPlus1 = xiPlus2;
		yiPlus1 = yiPlus2;
	}
}
