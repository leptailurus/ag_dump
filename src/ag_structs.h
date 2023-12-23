#ifndef _AG_STRUCTS_H_
#define _AG_STRUCTS_H_

#include <assert.h>
#include <stdint.h>

#define PAGE_SIZE 512
#define OBJECT_STRUCT_SIZE 64
#define LINK_STRUCT_SIZE 28

#define OBJECTS_PER_PAGE (PAGE_SIZE / OBJECT_STRUCT_SIZE)
#define LINKS_PER_PAGE (PAGE_SIZE / LINK_STRUCT_SIZE)

struct ag_main_header {
	uint16_t id;
	char description[41];
	uint8_t password[13];
	uint16_t first_free_obj;
	uint16_t last_alloc_obj;
	uint16_t first_free_link;
	uint16_t last_alloc_link;
	uint16_t last_free_obj;
	uint16_t last_free_link;
	uint8_t unknown0[444];
} __attribute__ ((packed));

static_assert(512 == sizeof(struct ag_main_header));


struct ag_extra_header {
	uint8_t unknown0[512];
} __attribute__ ((packed));

static_assert(512 == sizeof(struct ag_extra_header));


struct ag_object_struct {
	uint8_t unknown0[64];
} __attribute__ ((packed));

static_assert(OBJECT_STRUCT_SIZE == sizeof(struct ag_object_struct));


struct ag_link_struct {
	uint8_t unknown0[28];
} __attribute__ ((packed));

static_assert(LINK_STRUCT_SIZE == sizeof(struct ag_link_struct));

#endif
