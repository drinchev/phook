#!/usr/bin/perl

#
# Will output :
#
# 1
# 2
# 3
# 4
#

print "1\n";
system("../phook", "-e", "echo 2", "-a", "echo 4");
sleep 1;
print "3\n";
exit 0;
