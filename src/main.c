#include <ag_fields.h>
#include <ag_structs.h>

#include <sys/stat.h>

#include <io.h>
#include <locale.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILE_SIZE_MB 8

const char *ag_file_name = 0;
uint8_t *ag_file_data = 0;
size_t ag_file_size = 0;

size_t ag_extra_header_offset = PAGE_SIZE;
size_t ag_obj_structs_first_page = 2;
size_t ag_obj_structs_count = 0;
size_t ag_obj_pages_count = 0;
size_t ag_link_structs_first_page = 0;
size_t ag_link_structs_count = 0;
size_t ag_link_pages_count = 0;
size_t ag_extra_data_page = 0;
size_t ag_extra_data_length = 0;

struct ag_main_header *main_header = 0;

bool process_args(int argc, char *argv[]) {
	int narg = 1;
	while (narg < argc) {
		if (!ag_file_name) {
			ag_file_name = argv[narg];
		}
		else {
			fprintf(stderr, "Error: more than one file name specified\n");
			return false;
		}
		++narg;
	}

	if (!ag_file_name) {
		fprintf(stderr, "Error: no file name specified\n");
		return false;
	}

	return true;
}

bool read_ag_file() {
	struct stat stat_buf;
	FILE* ag_file;
	size_t bytes_read;
	if (0 != stat(ag_file_name, &stat_buf)) {
		fprintf(stderr, "Error: failed to stat file\n");
		return false;
	}
	if (stat_buf.st_size > MAX_FILE_SIZE_MB * 1024 * 1024) {
		fprintf(stderr, "Error: file larger than %d MiB\n", MAX_FILE_SIZE_MB);
		return false;
	}
	ag_file = fopen(ag_file_name, "rb");
	if (!ag_file) {
		fprintf(stderr, "Error: failed to open file\n");
		return false;
	}
	ag_file_data = malloc(stat_buf.st_size);
	if (!ag_file_data) {
		fprintf(stderr, "Error: failed to allocate file data buffer\n");
		fclose(ag_file);
		return false;
	}	
	bytes_read = fread(ag_file_data, 1, stat_buf.st_size, ag_file);
	if (bytes_read != stat_buf.st_size) {
		fprintf(stderr, "Error: failed to read file data\n");
		fclose(ag_file);
		free(ag_file_data);
		ag_file_data = 0;
		return false;
	}
	ag_file_size = bytes_read;
	return true;
}

bool check_file_structure() {
	size_t extra_data_offset;

	if (ag_file_size < PAGE_SIZE) {
		fprintf(stderr, "Error: file too small to contain main header\n");
		return false;
	}
	main_header = (struct ag_main_header *)ag_file_data;

	ag_obj_structs_count = main_header->last_alloc_obj;
	ag_obj_pages_count = 
		(ag_obj_structs_count + OBJECTS_PER_PAGE - 1) / OBJECTS_PER_PAGE;
	ag_link_structs_first_page = ag_obj_structs_first_page + ag_obj_pages_count;
	ag_link_structs_count = main_header->last_alloc_link;
	ag_link_pages_count =
		(ag_link_structs_count + LINKS_PER_PAGE - 1) / LINKS_PER_PAGE;
	ag_extra_data_page = ag_link_structs_first_page + ag_link_pages_count;

	extra_data_offset = ag_extra_data_page * PAGE_SIZE;
	if (extra_data_offset > ag_file_size) {
		fprintf(stderr, "Error: file too small to contain data declared in header\n");
		return false;
	}
	ag_extra_data_length = ag_file_size - extra_data_offset;

	fprintf(stderr, "File information:\n");
	fprintf(stderr, "  Total file size: %zu bytes\n", ag_file_size);
	fprintf(stderr, "  %zu object structs (%zu page%s)\n",
		ag_obj_structs_count, ag_obj_pages_count,
		ag_obj_pages_count == 1 ? "" : "s");
	fprintf(stderr, "  %zu link structs (%zu page%s)\n",
		ag_link_structs_count, ag_link_pages_count,
		ag_link_pages_count == 1 ? "" : "s");
	fprintf(stderr, "  %zu bytes extra data\n", ag_extra_data_length);

	return true;
}

void dump_hex(const uint8_t *data, size_t length, size_t abs_offset)
{
	size_t remaining = length;
	size_t rel_offset = 0;

	while (remaining > 0) {
		const size_t this_line =
			remaining > 16 ? 16 : remaining;
		remaining -= this_line;

		printf("%06zx %04zx  ", abs_offset, rel_offset);

		for (size_t i = 0; i < this_line; ++i) {
			printf("%02hhx%s", data[i],
				i==7 ? " - " : " ");
		}
		if (this_line < 16) {
			if (this_line < 8) {
				printf("  ");
			}
			for (size_t i = 0; i < 16 - this_line; ++i) {
				printf("   ");
			}
		}
		printf(" ");

		for (size_t i = 0; i < this_line; ++i) {
			if (data[i] < 0x20 ||
				(data[i] >= 0x7f && data[i] < 0xa0) ||
				data[i] == 0xad)
			{
				printf("%s", "\xc2\xb7");
			}
			else if (data[i] < 0x80) {
				putchar(data[i]);
			}
			else {
				putchar(0xc0 | (data[i] >> 6));
				putchar(0x80 | (data[i] & 0x3f));
			}
		}

		data += this_line;
		abs_offset += this_line;
		rel_offset += this_line;
		printf("\n");
	}
}

void dump_fields(const uint8_t *base,
	const struct ag_field_spec *fields,
	size_t count)
{
	const size_t base_offset = base - ag_file_data;
	for (size_t i = 0; i < count; ++i) {
		const uint8_t *data = base + fields[i].offset;
		const size_t abs_offset = base_offset + fields[i].offset;
		dump_hex(data, fields[i].size, abs_offset);
		printf("  %s: ", fields[i].description);
		dump_field_value(&fields[i], data);
		printf("\n");
	}
	printf("\n");
}

void dump_main_header() {
	const uint8_t *start_address = ag_file_data;
	printf("Main Header\n");
	printf("===========\n");
	dump_fields(start_address, main_header_fields, main_header_field_count);
}

int main(int argc, char *argv[]) {
#ifdef __MINGW32__
	{
		const char* ctype = getenv("LC_CTYPE");
		if (ctype) {
			setlocale(LC_CTYPE, ctype);
		}
	}
#else
	setlocale(LC_ALL, "");
#endif

	if (!process_args(argc, argv)) {
		return 1;
	}
	if (!read_ag_file()) {
		return 1;
	}
	if (!check_file_structure()) {
		return 1;
	}

	dump_main_header();

	if (ag_file_data) {
		free(ag_file_data);
		ag_file_data = 0;
	}
}
