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
	"xorify -- A program calculating bitwise XOR of IN_FILE_0 and IN_FILE_1. "
	"If IN_FILE_1 is omitted, stdin is used instead.\v"
	"Principal use of this program is to highlight differences in binary "
	"files.";

// non-option arguments description:
const char args_doc[] = "IN_FILE_0 [IN_FILE_1]";

// communication between parse_opt() and main():
struct arguments {
	int i;					/* number of command line arguments processed */
	int verbosity;				/* verbosity option */
	const char * fn_in_0;			/* first input file name */
	const char * fn_in_1;			/* secound input file name */
	const char * fn_out;			/* output file name */
};

// option arguments we understand:
const struct argp_option options[] = {
	{"verbose", 'v', 0, 0, "Produce verbose output"},
	{"output", 'o',  "OUT_FILE", 0, "Output file name (if omitted, stdout is used)"},
	{0}
};

// argument parser logics:
error_t parse_opt (int key, char * arg, struct argp_state * state) {

	struct arguments * arguments = state->input; // communication with main()
	
	switch (key) {

		// initialize paser by setting up default argument values:
		case ARGP_KEY_INIT:
			arguments->i = 0;
			arguments->verbosity = 0;
			arguments->fn_in_0 = NULL;
			arguments->fn_in_1 = NULL;
			arguments->fn_out = NULL;
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
				// parsing first input file name:
				case 0:
					arguments->fn_in_0 = arg;
					break;

				// parsing second input file name:
				case 1:
					arguments->fn_in_1 = arg;
					break;
			}
			arguments->i++;
			break; // case ARGP_KEY_ARG

		// check, whether we’ve got all neccessary arguments
		case ARGP_KEY_END:
			if (arguments->i > 2) {
				argp_error(state,
					"Too many command line arguments. Only IN_FILE_0 and IN_FILE_1 are supported.");
			}
			if (arguments->i < 1) {
				argp_error(state, "Mandatory argument IN_FILE_0 is missing.");
			}
			break;
				
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

	// parse our arguments:
	argp_parse(&argp, argc, argv, 0, 0, &arguments);

	// open all needed streams:
	FILE * fp_in_0;
	FILE * fp_in_1;
	FILE * fp_out;
	if (!(fp_in_0 = fopen(arguments.fn_in_0,"r"))) {
		fprintf(stderr, "%s: %s\n", arguments.fn_in_0, strerror(errno));
		return EXIT_FAILURE;
	} else if(arguments.verbosity)
		fprintf(stderr, "%s: Successfully opened for reading.\n", arguments.fn_in_0);

	if (!arguments.fn_in_1) {
		fp_in_1 = stdin;
		if (arguments.verbosity) fprintf(stderr, "IN_FILE_1 not provided, using stdin.\n");
	} else {
		if (!(fp_in_1 = fopen(arguments.fn_in_1,"r"))) {
			fprintf(stderr, "%s: %s\n", arguments.fn_in_0, strerror(errno));
			return EXIT_FAILURE;
		} else if (arguments.verbosity) {
			fprintf(stderr, "%s: Successfully opened for reading.\n", arguments.fn_in_1);
		}
	}

	if (!arguments.fn_out) {
		fp_out = stdout;
		if (arguments.verbosity) fprintf(stderr, "OUT_FILE not provided, using stdout.\n");
	} else {
		if (!(fp_out = fopen(arguments.fn_out,"w"))) {
			fprintf(stderr, "%s: %s\n", arguments.fn_out, strerror(errno));
			return EXIT_FAILURE;
		} else if (arguments.verbosity) {
			fprintf(stderr, "%s: Successfully opened for writing.\n", arguments.fn_out);
		}
	}

	// do our main task:
	int in_0, in_1;
	while(in_0=1) {
	}

	// close opened streams:
	fclose(fp_in_0);
	fclose(fp_in_1);
	fclose(fp_out);

	return EXIT_SUCCESS;
}
