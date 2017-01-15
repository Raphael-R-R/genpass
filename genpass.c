/*
 * genpass: A simple password generator.
 * 
 * Author: Raphaël RAGOOMUNDUN
 * License: MIT
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <regex.h>

#define DEFAULT_LENGTH 6
#define MAX_LENGTH 32000

#define ASCII_DIGIT_MIN ((int)'0')
#define ASCII_DIGIT_MAX ((int)'9')
#define ASCII_LETTER_UCASE_MIN ((int)'A')
#define ASCII_LETTER_UCASE_MAX ((int)'Z')
#define ASCII_LETTER_LCASE_MIN ((int)'a')
#define ASCII_LETTER_LCASE_MAX ((int)'z')
#define ASCII_SPECAR_1_MIN ((int)'!')
#define ASCII_SPECAR_1_MAX ((int)'/')
#define ASCII_SPECAR_2_MIN ((int)':')
#define ASCII_SPECAR_2_MAX ((int)'@')
#define ASCII_SPECAR_3_MIN ((int)'[')
#define ASCII_SPECAR_3_MAX ((int)'`')
#define ASCII_SPECAR_4_MIN ((int)'{')
#define ASCII_SPECAR_4_MAX ((int)'~')

#define SPECAR_1 0
#define SPECAR_2 1
#define SPECAR_3 2
#define SPECAR_4 3

#define NB_SPECAR_RANGE (SPECAR_4 + 1)

#define OPTION_NUMBERS 0
#define OPTION_LETTERS 1
#define OPTION_LCASE_LETTER 2
#define OPTION_UCASE_LETTER 3
#define OPTION_SPECAR 4

#define NB_OPTIONS (OPTION_SPECAR + 1)

char options[NB_OPTIONS] = {1, 1, 1, 1, 0};
int length = DEFAULT_LENGTH;

/*
 * usage: Display program usage
 * 
 * @progName: first argument on the command line, the software name
 */
 
void usage(char *progName)
{
	fprintf(stderr, "Usage: %s [-h|[[-nl|[-nlc|-nup]] [-nn] [-s] [\E[4mlength\E[24m]]]\n", progName);
	fprintf(stderr, "Generate a random password.\n\n");
	fprintf(stderr, "Options:\n");
	fprintf(stderr, "  -h     : display this help\n"
				    "  -nn    : no digit\n"
				    "  -nl    : no letter\n"
				    "  -nlc   : no lower case letter\n"
				    "  -nup   : no upper case letter\n"
				    "  -s     : add specials characters\n"
				    "  length : password's length between 1 and %d\n\n", MAX_LENGTH);
	fprintf(stderr, "Note : options -nn and -nl cannot be used together if option -s is not specify.\n");

	exit(EXIT_FAILURE);
}

/*
 * argSearch: Search for an argument
 * 
 * @argc: the number of arguments
 * @argv: an array which contains a list of arguments
 * @arg: the searched argument
 * 
 * Return: > 0 on success, 0 on failure
 */
 
int argSearch(int argc, char *argv[], char *arg)
{
	int i;
	regex_t regex;

	for(i = 1; i < argc; i++)
	{
		if(regcomp(&regex, arg, 0))
		{
			fprintf(stderr, "Cannot compile regex : %s\n", arg);
			exit(EXIT_FAILURE);
		}

		if(!regexec(&regex, argv[i], 0, NULL, 0))
			return i;
	}

	return 0;
}

/*
 * argProcess: Manage command line arguments
 * 
 * @argc: the number of arguments
 * @argv: an array which contains a list of arguments
 */
 
void argProcess(int argc, char *argv[])
{
	int lengthTest;

	options[OPTION_NUMBERS] = !argSearch(argc, argv, "^-nn$");
	options[OPTION_LETTERS] = !argSearch(argc, argv, "^-nl$");
	options[OPTION_LCASE_LETTER] = !argSearch(argc, argv, "^-nlc$");
	options[OPTION_UCASE_LETTER] = !argSearch(argc, argv, "^-nup$");
	options[OPTION_SPECAR] = argSearch(argc, argv, "^-s$");

	if((lengthTest = argSearch(argc, argv, "^[1-9][0-9]*$")))
		length = atoi(argv[lengthTest]);

	if(!options[OPTION_LETTERS])
	{
		options[OPTION_LCASE_LETTER] = 0;
		options[OPTION_UCASE_LETTER] = 0;
	}

	if(argSearch(argc, argv, "^-h$") || 
	  (!options[OPTION_NUMBERS] && !options[OPTION_LETTERS] && !options[OPTION_SPECAR]) || 
	  (options[OPTION_LETTERS] && !options[OPTION_LCASE_LETTER] && !options[OPTION_UCASE_LETTER]) || 
	  length > MAX_LENGTH)
		usage(argv[0]);
}

/*
 * genGigit: Return a random digit
 */
 
char genDigit(void)
{
	return (rand() % (ASCII_DIGIT_MAX - ASCII_DIGIT_MIN + 1) + ASCII_DIGIT_MIN);
}

/*
 * genUCLetter: Return a random upper case letter
 */
 
char genUCLetter(void)
{
	return (rand() % (ASCII_LETTER_UCASE_MAX - ASCII_LETTER_UCASE_MIN + 1) + ASCII_LETTER_UCASE_MIN);
}

/*
 * genLCLetter: Return a random lower case letter
 */
 
char genLCLetter(void)
{
	return (rand() % (ASCII_LETTER_LCASE_MAX - ASCII_LETTER_LCASE_MIN + 1) + ASCII_LETTER_LCASE_MIN);
}

/*
 * genSCharacter: Return a random special character
 */
 
char genScharacter(void)
{
	int r = rand() % NB_SPECAR_RANGE;
	int max, min;

	switch(r)
	{
		case SPECAR_1:
			min = ASCII_SPECAR_1_MIN;
			max = ASCII_SPECAR_1_MAX;
			break;
		case SPECAR_2:
			min = ASCII_SPECAR_2_MIN;
			max = ASCII_SPECAR_2_MAX;
			break;
		case SPECAR_3:
			min = ASCII_SPECAR_3_MIN;
			max = ASCII_SPECAR_3_MAX;
			break;
		case SPECAR_4:
			min = ASCII_SPECAR_4_MIN;
			max = ASCII_SPECAR_4_MAX;
			break;
	}

	return (rand() % (max - min + 1) + min);
}

/*
 * gen : Generate a random password
 */
 
void gen(void)
{
	char c; int i, r;

	for(i = 0; i < length; i++)
	{
		do
			r = rand() % NB_OPTIONS;
		while(!options[r] || r == OPTION_LETTERS);

		switch(r)
		{
			case OPTION_NUMBERS:
				c = genDigit();
				break;
			case OPTION_LCASE_LETTER:
				c = genLCLetter();
				break;
			case OPTION_UCASE_LETTER:
				c = genUCLetter();
				break;
			case OPTION_SPECAR:
				c = genScharacter();
				break;
		}

		printf("%c", c);
	}

	printf("\n");
}

int main(int argc, char *argv[])
{
	if(argc > 1)
		argProcess(argc, argv);

	srand(time(NULL));
	gen();

	return EXIT_SUCCESS;
}
