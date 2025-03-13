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

struct ag_field_spec object_struct_item_fields[] = {
	MAKE_FIELD(ag_object_struct_item, id, "ID", UINT),
	MAKE_FIELD(ag_object_struct_item, type, "Type", AG_ITEM_TYPE),
	MAKE_FIELD(ag_object_struct_item, text_pointer, "Text pointer", UINT),
	MAKE_FIELD(ag_object_struct_item, note_pointer, "Note pointer", UINT),
	MAKE_FIELD(ag_object_struct_item, unknown08, "Unknown", UNKNOWN),
	MAKE_FIELD(ag_object_struct_item, first_link, "First link", UINT),
	MAKE_FIELD(ag_object_struct_item, last_link, "Last link", UINT),
	MAKE_FIELD(ag_object_struct_item, unknown16, "Unknown", UNKNOWN),
	MAKE_FIELD(ag_object_struct_item, text, "Text", CSTRING),
	MAKE_FIELD(ag_object_struct_item, unknown3e, "Unknown", UNKNOWN)
};

size_t object_struct_item_field_count =
	sizeof(object_struct_item_fields) / sizeof(object_struct_item_fields[0]);


struct ag_field_spec object_struct_text_fields[] = {
	MAKE_FIELD(ag_object_struct_text, id, "ID", UINT),
	MAKE_FIELD(ag_object_struct_text, type, "Type", AG_ITEM_TYPE),
	MAKE_FIELD(ag_object_struct_text, next, "Next", UINT),
	MAKE_FIELD(ag_object_struct_text, prev, "Prev", UINT),
	MAKE_FIELD(ag_object_struct_text, text, "Text", CSTRING)
};

size_t object_struct_text_field_count =
	sizeof(object_struct_text_fields) / sizeof(object_struct_text_fields[0]);


struct ag_field_spec object_struct_category_fields[] = {
	MAKE_FIELD(ag_object_struct_category, id, "ID", UINT),
	MAKE_FIELD(ag_object_struct_category, type, "Type", AG_ITEM_TYPE),
	MAKE_FIELD(ag_object_struct_category, text_pointer, "Text pointer", UINT),
	MAKE_FIELD(ag_object_struct_category, note_pointer, "Note pointer", UINT),
	MAKE_FIELD(ag_object_struct_category, unknown08, "Unknown", UNKNOWN),
	MAKE_FIELD(ag_object_struct_category, prev_sibling, "Prev sibling", UINT),
	MAKE_FIELD(ag_object_struct_category, next_sibling, "Next sibling", UINT),
	MAKE_FIELD(ag_object_struct_category, parent, "Parent", UINT),
	MAKE_FIELD(ag_object_struct_category, first_child, "First child", UINT),
	MAKE_FIELD(ag_object_struct_category, last_child, "Last child", UINT),
	MAKE_FIELD(ag_object_struct_category, unknown26, "Unknown", UNKNOWN),
	MAKE_FIELD(ag_object_struct_category, last_search_node, "Last search node", UINT),
	MAKE_FIELD(ag_object_struct_category, unknown2e, "Unknown", UNKNOWN),
	MAKE_FIELD(ag_object_struct_category, category_type, "Category type", AG_CATEGORY_TYPE),
	MAKE_FIELD(ag_object_struct_category, genealogy_object, "Genealogy object", UINT),
	MAKE_FIELD(ag_object_struct_category, unknown38, "Unknown", UNKNOWN)
};

size_t object_struct_category_field_count =
	sizeof(object_struct_category_fields) / sizeof(object_struct_category_fields[0]);


struct ag_field_spec object_struct_genealogy_fields[] = {
	MAKE_FIELD(ag_object_struct_genealogy, id, "ID", UINT),
	MAKE_FIELD(ag_object_struct_genealogy, type, "Type", AG_ITEM_TYPE),
	MAKE_FIELD(ag_object_struct_genealogy, genealogy, "Genealogy", AG_GENEALOGY)
};

size_t object_struct_genealogy_field_count =
	sizeof(object_struct_genealogy_fields) / sizeof(object_struct_genealogy_fields[0]);


struct ag_field_spec link_struct_base_fields[] = {
	MAKE_FIELD(ag_link_struct_base, id, "ID", UINT),
	MAKE_FIELD(ag_link_struct_base, type, "Type", AG_LINK_TYPE),
	MAKE_FIELD(ag_link_struct_base, unknown04, "Unknown", UNKNOWN)
};

size_t link_struct_base_field_count =
	sizeof(link_struct_base_fields) / sizeof(link_struct_base_fields[0]);


struct ag_field_spec link_struct_link_fields[] = {
	MAKE_FIELD(ag_link_struct_link, id, "ID", UINT),
	MAKE_FIELD(ag_link_struct_link, type, "Type", AG_LINK_TYPE),
	MAKE_FIELD(ag_link_struct_link, target, "Target", UINT),
	MAKE_FIELD(ag_link_struct_link, prev_link, "Prev link", UINT),
	MAKE_FIELD(ag_link_struct_link, next_link, "Next link", UINT),
	MAKE_FIELD(ag_link_struct_link, origin, "Origin", UINT),
	MAKE_FIELD(ag_link_struct_link, unknown0c, "Unknown", UNKNOWN)
};

size_t link_struct_link_field_count =
	sizeof(link_struct_link_fields) / sizeof(link_struct_link_fields[0]);


struct ag_field_spec link_struct_catsearch_fields[] = {
	MAKE_FIELD(ag_link_struct_catsearch, id, "ID", UINT),
	MAKE_FIELD(ag_link_struct_catsearch, type, "Type", AG_LINK_TYPE),
	MAKE_FIELD(ag_link_struct_catsearch, category, "Category", UINT),
	MAKE_FIELD(ag_link_struct_catsearch, next_node, "Next node", UINT),
	MAKE_FIELD(ag_link_struct_catsearch, prev_node, "Prev node", UINT),
	MAKE_FIELD(ag_link_struct_catsearch, left_child, "Left child", UINT),
	MAKE_FIELD(ag_link_struct_catsearch, right_child, "Right child", UINT),
	MAKE_FIELD(ag_link_struct_catsearch, parent, "Parent", UINT),
	MAKE_FIELD(ag_link_struct_catsearch, unknown10, "Unknown", UNKNOWN),
	MAKE_FIELD(ag_link_struct_catsearch, condition, "Condition", UINT),
	MAKE_FIELD(ag_link_struct_catsearch, height, "Height", UINT),
	MAKE_FIELD(ag_link_struct_catsearch, unknown18, "Unknown", UNKNOWN)
};

size_t link_struct_catsearch_field_count =
	sizeof(link_struct_catsearch_fields) / sizeof(link_struct_catsearch_fields[0]);


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

