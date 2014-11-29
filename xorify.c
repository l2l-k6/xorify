//
// xorify.c: calculate bitwise XOR of two files
//
// Copyright (C) 2014 Leonid Chaichenets
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation; either version 2 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
// 02111-1307, USA.
//
// Authors:
//      Leonid Chaichenets <leonid.chaichenets@googlemail.com>
//

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <string.h>
#include <argp.h>

/* Argument parser setup -- START */
// program version:
const char *argp_program_version = "xorify 1.0";

// contact address:
const char * argp_program_bug_address =
	"Leonid Chaichenets <leonid.chaichenets@googlemail.com>";

// program description:
const char doc[] =
	"xorify -- A program calculating bitwise XOR of two given files.\v"
	"Principal use of this program is to higlight differences in binary "
	"files.\n\n";

// non-option arguments description:
const char args_doc[] = "IN_FILE_1 IN_FILE_2";

// communication between parse_opt() and main():
struct arguments {
	int i;					/* number of command line arguments processed */
	int verbosity;				/* verbosity option */
	char * fn_in_0, fn_in_1, fn_out;	/* input and output file names */
};

// option arguments we understand:
const struct argp_option options[] = {
	{"verbose", 'v', 0, 0, "Produce verbose output"},
	{"output", 'o',  "fn_out",  , "Output file name (if omitted stdout is used)"},
	{0}
};

// argument parser logics:
error_t parse_opt (int key, char * arg, struct argp_state * state) {

	struct arguments * arguments = state->input; // communication with main()
	char ** tailptr; // used to detect incorrect values of upper or lower limits
	
	switch (key) {

		// initialize paser by setting up default argument values:
		case ARGP_KEY_INIT:
			arguments->i = 0;
			arguments->verbosity = 0;
			break;
		
		// parse verbosity flags:
		case 'v':
			if (arguments->verbosity < INT_MAX)
				arguments->verbosity++;
			break;
 
		// set output file:
		case 'o':
			arguments->fn_out = arg;
			break;

		// parse input file names:
		case ARGP_KEY_ARG:
			switch(arguments->i) {

				// no input file names yet parsed:
				case 0:
					arguments->fn_in_0 = arg;
					arguments->i++;
					break;

				// one input file name parsed:
				case 1:
					arguments->fn_in_1 = arg;
					arguments->i++;
					break;
			}
			break; // case ARGP_KEY_ARG

			// Check, wheather we’ve got all neccessary options
			case ARGP_KEY_END:
				
			// parsing has been terminated with an error:
			case ARGP_KEY_ERROR:
				fprintf(stderr, "Argument parsing has been terminated due to an error!\n");
				break;

			// catch all:
			default:
				return ARGP_ERR_UNKNOWN;
	} // switch (key)

	return 0;

} // parse_opt()

// put all ARGP stuff together:
const struct argp argp = {options, parse_opt, args_doc, doc};
/* Argument parser setup -- END*/

int main (int argc, char **argv) {

	// get us a communication channel with the parser:
	struct arguments arguments;

	//parse our arguments, all the work is done there:
	argp_parse(&argp, argc, argv, 0, 0, &arguments);

	return arguments.retcode;

};
