#include <stdbool.h>
#include <stdio.h>
#include <string.h>

const char* ag_file_name = 0;

bool process_args(int argc, char *argv[]) {
	int narg = 1;
	while (narg < argc) {
		if (!ag_file_name) {
			ag_file_name = argv[narg];
		}
		else {
			fprintf(stderr, "Error: more than one file name specified");
			return false;
		}
		++narg;
	}

	if (!ag_file_name) {
		fprintf(stderr, "Error: no file name specified");
		return false;
	}

	return true;
}

int main(int argc, char *argv[]) {
	if (!process_args(argc, argv)) {
		return 1;
	}
}
