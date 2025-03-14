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

struct ag_bitfield_member {
	size_t offset;
	size_t size;
	size_t max_value;
	const char **value_names;
};

#define DECLARE_STRUCT(name) \
extern struct ag_field_spec name##_fields[]; \
extern size_t name##_field_count

#define DECLARE_BITFIELD(name) \
extern struct ag_bitfield_member name##_members[]; \
extern size_t name##_member_count

#define DECLARE_ENUM_NAMES(name) \
extern const char *name##_names[]; \
extern size_t name##_name_count

DECLARE_ENUM_NAMES(ag_global_off_on);

DECLARE_STRUCT(ag_main_header);
DECLARE_STRUCT(ag_extra_header);

DECLARE_STRUCT(ag_object_struct_base);
DECLARE_STRUCT(ag_object_struct_item);
DECLARE_STRUCT(ag_object_struct_text);
DECLARE_STRUCT(ag_object_struct_category);
DECLARE_STRUCT(ag_object_struct_genealogy);

DECLARE_STRUCT(ag_link_struct_base);
DECLARE_STRUCT(ag_link_struct_link);
DECLARE_STRUCT(ag_link_struct_catsearch);

struct dump_context {
	const struct ag_main_header *main_header;
	const struct ag_extra_header *extra_header;
	const void *current_object;
};

void dump_field_value(const struct ag_field_spec *field,
		const uint8_t *data, const struct dump_context *context);

#endif
