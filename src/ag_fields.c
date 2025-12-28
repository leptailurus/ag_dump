#include <ag_fields.h>
#include <ag_structs.h>
#include <cp850.h>
#include <datetime.h>

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define sizeof_field(parent, field) (sizeof(((parent*)0)->field))

#define _M_CAT(a, b) a##b
#define M_CAT(a, b) _M_CAT(a, b)
#define ID(a) a

#define DEFINE_ENUM_VALUES(name, ...) \
const struct ag_enum_value name##_values[] = {__VA_ARGS__}; \
const size_t name##_value_count = sizeof(name##_values) / sizeof(name##_values[0])

DEFINE_ENUM_VALUES(ag_no_yes,
	{0x00, "No"},
	{0x01, "Yes"}
);

DEFINE_ENUM_VALUES(ag_global_off_on,
	{0x00, "Global"},
	{0x01, "Off"},
	{0x02, "On"}
);

DECLARE_ENUM_VALUES(ag_text_enclosed_by);

DEFINE_ENUM_VALUES(ag_text_enclosed_by,
	   {'\"', "\" \""},
	   {'<', "< >"},
	   {'\'', "\' \'"},
	   {'(', "( )"},
	   {'/', "/ /"},
	   {'#', "# #"},
	   {'[', "[ ]"},
	   {'{', "{ }"}
);

#define DEFINE_BITFIELD(name, ...) \
const struct ag_bitfield_member name##_members[] = {__VA_ARGS__}; \
const size_t name##_member_count = sizeof(name##_members) / sizeof(name##_members[0])

#define BITFIELD_UNKNOWN(offset, size) {offset, size, "Unknown", BITFIELD_MEMBER_UNKNOWN, 0, 0}
#define BITFIELD_RESERVED(offset, size) {offset, size, "Reserved", BITFIELD_MEMBER_RESERVED, 0, 0}
#define BITFIELD_ENUM(offset, size, description, name) {offset, size, description, BITFIELD_MEMBER_ENUM, name##_values, name##_value_count}

DEFINE_BITFIELD(ag_backup_settings,
	BITFIELD_ENUM(0, 1, "Make backup on open", ag_no_yes),
	BITFIELD_RESERVED(1, 7)
);

#define STRUCT_BEGIN \
struct ag_field_spec M_CAT(STRUCT_NAME, _fields[]) = {

#define STRUCT_END \
};\
\
size_t M_CAT(STRUCT_NAME, _field_count) = \
	sizeof(M_CAT(STRUCT_NAME, _fields)) / sizeof(M_CAT(STRUCT_NAME, _fields[0]));

