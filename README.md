# genpass
A simple password generator

	Usage: ./a.out [-h|[[-nl|[-nlc|-nup]] [-nn] [-s] [length]]]
	Generate a random password.

	Options:
	  -h     : display this help
	  -nn    : no digit
	  -nl    : no letter
	  -nlc   : no lower case letter
	  -nup   : no upper case letter
	  -s     : add specials characters
	  length : password's length between 1 and 32000

	Note : options -nn and -nl cannot be used together if option -s is not specify.
	
### Exemples

	$ ./genpass -s 24
	2Y0_J}Wrc5n2q~LC4HRV1Viv

	$ ./genpass -nup -s
	,o#m!

	$ ./genpass
	77wBZw
