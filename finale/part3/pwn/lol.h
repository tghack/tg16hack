#ifndef _LOL_H
#define _LOL_H

#define MAX_USERS 3
#define LOL_ALLOC 0x1
#define LOL_DEBUG 0x2
#define LOL_ADD_USER 0x3
#define LOL_DEL_USER 0x4

struct lol_alloc {
	size_t size;
};

#endif /* _LOL_H */