#define STRUCT_FIELD(field, desc, type) \
{ offsetof(struct ID(STRUCT_NAME), field), desc, FIELD_TYPE_##type, sizeof_field(struct ID(STRUCT_NAME), field), 0, 0 }

#define STRUCT_FIELD_BITFIELD(field, desc, bitfield) \
{ offsetof(struct ID(STRUCT_NAME), field), desc, FIELD_TYPE_BITFIELD, sizeof_field(struct ID(STRUCT_NAME), field), bitfield##_members, bitfield##_member_count }

#define STRUCT_FIELD_ENUM(field, desc, enum_name) \
{ offsetof(struct ID(STRUCT_NAME), field), desc, FIELD_TYPE_ENUM, sizeof_field(struct ID(STRUCT_NAME), field), enum_name##_values, enum_name##_value_count }

#define STRUCT_NAME ag_main_header
STRUCT_BEGIN
	STRUCT_FIELD(id, "ID", UINT),
	STRUCT_FIELD(description, "Database description", CSTRING),
	STRUCT_FIELD(password, "Password", AG_PASSWD),
	STRUCT_FIELD(first_free_obj, "First free object", UINT),
	STRUCT_FIELD(last_obj_in_file, "Last object in file", UINT),
	STRUCT_FIELD(first_free_link, "First free link", UINT),
	STRUCT_FIELD(last_link_in_file, "Last link in file", UINT),
	STRUCT_FIELD(main_category, "Main category", UINT),
	STRUCT_FIELD(first_view, "First view", UINT),
	STRUCT_FIELD(active_view, "Active view", UINT),
	STRUCT_FIELD(reserved046, "Reserved", RESERVED),
	STRUCT_FIELD(unknown052, "Unknown", UNKNOWN),
	STRUCT_FIELD(reserved054, "Reserved", RESERVED),
	STRUCT_FIELD(previous_active_view, "Previously active view", UINT),
	STRUCT_FIELD(last_free_obj, "Last free object", UINT),
	STRUCT_FIELD(last_free_link, "Last free link", UINT),
	STRUCT_FIELD(save_date, "Save date", FAT_DATE),
	STRUCT_FIELD_BITFIELD(backup_settings, "Backup settings", ag_backup_settings),
	STRUCT_FIELD(unknown05f, "Unknown", UNKNOWN),
	STRUCT_FIELD(save_time, "Save time", FAT_TIME),
	STRUCT_FIELD(done_export_file, "Done items export file", UINT),
	STRUCT_FIELD(reserved064, "Reserved 0x0064", RESERVED),
	STRUCT_FIELD(last_discarded_item, "Last discarded item", UINT),
	STRUCT_FIELD(search_tree_root, "Search tree root", UINT),
	STRUCT_FIELD(last_imported_file, "Last imported file", UINT),
	STRUCT_FIELD(reserved06e, "Reserved 0x006e", RESERVED),
	STRUCT_FIELD(circular_category, "Circular reference category", UINT),
	STRUCT_FIELD(item_count, "Item count", UINT),
	STRUCT_FIELD(category_count, "Category count", UINT),
	STRUCT_FIELD(free_link_count, "Free link count", UINT),
	STRUCT_FIELD(free_object_count, "Free object count", UINT),
	STRUCT_FIELD(reserved07a, "Reserved 0x007a", RESERVED),
	STRUCT_FIELD(tab_width, "Tab width", UINT),
	STRUCT_FIELD(reserved07d, "Reserved 0x007d", RESERVED),
	STRUCT_FIELD_ENUM(ignore_text_enclosed_by, "Ignore item text enclosed by", ag_text_enclosed_by),
	STRUCT_FIELD(reserved081, "Reserved 0x0081", RESERVED),
	STRUCT_FIELD(unknown084, "Unknown", UNKNOWN),
	STRUCT_FIELD(save_datetime, "Save date/time", DATETIME),
	STRUCT_FIELD(unknown170, "Unknown", UNKNOWN)
STRUCT_END
#undef STRUCT_NAME

#define STRUCT_NAME ag_extra_header
STRUCT_BEGIN
	STRUCT_FIELD(unknown000, "Unknown", UNKNOWN)
STRUCT_END
#undef STRUCT_NAME

#define STRUCT_NAME ag_object_struct_base
STRUCT_BEGIN
	STRUCT_FIELD(id, "ID", UINT),
	STRUCT_FIELD(type, "Type", AG_ITEM_TYPE),
	STRUCT_FIELD(unknown04, "Unknown", UNKNOWN)
STRUCT_END
#undef STRUCT_NAME

#define STRUCT_NAME ag_object_struct_item
STRUCT_BEGIN
	STRUCT_FIELD(id, "ID", UINT),
	STRUCT_FIELD(type, "Type", AG_ITEM_TYPE),
	STRUCT_FIELD(text_pointer, "Text pointer", UINT),
	STRUCT_FIELD(note_pointer, "Note pointer", UINT),
	STRUCT_FIELD(unknown08, "Unknown", UNKNOWN),
	STRUCT_FIELD(first_link, "First link", UINT),
	STRUCT_FIELD(last_link, "Last link", UINT),
	STRUCT_FIELD(unknown16, "Unknown", UNKNOWN),
	STRUCT_FIELD(text, "Text", CSTRING),
	STRUCT_FIELD(unknown3e, "Unknown", UNKNOWN)
STRUCT_END
#undef STRUCT_NAME

#define STRUCT_NAME ag_object_struct_text
STRUCT_BEGIN
	STRUCT_FIELD(id, "ID", UINT),
	STRUCT_FIELD(type, "Type", AG_ITEM_TYPE),
	STRUCT_FIELD(next, "Next", UINT),
	STRUCT_FIELD(prev, "Prev", UINT),
	STRUCT_FIELD(text, "Text", CSTRING)
STRUCT_END
#undef STRUCT_NAME

#define STRUCT_NAME ag_object_struct_category
STRUCT_BEGIN
	STRUCT_FIELD(id, "ID", UINT),
	STRUCT_FIELD(type, "Type", AG_ITEM_TYPE),
	STRUCT_FIELD(text_pointer, "Text pointer", UINT),
	STRUCT_FIELD(note_pointer, "Note pointer", UINT),
	STRUCT_FIELD(unknown08, "Unknown", UNKNOWN),
	STRUCT_FIELD(prev_sibling, "Prev sibling", UINT),
	STRUCT_FIELD(next_sibling, "Next sibling", UINT),
	STRUCT_FIELD(parent, "Parent", UINT),
	STRUCT_FIELD(first_child, "First child", UINT),
	STRUCT_FIELD(last_child, "Last child", UINT),
	STRUCT_FIELD(unknown26, "Unknown", UNKNOWN),
	STRUCT_FIELD(last_search_node, "Last search node", UINT),
	STRUCT_FIELD(unknown2e, "Unknown", UNKNOWN),
	STRUCT_FIELD(category_type, "Category type", AG_CATEGORY_TYPE),
	STRUCT_FIELD(genealogy_object, "Genealogy object", UINT),
	STRUCT_FIELD(unknown38, "Unknown", UNKNOWN)
STRUCT_END
#undef STRUCT_NAME

#define STRUCT_NAME ag_object_struct_genealogy
STRUCT_BEGIN
	STRUCT_FIELD(id, "ID", UINT),
	STRUCT_FIELD(type, "Type", AG_ITEM_TYPE),
	STRUCT_FIELD(genealogy, "Genealogy", AG_GENEALOGY)
STRUCT_END
#undef STRUCT_NAME


#define STRUCT_NAME ag_link_struct_base
STRUCT_BEGIN
	STRUCT_FIELD(id, "ID", UINT),
	STRUCT_FIELD(type, "Type", AG_LINK_TYPE),
	STRUCT_FIELD(unknown04, "Unknown", UNKNOWN)
STRUCT_END
#undef STRUCT_NAME

#define STRUCT_NAME ag_link_struct_link
STRUCT_BEGIN
	STRUCT_FIELD(id, "ID", UINT),
	STRUCT_FIELD(type, "Type", AG_LINK_TYPE),
	STRUCT_FIELD(target, "Target", UINT),
	STRUCT_FIELD(prev_link, "Prev link", UINT),
	STRUCT_FIELD(next_link, "Next link", UINT),
	STRUCT_FIELD(origin, "Origin", UINT),
	STRUCT_FIELD(unknown0c, "Unknown", UNKNOWN)
STRUCT_END
#undef STRUCT_NAME

#define STRUCT_NAME ag_link_struct_catsearch
STRUCT_BEGIN
	STRUCT_FIELD(id, "ID", UINT),
	STRUCT_FIELD(type, "Type", AG_LINK_TYPE),
	STRUCT_FIELD(category, "Category", UINT),
	STRUCT_FIELD(next_node, "Next node", UINT),
	STRUCT_FIELD(prev_node, "Prev node", UINT),
	STRUCT_FIELD(left_child, "Left child", UINT),
	STRUCT_FIELD(right_child, "Right child", UINT),
	STRUCT_FIELD(parent, "Parent", UINT),
	STRUCT_FIELD(unknown10, "Unknown", UNKNOWN),
	STRUCT_FIELD(condition, "Condition", UINT),
	STRUCT_FIELD(height, "Height", UINT),
	STRUCT_FIELD(unknown18, "Unknown", UNKNOWN)
STRUCT_END
#undef STRUCT_NAME

void dump_unknown_reserved_value(size_t size, bool is_reserved, const uint8_t *data, struct dump_context *context) {
	if (is_reserved) {
		for (size_t i = 0; i < size; i++) {
			if (data[i] != 0) {
				fprintf(stderr, "WARNING: %s (offset 0x%08llx, local 0x%04llx) reserved area contains nonzero bytes\n",
						context->current_object_name, context->global_offset, context->local_offset);
				break;
			}
		}
	}
	printf("(%zu bytes)", size);
}

void dump_int_value(size_t size, bool is_signed, const uint8_t *data) {
	switch (size) {
		uint8_t i1;
		uint16_t i2;
		case 1:
			memcpy(&i1, data, 1);
			if (is_signed) {
				printf("%hhd (0x%02hhx)", (int8_t)i1, i1);
			}
			else {
				printf("%hhu (0x%02hhx)", i1, i1);
			}
			break;
		case 2:
			memcpy(&i2, data, 2);
			if (is_signed) {
				printf("%hd (0x%04hx)", (int16_t)i2, i2);
			}
			else {
				printf("%hu (0x%04hx)", i2, i2);
			}
			break;
		default:
			printf("Invalid field size (%zu)", size);
			break;
	}
}

void dump_cstring_value(size_t size, const uint8_t *data) {
	for (size_t i = 0; i < size; ++i) {
		if (data[i] == 0) {
			break;
		}
		printf("%s", cp850_utf8[data[i]]);
	}
}

void dump_fat_date(size_t size, const uint8_t *data) 
{
	struct ag_datetime adt;
	const uint16_t *fd = (const uint16_t*)data;
	fat_datetime_to_ag_datetime(*fd, 0, &adt);
	printf("%04d/%04d-%02d-%02d (0x%04hx)",
		adt.year + 32, adt.year, adt.month, adt.day, *fd);
}

void dump_fat_time(size_t size, const uint8_t *data) 
{
	struct ag_datetime adt;
	const uint16_t *ft = (const uint16_t*)data;
	fat_datetime_to_ag_datetime(0, *ft, &adt);
	printf("%02d:%02d:%02d (0x%04hx)", adt.hour, adt.minute, adt.second, *ft);
}

void dump_datetime(size_t size, const uint8_t *data) 
{
	struct ag_datetime adt;
	const double *dt = (const double*)data;
	double_to_ag_datetime(*dt, &adt);
	printf("%04d-%02d-%02d %02d:%02d:%02d (%f)",
		adt.year, adt.month, adt.day, adt.hour, adt.minute, adt.second, *dt);
}

void dump_bitfield_member_unk_res(uint32_t value, const struct ag_bitfield_member *member, struct dump_context *context)
{
	if (member->type == BITFIELD_MEMBER_UNKNOWN) {
		printf("Unknown (0x%x)", value);
	}
	else {
		printf("Reserved (0x%x)", value);
		if (value != 0) {
			fprintf(stderr, "WARNING: %s (offset 0x%08llx, local 0x%04llx) reserved bitfield member at offset %lld is nonzero\n",
					context->current_object_name, context->global_offset, context->local_offset, member->offset);
		}
	}
}

void dump_bitfield_member_enum(uint32_t value, const struct ag_bitfield_member *member, struct dump_context *context)
{
	const struct ag_enum_value* enum_values = (const struct ag_enum_value*)member->context;
	bool found = false;
	printf("%s: ", member->description);
	for (size_t i = 0; i < member->context_size; i++) {
		if (value == enum_values[i].value) {
			found = true;
			printf("%s (0x%x)", enum_values[i].name, value);
			break;
		}
	}
	if (!found) {
		printf("<Invalid> (0x%x)", value);
	}
}

void dump_bitfield(size_t size, const char* description, const struct ag_bitfield_member *members, size_t member_count, const uint8_t *data, struct dump_context *context)
{
	const size_t num_bits = size * 8;
	size_t next_bit = 0;
	uint32_t value;

	switch (size)
	{
		case 1:
			value = *((const uint8_t*)data);
			break;
		case 2:
			value = *((const uint16_t*)data);
			break;
		case 4:
			value = *((const uint32_t*)data);
			break;
		default:
			fprintf(stderr, "ERROR: Bitfield %s; found size: %lld, expected 1, 2, or 4\n", description, size);
			return;
	}

	printf("Bitfield");
	for (size_t im = 0; im < member_count; im++) {
		const struct ag_bitfield_member *m = members + im;
		const uint32_t member_mask = 0xffffffff >> (32 - m->size);
		const uint32_t member_value = (value >> m->offset) & member_mask;

		if (m->offset < next_bit) {
			fprintf(stderr, "ERROR: Bitfield %s; member %lld overlapping with previous member(s)\n", description, im);
			return;
		}
		if (m->offset >= num_bits) {
			fprintf(stderr, "ERROR: Bitfield %s; member %lld starting beyond end of bitfield\n", description, im);
			return;
		}
		if (m->size < 1) {
			fprintf(stderr, "ERROR: Bitfield %s; member %lld has size <= 1\n", description, im);
			return;
		}
		if (m->offset + m->size > num_bits) {
			fprintf(stderr, "ERROR: Bitfield %s; member %lld extending beyond end of bitfield\n", description, im);
			return;
		}
		if (m->offset > next_bit) {
			fprintf(stderr, "WARNING: Bitfield %s; member %lld has a gap to previous member\n", description, im);
		}

		printf("\n      ");
		for (int8_t bit = (int8_t)num_bits - 1; bit >= 0; --bit) {
			uint32_t mask = 1 << bit;
			uint32_t bit_value = value & mask;
			if (bit >= m->offset && bit < (m->offset + m->size)) {
				printf(bit_value != 0 ? "1" : "0");
			}
			else {
				printf("x");
			}
			if ((bit % 4) == 0) {
				printf(" ");
			}
		}

		switch (m->type) {
			case BITFIELD_MEMBER_UNKNOWN:
			case BITFIELD_MEMBER_RESERVED:
				dump_bitfield_member_unk_res(member_value, m, context);
				break;
			case BITFIELD_MEMBER_ENUM:
				dump_bitfield_member_enum(member_value, m, context);
				break;
		}

		next_bit = m->offset + m->size;
	}

	if (next_bit < num_bits) {
		fprintf(stderr, "WARNING: Bitfield %s; members do not cover all bits\n", description);
	}
}

void dump_enum(size_t size, const uint8_t *data, const struct ag_field_spec *spec)
{
	const struct ag_enum_value* enum_values = (const struct ag_enum_value*)spec->context;
	bool found = false;
	for (size_t i = 0; i < spec->context_size; i++) {
		if (data[0] == enum_values[i].value) {
			found = true;
			printf("%s (0x%x)", enum_values[i].name, data[0]);
			break;
		}
	}
	if (!found) {
		printf("<Invalid> (0x%x)", data[0]);
	}
}

void dump_ag_password(size_t size, const uint8_t *data)
{
	for (size_t i = 0; i < size - 1; ++i) {
		uint8_t c = data[i] ^ 0xab;
		if (c == 0) {
			break;
		}
		printf("%s", cp850_utf8[c]);
	}
}

void dump_ag_item_type(size_t size, const uint8_t *data)
{
	const uint16_t *type = (const uint16_t*)data;
	switch (*type)
	{
		case 1: printf("Item"); break;
		case 2: printf("Text"); break;
		case 3: printf("Category"); break;
		case 9: printf("Genealogy"); break;
		default: printf("Unknown"); break;
	}
	printf(" (%hu / 0x%04hx)", *type, *type);
}

void dump_ag_category_type(size_t size, const uint8_t *data)
{
	const uint16_t *type = (const uint16_t*)data;
	switch (*type)
	{
		case 0x01: printf("Standard"); break;
		case 0x05: printf("Unindexed"); break;
		case 0x09: printf("Numeric"); break;
		case 0x0d: printf("Date/Time"); break;
		default: printf("Unknown"); break;
	}
	printf(" (%hu / 0x%04hx)", *type, *type);
}

void dump_ag_genealogy(size_t size, const uint8_t *data)
{
	const uint16_t *gen = (const uint16_t*)data;
	const size_t count = size / sizeof(uint16_t);
	for (size_t i = 0; i < count; ++i)
	{
		if (gen[i] == 0)
		{
			break;
		}
		if (i > 0) {
			printf(" -> ");
		}
		printf("%hu", gen[i]);
	}
}

void dump_ag_link_type(size_t size, const uint8_t *data)
{
	const uint16_t *type = (const uint16_t*)data;
	switch (*type)
	{
		case 1: printf("Link"); break;
		case 9: printf("Category search node"); break;
		default: printf("Unknown"); break;
	}
	printf(" (%hu / 0x%04hx)", *type, *type);
}


void dump_field_value(const struct ag_field_spec* field,
	const uint8_t* data, struct dump_context *context)
{
	(void)context;

	switch (field->type) {
		case FIELD_TYPE_UNKNOWN:
			dump_unknown_reserved_value(field->size, false, data, context);
			break;
		case FIELD_TYPE_RESERVED:
			dump_unknown_reserved_value(field->size, true, data, context);
			break;
		case FIELD_TYPE_INT:
			dump_int_value(field->size, true, data);
			break;
		case FIELD_TYPE_UINT:
			dump_int_value(field->size, false, data);
			break;
		case FIELD_TYPE_CSTRING:
			dump_cstring_value(field->size, data);
			break;
		case FIELD_TYPE_FAT_DATE:
			dump_fat_date(field->size, data);
			break;
		case FIELD_TYPE_FAT_TIME:
			dump_fat_time(field->size, data);
			break;
		case FIELD_TYPE_DATETIME:
			dump_datetime(field->size, data);
			break;
		case FIELD_TYPE_BITFIELD:
			dump_bitfield(field->size, field->description, (const struct ag_bitfield_member*)field->context, field->context_size, data, context);
			break;
		case FIELD_TYPE_ENUM:
			dump_enum(field->size, data, field);
			break;
		case FIELD_TYPE_AG_PASSWD:
			dump_ag_password(field->size, data);
			break;
		case FIELD_TYPE_AG_ITEM_TYPE:
			dump_ag_item_type(field->size, data);
			break;
		case FIELD_TYPE_AG_CATEGORY_TYPE:
			dump_ag_category_type(field->size, data);
			break;
		case FIELD_TYPE_AG_GENEALOGY:
			dump_ag_genealogy(field->size, data);
			break;
		case FIELD_TYPE_AG_LINK_TYPE:
			dump_ag_link_type(field->size, data);
			break;
		default:
			printf("[Unknown field type]");
			break;
	}
}

