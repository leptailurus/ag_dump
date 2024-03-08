#include <ag_fields.h>
#include <ag_structs.h>

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define sizeof_field(parent, field) (sizeof(((parent*)0)->field))

#define MAKE_FIELD(parent, field, desc, type) \
{ offsetof(struct parent, field), desc, FIELD_TYPE_##type, sizeof_field(struct parent, field) }

struct ag_field_spec main_header_fields[] = {
	MAKE_FIELD(ag_main_header, id, "ID", UINT),
	MAKE_FIELD(ag_main_header, description, "Database description", CSTRING),
	MAKE_FIELD(ag_main_header, password, "Password", AG_PASSWD),
	MAKE_FIELD(ag_main_header, unknown0, "Unknown", UNKNOWN)
};

size_t main_header_field_count =
	sizeof(main_header_fields) / sizeof(main_header_fields[0]);


void dump_unknown_value(size_t size, const uint8_t *data) {
	(void)data;
	printf("(%zu bytes)", size);
}

void dump_int_value(size_t size, bool is_signed, const uint8_t *data) {
	switch (size) {
		uint16_t i2;
		case 2:
			memcpy(&i2, data, 2);
			if (is_signed) {
				printf("%hd (0x%02hx)", (int16_t)i2, i2);
			}
			else {
				printf("%hu (0x%02hx)", i2, i2);
			}
			break;
		default:
			printf("Invalid field size (%zu)", size);
			break;
	}
}

void dump_cstring_value(size_t size, const uint8_t *data) {
}

void dump_field_value(const struct ag_field_spec* field,
	const uint8_t* data)
{
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
		default:
			printf("[Unknown field type]");
			break;
	}
}

