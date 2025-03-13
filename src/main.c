#include <ag_fields.h>
#include <ag_structs.h>
#include <datetime.h>

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

size_t ag_extra_header_offset = 0;

size_t ag_obj_structs_first_page = 0;
size_t ag_obj_first_page_offset = 0;
size_t ag_obj_structs_count = 0;
size_t ag_obj_pages_count = 0;

size_t ag_link_structs_first_page = 0;
size_t ag_link_first_page_offset = 0;
size_t ag_link_structs_count = 0;
size_t ag_link_pages_count = 0;

size_t ag_extra_data_page = 0;
size_t ag_extra_data_offset = 0;
size_t ag_extra_data_length = 0;

struct ag_main_header *main_header = 0;
struct ag_extra_header *extra_header = 0;

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
	if (ag_file_size < PAGE_SIZE) {
		fprintf(stderr, "Error: file too small to contain main header\n");
		return false;
	}
	main_header = (struct ag_main_header *)ag_file_data;

	ag_extra_header_offset = PAGE_SIZE;
	extra_header = (struct ag_extra_header *)(ag_file_data + ag_extra_header_offset);

	ag_obj_structs_first_page = 2;
	ag_obj_first_page_offset = ag_obj_structs_first_page * PAGE_SIZE;
	ag_obj_structs_count = main_header->last_obj_in_file + 1;
	ag_obj_pages_count = 
		(ag_obj_structs_count + OBJECTS_PER_PAGE - 1) / OBJECTS_PER_PAGE;

	ag_link_structs_first_page = ag_obj_structs_first_page + ag_obj_pages_count;
	ag_link_first_page_offset = ag_link_structs_first_page * PAGE_SIZE;
	ag_link_structs_count = main_header->last_link_in_file + 1;
	ag_link_pages_count =
		(ag_link_structs_count + LINKS_PER_PAGE - 1) / LINKS_PER_PAGE;

	ag_extra_data_page = ag_link_structs_first_page + ag_link_pages_count;
	ag_extra_data_offset = ag_extra_data_page * PAGE_SIZE;

	if (ag_extra_data_offset > ag_file_size) {
		fprintf(stderr, "Error: file too small to contain data declared in header\n");
		return false;
	}
	ag_extra_data_length = ag_file_size - ag_extra_data_offset;

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

void dump_hex(const uint8_t *data, size_t length, size_t abs_offset, size_t rel_offset)
{
	size_t line_rel_offset = (rel_offset / 16) * 16;
	size_t line_abs_offset = (abs_offset / 16) * 16;
	size_t line_data_start = rel_offset - line_rel_offset;
	size_t remaining = length;

	while (remaining > 0) {
		const size_t line_available = 16 - line_data_start;
		const size_t this_line =
			remaining > line_available ? line_available : remaining;
		const size_t line_end = line_data_start + this_line;
		remaining -= this_line;

		//printf("    %06zx %04zx  ", abs_offset, line_rel_offset);
		printf("    %04zx  ", line_rel_offset);

		for (size_t i = 0; i < line_data_start; ++i)
		{
			printf("  %s", i==7 ? " - " : " ");
		}

		for (size_t i = 0; i < this_line; ++i) {
			printf("%02hhx%s", data[i],
				(i + line_data_start)==7 ? " - " : " ");
		}
		if (line_end < 16) {
			if (line_end < 8) {
				printf("  ");
			}
			for (size_t i = 0; i < 16 - line_end; ++i) {
				printf("   ");
			}
		}
		printf(" ");

		for (size_t i = 0; i < line_data_start; ++i)
		{
			printf(" ");
		}
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
		line_rel_offset += 16;
		line_abs_offset += 16;
		line_data_start = 0;
		abs_offset += this_line;
		rel_offset += this_line;
		printf("\n");
	}
}

void dump_fields(const uint8_t *base,
	const struct ag_field_spec *fields,
	size_t count, const struct dump_context *context)
{
	const size_t base_offset = base - ag_file_data;
	for (size_t i = 0; i < count; ++i) {
		const uint8_t *data = base + fields[i].offset;
		const size_t abs_offset = base_offset + fields[i].offset;
		printf("  %s: ", fields[i].description);
		dump_field_value(&fields[i], data, context);
		printf("\n");
		dump_hex(data, fields[i].size, abs_offset, fields[i].offset);
	}
	printf("\n");
}

void dump_main_header(struct dump_context *context) {
	const uint8_t *start_address = (const uint8_t*)main_header;
	printf("Main Header\n");
	printf("===========\n");
	context->current_object = start_address;
	dump_fields(start_address, ag_main_header_fields, ag_main_header_field_count, context);
	printf("\n");
}

void dump_extra_header(struct dump_context *context) {
	const uint8_t *start_address = (const uint8_t*)extra_header;
	printf("Extra Header\n");
	printf("============\n");
	context->current_object = start_address;
	dump_fields(start_address, ag_extra_header_fields, ag_extra_header_field_count, context);
	printf("\n");
}

