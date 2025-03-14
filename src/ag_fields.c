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

#define DEFINE_ENUM_NAMES(name, ...) \
const char *name##_names[] = {__VA_ARGS__}; \
size_t name##_name_count = sizeof(name##_names) / sizeof(name##_names[0])

DEFINE_ENUM_NAMES(ag_global_off_on,
	"Global",
	"Off",
	"On"
);


#define DEFINE_STRUCT(name, ...) \
struct ag_field_spec name##_fields[] = { \
	__VA_ARGS__ \
}; \
size_t name##_field_count = \
	sizeof(name##_fields) / sizeof(name##_fields[0]))

#define STRUCT_BEGIN \
struct ag_field_spec M_CAT(STRUCT_NAME, _fields[]) = {

#define STRUCT_END \
};\
\
size_t M_CAT(STRUCT_NAME, _field_count) = \
	sizeof(M_CAT(STRUCT_NAME, _fields)) / sizeof(M_CAT(STRUCT_NAME, _fields[0]));

#define MAKE_FIELD(parent, field, desc, type) \
{ offsetof(struct parent, field), desc, FIELD_TYPE_##type, sizeof_field(struct parent, field) }

#define STRUCT_FIELD(field, desc, type) \
{ offsetof(struct ID(STRUCT_NAME), field), desc, FIELD_TYPE_##type, sizeof_field(struct ID(STRUCT_NAME), field) }

#define STRUCT_NAME ag_main_header
STRUCT_BEGIN
	STRUCT_FIELD(id, "ID", UINT),
	STRUCT_FIELD(description, "Database description", CSTRING),
	STRUCT_FIELD(password, "Password", AG_PASSWD),
	STRUCT_FIELD(first_free_obj, "First free object", UINT),
	STRUCT_FIELD(last_obj_in_file, "Last object in file", UINT),
	STRUCT_FIELD(first_free_link, "First free link", UINT),
	STRUCT_FIELD(last_link_in_file, "Last link in file", UINT),
	STRUCT_FIELD(unknown040, "Unknown", UNKNOWN),
	STRUCT_FIELD(unknown058, "Unknown", UNKNOWN),
	STRUCT_FIELD(save_date, "Save date", FAT_DATE),
	STRUCT_FIELD(unknown05e, "Unknown", UNKNOWN),
	STRUCT_FIELD(save_time, "Save time", FAT_TIME),
	STRUCT_FIELD(unknown062, "Unknown", UNKNOWN),
	STRUCT_FIELD(category_count, "Category count", UINT),
	STRUCT_FIELD(unknown076, "Unknown", UNKNOWN),
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

void dump_unknown_value(size_t size, const uint8_t *data) {
	(void)data;
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

void dump_ag_password(size_t size, const uint8_t *data)
{
	for (size_t i = 0; i < size; ++i) {
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
	const uint8_t* data, const struct dump_context *context)
{
	(void)context;

	switch (field->type) {
		case FIELD_TYPE_UNKNOWN:
			dump_unknown_value(field->size, data);
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

