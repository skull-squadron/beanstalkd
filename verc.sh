#!/bin/sh

echo '#include "dat.h"'
printf 'const char version[] = "'
./vers.sh
printf '";\n'
