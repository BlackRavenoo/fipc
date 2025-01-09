#!/bin/bash

PALETTES_DIR="palettes"
INCLUDE_DIR="include/palettes"
MAP_FILE="include/palettes_map.h"

mkdir -p $INCLUDE_DIR

rm -f $INCLUDE_DIR/*.h
rm -f $MAP_FILE

for file in $PALETTES_DIR/*.json; do
    filename=$(basename -- "$file")
    name="${filename%.*}"
    header_file="$INCLUDE_DIR/$name.h"

    xxd -i "$file" >> $header_file
done

echo "#include <map>" >> $MAP_FILE
echo "#include <string>" >> $MAP_FILE
echo "" >> $MAP_FILE

for file in $PALETTES_DIR/*.json; do
    filename=$(basename -- "$file")
    name="${filename%.*}"
    echo "#include \"$name.h\"" >> $MAP_FILE
done

echo "" >> $MAP_FILE
echo "std::map<std::string, std::pair<const unsigned char*, size_t>> palettes = {" >> $MAP_FILE

for file in $PALETTES_DIR/*.json; do
    filename=$(basename -- "$file")
    name="${filename%.*}"
    var_name="palettes_${name}_json"
    len_name="palettes_${name}_json_len"
    echo "    {\"$name\", {$var_name, $len_name}}," >> $MAP_FILE
done

echo "};" >> $MAP_FILE