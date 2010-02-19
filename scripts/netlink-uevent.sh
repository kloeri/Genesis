#!/bin/bash

source $1 &> /dev/null

for sub in ${!SUBSCRIPTIONS_*};
do
    declare -r ${sub}
    for match in ${!sub};
    do
        echo "${sub/SUBSCRIPTIONS_/}, ${match}"
    done
done

exit 3
