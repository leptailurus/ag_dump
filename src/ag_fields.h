#ifndef _AG_FIELDS_H_
#define _AG_FIELDS_H_

#include <stddef.h>
#include <stdint.h>

enum ag_field_type {
	FIELD_TYPE_UNKNOWN,
	FIELD_TYPE_INT,
	FIELD_TYPE_UINT,
	FIELD_TYPE_CSTRING,
	FIELD_TYPE_FAT_DATE,
	FIELD_TYPE_FAT_TIME,
	FIELD_TYPE_DATETIME,
	/* Application-specific fields */
	FIELD_TYPE_AG_PASSWD,
	FIELD_TYPE_AG_ITEM_TYPE,
	FIELD_TYPE_AG_CATEGORY_TYPE,
	FIELD_TYPE_AG_GENEALOGY,
	FIELD_TYPE_AG_LINK_TYPE,
};


struct ag_field_spec {
	size_t offset;
	const char* description;
	enum ag_field_type type;
	size_t size;
};

extern struct ag_field_spec main_header_fields[];
extern size_t main_header_field_count;

extern struct ag_field_spec extra_header_fields[];
extern size_t extra_header_field_count;

extern struct ag_field_spec object_struct_base_fields[];
extern size_t object_struct_base_field_count;

extern struct ag_field_spec object_struct_item_fields[];
extern size_t object_struct_item_field_count;

extern struct ag_field_spec object_struct_text_fields[];
extern size_t object_struct_text_field_count;

extern struct ag_field_spec object_struct_category_fields[];
extern size_t object_struct_category_field_count;

extern struct ag_field_spec object_struct_genealogy_fields[];
extern size_t object_struct_genealogy_field_count;

extern struct ag_field_spec link_struct_base_fields[];
extern size_t link_struct_base_field_count;

extern struct ag_field_spec link_struct_link_fields[];
extern size_t link_struct_link_field_count;

extern struct ag_field_spec link_struct_catsearch_fields[];
extern size_t link_struct_catsearch_field_count;

struct dump_context {
	const struct ag_main_header *main_header;
	const struct ag_extra_header *extra_header;
	const void *current_object;
};

void dump_field_value(const struct ag_field_spec *field,
		const uint8_t *data, const struct dump_context *context);

#endif
