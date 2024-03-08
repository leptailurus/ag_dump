#ifndef _AG_FIELDS_H_
#define _AG_FIELDS_H_

#include <stddef.h>
#include <stdint.h>

enum ag_field_type {
	FIELD_TYPE_UNKNOWN,
	FIELD_TYPE_INT,
	FIELD_TYPE_UINT,
	FIELD_TYPE_CSTRING,
	/* Application-specific fields */
	FIELD_TYPE_AG_PASSWD,
};


struct ag_field_spec {
	size_t offset;
	const char* description;
	enum ag_field_type type;
	size_t size;
};

extern struct ag_field_spec main_header_fields[];
extern size_t main_header_field_count;

void dump_field_value(const struct ag_field_spec *field,
		const uint8_t *data);

#endif
