#! /usr/bin/awk -f

BEGIN { sum = 0 }

{ sum = sum + $2}

END { print sum }
