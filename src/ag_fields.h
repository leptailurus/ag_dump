#ifndef _AG_FIELDS_H_
#define _AG_FIELDS_H_

#include <stddef.h>

enum ag_field_type {
	FIELD_TYPE_UNKNOWN,
	FIELD_TYPE_INT,
	FIELD_TYPE_UINT,
};


struct ag_field_spec {
	size_t offset;
	const char* description;
	enum ag_field_type type;
	size_t size;
};

extern struct ag_field_spec main_header_fields[];
extern size_t main_header_field_count;

#endif
