#ifndef RESOURCE_COPY_H
#define RESOURCE_COPY_H

void rom_to_ram(unsigned long start, unsigned long end, unsigned char *dst);
void rom_to_vram(unsigned long start, unsigned long end, unsigned int dst);

#endif
