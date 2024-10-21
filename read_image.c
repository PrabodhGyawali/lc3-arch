#include "read_image.h"

void read_image_file(FILE* file) {
	/* the origin tells us where in memory to place the image */
	uint16_t origin;
	fread(&origin, sizeof(origin), 1, file);
	origin = swap16(origin);

	/* we know the maximum file size so we only need one freed */
	uint16_t max_read = MEMORY_MAX - origin;
	uint16_t* p = memory + origin;
	size_t read = fread(p, sizeof(uint16_t), max_read, file);

	/* swap to little endian */
	while (read-- > 0) {
		*p = swap16(*p);
		++p;
	}
}
