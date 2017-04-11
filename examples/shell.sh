#!/bin/bash

#
# Will output :
#
# 1
# 2
# 3
# 4
#

echo 1
../phook -e 'echo 2' -a 'echo 4'
sleep 1
echo 3
exit 0
