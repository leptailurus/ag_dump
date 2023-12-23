#include <ag_fields.h>
#include <ag_structs.h>

#define sizeof_field(parent, field) (sizeof(((parent*)0)->field))

#define MAKE_FIELD(parent, field, desc, type) \
{ offsetof(struct parent, field), desc, FIELD_TYPE_##type, sizeof_field(struct parent, field) }

struct ag_field_spec main_header_fields[] = {
	MAKE_FIELD(ag_main_header, id, "ID", UINT),
	MAKE_FIELD(ag_main_header, unknown0, "Unknown", UNKNOWN)
};

size_t main_header_field_count =
	sizeof(main_header_fields) / sizeof(main_header_fields[0]);
