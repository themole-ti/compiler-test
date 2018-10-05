#!/bin/bash
# Script to generate defines for resource locations in cartridge image

# Parameters
MAKEFILE=Makefile.res
MAPFILE=res.map
OUTFILE=$1

# First, generate map file via linker
make -f $MAKEFILE

# Create arrays
LOCATIONS=($(cat $MAPFILE | grep _binary_resources | cut -b 27-34))
NAMES=($(cat $MAPFILE | grep _binary_resources | cut -b 51-))

# Output file header
echo "#ifndef RESOURCE_DEFS_H" > $OUTFILE
echo "#define RESOURCE_DEFS_H" >> $OUTFILE
echo "" >> $OUTFILE
echo "#define RES_START(a) _binary_resources_ ## a ##_dat_start" >> $OUTFILE
echo "#define RES_END(a) _binary_resources_ ## a ##_dat_end" >> $OUTFILE
echo "" >> $OUTFILE

# Output defines to file
for i in "${!NAMES[@]}"; do
	echo -e "#define " "${NAMES[i]} \t" 0x"${LOCATIONS[i]}"l >> $OUTFILE
done

echo "" >> $OUTFILE
echo "#endif" >> $OUTFILE
