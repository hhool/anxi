#/bin/bash
# Convert gbk to utf-8 for all files in the path and subpath
# keep the original content not changed with the same file name under utf8
# Author: hhool
# Date: 2024-06-01
# Usage: gbk2utf.sh path
# Example: gbk2utf.sh /home/hhool/common

if [ $# -ne 1 ]; then
    echo "Usage: $0 path"
    exit 1
fi

path=$1
if [ ! -d $path ]; then
    echo "Error: $path is not a directory"
    exit 1
fi

# find all files in the path and subpath with type f
# and .cpp .h extension
files=$(find $path -type f -name "*.cpp" -o -name "*.h" -o -name "*.c")

# output the files for debug
echo "The files to be converted to utf-8 are:"
echo $files

# convert gbk to utf-8 for all files
for file in $files; do
    # check file is encoded with gbk
    # if not, skip it.
    file $file | grep -q "UTF-8" && continue
    # convert gbk to utf-8
    iconv -f gbk -t utf-8 $file > $file.utf8
    # keep the original content not changed with the same file name under utf8
    mv $file.utf8 $file

    # check the result
    if [ $? -eq 0 ]; then
        echo "Convert $file to utf-8 successfully"
    else
        echo "Error: convert $file to utf-8 failed"
    fi
done

echo "Convert all gbk files to utf-8 successfully"