void dump_object_struct(const uint8_t *data, struct dump_context *context) {
	const struct ag_object_struct_base *object = (const struct ag_object_struct_base*)data;
	context->current_object = data;
	printf("Object %05hd\n", object->id);
	printf("------------\n");
	switch (object->type)
	{
		case 1:
			dump_fields(data, object_struct_item_fields, object_struct_item_field_count, context);
			break;
		case 2:
			dump_fields(data, object_struct_text_fields, object_struct_text_field_count, context);
			break;
		case 3:
			dump_fields(data, object_struct_category_fields, object_struct_category_field_count, context);
			break;
		case 9:
			dump_fields(data, object_struct_genealogy_fields, object_struct_genealogy_field_count, context);
			break;
		default:
			dump_fields(data, ag_object_struct_base_fields, ag_object_struct_base_field_count, context);
			break;
	}
}

void dump_objects_structs(const uint8_t *data, struct dump_context *context) {
	printf("Object Structs\n");
	printf("==============\n\n");
	for (size_t i = 0; i < ag_obj_pages_count; ++i) {
		for (size_t j = 0; j < OBJECTS_PER_PAGE; ++j) {
			dump_object_struct(data, context);
			data += OBJECT_STRUCT_SIZE;
		}
	}
}

void dump_link_struct(const uint8_t *data, struct dump_context *context) {
	const struct ag_link_struct_base *link = (const struct ag_link_struct_base*)data;
	context->current_object = data;
	printf("Link %05hd\n", link->id);
	printf("----------\n");
	switch (link->type)
	{
		case 1:
			dump_fields(data, link_struct_link_fields, link_struct_link_field_count, context);
			break;
		case 9:
			dump_fields(data, link_struct_catsearch_fields, link_struct_catsearch_field_count, context);
			break;
		default:
			dump_fields(data, link_struct_base_fields, link_struct_base_field_count, context);
			break;
	}
}

void dump_link_structs(const uint8_t *data, struct dump_context *context) {
	printf("Link Structs\n");
	printf("============\n\n");
	for (size_t i = 0; i < ag_link_pages_count; ++i) {
		for (size_t j = 0; j < LINKS_PER_PAGE; ++j) {
			dump_link_struct(data, context);
			data += LINK_STRUCT_SIZE;
		}
		printf("Padding\n");
		printf("-------\n");
		dump_hex(data, LINKS_PAGE_PADDING, data - ag_file_data, 0);
		data += LINKS_PAGE_PADDING;
		printf("\n");
	}
}

void dump_extra_data(const uint8_t *data, size_t size, struct dump_context *context) {
	printf("Extra Data\n");
	printf("==========\n\n");
	printf("  (%zu bytes)\n", size);
	dump_hex(data, size, data - ag_file_data, 0);
}

int main(int argc, char *argv[]) {
/*
	int jd = gregorian_to_jdn(2025, 2, 21);
	printf("JDN: %d\n", jd);
	int y, m, d;
	jdn_to_gregorian(jd, &y, &m, &d);
	printf("Gregorian: %d-%d-%d\n", y, m, d);

	double d1;
	uint8_t* db = (uint8_t*)&d1;
	db[0] = 0xfd;
	db[1] = 0xd2;
	db[2] = 0x27;
	db[3] = 0x7d;
	db[4] = 0xb5;
	db[5] = 0x51;
	db[6] = 0xe6;
	db[7] = 0x40;

	printf("DT before: %f\n", d1);

	struct ag_datetime adt;
	double_to_ag_datetime(d1, &adt);

	printf("AG DT: %04d-%02d-%02d %02d:%02d:%02d\n",
		adt.year, adt.month, adt.day, adt.hour, adt.minute, adt.second);

	double d2 = ag_datetime_to_double(&adt);
	printf("DT after: %f\n", d2);

	uint16_t fd = 0x6855;
	uint16_t ft = 0x80ef;
	memset(&adt, 0, sizeof(adt));
	fat_datetime_to_ag_datetime(fd, ft, &adt);
	printf("AG from FAT DT: %04d-%02d-%02d %02d:%02d:%02d\n",
		adt.year, adt.month, adt.day, adt.hour, adt.minute, adt.second);
	fd = ft = 0;
	ag_datetime_to_fat_datetime(&adt, &fd, &ft);
	printf("FAT datetime: 0x%04hx 0x%04hx\n", fd, ft);

	return 0;
*/

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

	struct dump_context context;
	context.main_header = main_header;
	context.extra_header = extra_header;

	dump_main_header(&context);
	dump_extra_header(&context);
	dump_objects_structs(ag_file_data + ag_obj_first_page_offset, &context);
	dump_link_structs(ag_file_data + ag_link_first_page_offset, &context);
	dump_extra_data(ag_file_data + ag_extra_data_offset, ag_extra_data_length, &context);

	if (ag_file_data) {
		free(ag_file_data);
		ag_file_data = 0;
	}
}
