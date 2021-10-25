#!/bin/bash
GREEN="\033[38;5;2m"
NORMAL="\033[38;5;255m"
RED="\033[38;5;1m"
BLUE="\033[38;5;4m"
PURPLE="\033[0;35m"
YELLOW="\033[0;33m"

######### INPUT VALIDATION TEST ##############
echo -e "${PURPLE}---- INPUT VALIDATION ---- ${NORMAL}"

echo -e "${BLUE}less than 4-5 arg 100 100 ${NORMAL}"
./philo 100 100

echo -e "${BLUE}some arguments aren’t integers: abc 100 200 300 ${NORMAL}"
./philo abc 100 200 300

echo -e "${BLUE}arguments equal or smaller than xero: 2 0 0 -5 ${NORMAL}"
./philo 2 0 0 -5

######### SOME TEST CASES ##############
echo -e "${PURPLE}\n---- SOME TEST CASES ---- ${NORMAL}"

echo -e "${RED}1 800 200 200 ${YELLOW} the philosopher should not eat and should die! ${NORMAL}"
./philo 1 800 200 200

echo -e "${RED}4 310 200 100 ${YELLOW} a philo should die ${NORMAL}"
./philo 4 310 200 100

echo -e "${RED}2 60 60 60 ${YELLOW} a philo should die ${NORMAL}"
echo -e "${GREEN}time difference of each death shouldnt be bigger than 10ms${NORMAL}"
./philo 2 60 60 60

echo -e "${RED}4 410 200 200 ${YELLOW} no philo should die ${NORMAL}"
# ./philo 4 410 200 200

echo -e "${RED}5 800 200 200 ${YELLOW} no philo should die ${NORMAL}"
# ./philo 5 800 200 200

echo -e "${RED}5 800 200 200 7 ${YELLOW} no philo should die \n ${NORMAL}"
echo -e "${YELLOW} the simulation should stop when all the philosopher haseaten at least 7 times each${NORMAL}"
# ./philo 5 800 200 200 7

# ./philo 5 800 200 200 7 > x.txt
# grep -w "1 is eating" x.txt | wc -l
# grep -w "2 is eating" x.txt | wc -l
# grep -w "3 is eating" x.txt | wc -l
# grep -w "4 is eating" x.txt | wc -l
# grep -w "5 is eating" x.txt | wc -l

## BIG NUMBERS?

# #no philosopher should die
# ./philo 2 210 100 100
# ./philo 80 1000 200 200 2

