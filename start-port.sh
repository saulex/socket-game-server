#!/bin/bash
# set counter 'c' to 1 and condition 
# c is less than or equal to 5
for (( c=8085; c<=8915; c = c +2 ))
do
      ./socket_cpp $c &
done
