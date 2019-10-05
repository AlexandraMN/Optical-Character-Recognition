#!/bin/bash


MESSAGE_SIZE=22
EXEC=ex
TIME=120
BONUS_POINTS=20
ERRORS_NUMBER=0
MAX_BONUS=10
TWO=2
ONE=1
CODING_STYLE_BONUS=0

make


if test ! -x $EXEC; then
    echo "Execf file missing"
    exit 1
fi

sum=0

echo
echo "testing same_class"
points=$(cd resources; ./unit_testing_checker same_class)
return=$?

if [[ $return -eq 0 ]]
then
    sum=$(($sum + $points ))
else
    echo "encountered segfault in same_class: 0/10"
fi
echo



echo "testing get_random_samples"
points=$(cd resources; ./unit_testing_checker get_random_samples)
return=$?

if [[ $return -eq 0 ]]
then
    sum=$(($sum + $points ))
else
    echo "encountered segfault in get_random_samples: 0/10"
fi
echo



echo "testing random_dimensions"
points=$(cd resources; ./unit_testing_checker random_dimensions)
return=$?

if [[ $return -eq 0 ]]
then
    sum=$(($sum + $points ))
else
    echo "encountered segfault in random_dimensions: 0/10"
fi
echo



echo "testing compute_unique"
points=$(cd resources; ./unit_testing_checker compute_unique)
return=$?

if [[ $return -eq 0 ]]
then
    sum=$(($sum + $points ))
else
    echo "encountered segfault in compute_unique: 0/10"
fi
echo



echo "testing get_split_as_indexes"
points=$(cd resources; ./unit_testing_checker get_split_as_indexes)
return=$?

if [[ $return -eq 0 ]]
then
    sum=$(($sum + $points ))
else
    echo "encountered segfault in get_split_as_indexes 0/10"
fi
echo

echo "total unit testing points: $sum/50"

echo

echo "testing precision"
timeout $TIME ./ex > result

if [ "$?" == "124" ]
then
    echo "TIMEOUT [$TIME s]"
else
    typeset -i points=$(cat result)
    echo "Algorithm points: $points/30"
    sum=$(($sum + $points ))
fi

echo
rm result
make clean
echo

function checkBonus
{
    printf '%*s\n' "${COLUMNS:-$(($(tput cols) - $ONE))}" '' | tr ' ' -
    echo "" > checkstyle.txt

    echo -ne "Coding style Errors:\n"
    for entry in *.{cpp,hpp,h};
    do
        if [[ -f $entry ]]; then
            ./cpplint.py "$entry" > checkstyle.txt
            YOUR_BONUS=`cat checkstyle.txt | grep "Total errors found: "| tr -dc '0-9'`
            ERRORS_NUMBER=$(( $ERRORS_NUMBER + $YOUR_BONUS ))
        fi
    done

    if [[ $ERRORS_NUMBER != "0" ]]; then
        printf '\nCoding style check: %*sFAIL' "${COLUMNS:-$(($(tput cols) - $MESSAGE_SIZE))}" '' | tr ' ' -
        echo -ne "\n\t\tYou have a total of $ERRORS_NUMBER coding style errors.\n"

    else
        echo -ne "\n\tNone?\n\t\tWow, next level!"
        printf '\n\nCoding style check: %*sOK' "${COLUMNS:-$(($(tput cols) - $MESSAGE_SIZE + $TWO ))}" '' | tr ' ' -

        CODING_STYLE_BONUS=$(echo "scale=1; $sum / 8" | bc -l)
        echo "coding style bonus: $CODING_STYLE_BONUS"
        echo "$CODING_STYLE_BONUS" | grep '^\.' 2>&1 > /dev/null
        if [[ $? -eq "0" ]]; then
            CODING_STYLE_BONUS=0
        else
            CODING_STYLE_BONUS="$(echo "$CODING_STYLE_BONUS" |  cut -f1 -d'.')"
        fi
        sum=$(( $sum + $CODING_STYLE_BONUS ))

        # CODING_STYLE_BONUS=$(($CODING_STYLE_BONUS))
    fi
    rm -f checkstyle.txt
}

function checkReadme
{
    echo "Checking existence of README file..."
    ls | grep -i '^readme' 2>&1 > /dev/null
    if [[ "$?" -eq "0" && "$sum" -gt "0" ]]; then
        echo "README file found: 10/10"
        sum=$(( $sum + 10 ))
    else
        echo "No README file found 0/10"
    fi
    echo
}

function printBonus
{
    if [[ $ERRORS_NUMBER -gt 0 ]]; then
    printf '\n%*s' "${COLUMNS:-$(tput cols)}" '' | tr ' ' -
        echo -ne  "\n\tAnd you almost did it!\n"
    else
        echo -ne "\n\tDamn!\n"
    fi
    echo -ne "\n\t\tYou get $CODING_STYLE_BONUS/$MAX_BONUS for coding style.\n\n"
}

echo

checkReadme
checkBonus
printBonus

echo "TOTAL: $sum/100"


echo
