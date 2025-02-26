#ifndef _AG_STRUCTS_H_
#define _AG_STRUCTS_H_

#include <assert.h>
#include <stdint.h>

#define PAGE_SIZE 512
#define OBJECT_STRUCT_SIZE 64
#define LINK_STRUCT_SIZE 28

#define OBJECTS_PER_PAGE (PAGE_SIZE / OBJECT_STRUCT_SIZE)
#define LINKS_PER_PAGE (PAGE_SIZE / LINK_STRUCT_SIZE)

#define LINKS_PAGE_PADDING (PAGE_SIZE - (LINKS_PER_PAGE * LINK_STRUCT_SIZE))

struct ag_main_header {
	uint16_t id;
	char description[41];
	uint8_t password[13];
	uint16_t first_free_obj;
	uint16_t last_obj_in_file;
	uint16_t first_free_link;
	uint16_t last_link_in_file;
	uint8_t unknown040[24];
	uint8_t unknown058[4];
	//uint16_t last_free_obj;
	//uint16_t last_free_link;
	uint16_t save_date;
	uint8_t unknown05e[2];
	uint16_t save_time;
	uint8_t unknown062[18];
	uint16_t category_count;
	uint8_t unknown076[242];
	double save_datetime;
	uint8_t unknown170[144];
	//414
} __attribute__ ((packed));

static_assert(512 == sizeof(struct ag_main_header));


struct ag_extra_header {
	uint8_t unknown000[512];
} __attribute__ ((packed));

static_assert(512 == sizeof(struct ag_extra_header));


struct ag_object_struct_base {
	uint16_t id;
	uint16_t type;
	uint8_t unknown04[60];
} __attribute__ ((packed));

static_assert(OBJECT_STRUCT_SIZE == sizeof(struct ag_object_struct_base));


struct ag_object_struct_item {
	uint16_t id;
	uint16_t type;
	uint16_t text_pointer;
	uint16_t note_pointer;
	uint8_t unknown08[10];
	uint16_t first_link;
	uint16_t last_link;
	uint8_t unknown16[28];
	char text[12];
	uint8_t unknown3e[2];
} __attribute__ ((packed));

static_assert(OBJECT_STRUCT_SIZE == sizeof(struct ag_object_struct_item));


struct ag_object_struct_text {
	uint16_t id;
	uint16_t type;
	uint16_t next;
	uint16_t prev;
	char text[56];
} __attribute__ ((packed));

static_assert(OBJECT_STRUCT_SIZE == sizeof(struct ag_object_struct_text));


struct ag_object_struct_category {
	uint16_t id;
	uint16_t type;
	uint16_t text_pointer;
	uint16_t note_pointer;
	uint8_t unknown08[20];
	uint16_t prev_sibling;
	uint16_t next_sibling;
	uint16_t parent;
	uint16_t first_child;
	uint16_t last_child;
	uint8_t unknown26[6];
	uint16_t last_search_node;
	uint8_t unknown2e[6];
	uint16_t category_type;
	uint16_t genealogy_object;
	uint8_t unknown38[8];
} __attribute__ ((packed));

static_assert(OBJECT_STRUCT_SIZE == sizeof(struct ag_object_struct_category));


struct ag_object_struct_genealogy {
	uint16_t id;
	uint16_t type;
	uint16_t genealogy[30];
} __attribute__ ((packed));

static_assert(OBJECT_STRUCT_SIZE == sizeof(struct ag_object_struct_genealogy));


struct ag_link_struct_base {
	uint16_t id;
	uint16_t type;
	uint8_t unknown04[24];
} __attribute__ ((packed));

static_assert(LINK_STRUCT_SIZE == sizeof(struct ag_link_struct_base));


struct ag_link_struct_link {
	uint16_t id;
	uint16_t type;
	uint16_t target;
	uint16_t prev_link;
	uint16_t next_link;
	uint16_t origin;
	uint8_t unknown0c[16];
} __attribute__ ((packed));

static_assert(LINK_STRUCT_SIZE == sizeof(struct ag_link_struct_link));


struct ag_link_struct_catsearch {
	uint16_t id;
	uint16_t type;
	uint16_t category;
	uint16_t next_node;
	uint16_t prev_node;
	uint16_t left_child;
	uint16_t right_child;
	uint16_t parent;
	uint8_t unknown10[6];
	uint8_t condition;
	uint8_t height;
	uint8_t unknown18[4];
} __attribute__ ((packed));

static_assert(LINK_STRUCT_SIZE == sizeof(struct ag_link_struct_catsearch));

#endif
