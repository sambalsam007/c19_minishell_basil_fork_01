#!/bin/bash

# settings ------------
SHOW_ALL_TESTS=1
TEST_CASES=(
	# 'pipe:cat .xtmp/grep | grep second'
	# 'pipe:cat .xtmp/grep | grep first'
	# 'x:asdf'
	# 'x:echo "lol"'
	# 'x:echo $PATH'
	# 'x:pwd'
	# 'x:GEEN_COMMAND'
	# 'pipe:cat .xtmp/grep | grep second'
	# 'pipe:cat .xtmp/grep | grep second | grep line'
	# 'pipe:cat .xtmp/grep | grep second | grep first'
	# 'pipe:cat .xtmp/grep | grep GEEN_COMMAND'
	# 'pipe:cat .xtmp/grep | grep GEEN_COMMAND | grep second'
	# 'pipe:cat .xtmp/grep | grep second | grep GEEN_COMMAND'
	# 'x:date'
	# 'redir:date > .xtmp/redirect'
	# 'redir:ls >.xtmp/redirect'
	# 'redir:pwd>.xtmp/redirect'
	# 'x:< .xtmp/grep cat'
	# 'x:<'
	# 'x:< cat'
	# 'x:< .xtmp/grep'
	'exit'
	'exit 1'
	'exit -1'
	'exit 0'
	'exit   '
	'exit   asdfl'
	'exitasdfl'
	'exit | asdfl'
	'exit | ls'
)
# ---------------------

# create test files
mkdir -p .xtmp
echo "first line" > .xtmp/grep
echo "second line" >> .xtmp/grep

# colors
GREEN='\033[0;32m'
RED='\033[0;31m'
RESET='\033[0m'

# set environment
export LSAN_OPTIONS=suppressions=readlineleaks.supp:print_suppressions=0

# run tests
for TEST in "${TEST_CASES[@]}"; do

    TYPE=${TEST%%:*}      # Extract type (everything before :)
    COMMAND=${TEST#*:}    # Extract command (everything after :)

	# run tests
	BASH_OUT=$(bash -c "$COMMAND" 2>/dev/null)
	BASH_ERR=$(bash -c "$COMMAND" 2>&1 1>/dev/null)
	if [[ "$TYPE" == "pipe" ]];then
		# MSHELL_OUT=$(script -q -c "echo '$COMMAND' | ./minishell" /dev/null | tail -n +3 | head -n -2 )
		bash -c "$COMMAND" >.xtmp/bash_pipe_out
		BASH_OUT=$(cat .xtmp/bash_pipe_out)
		script -q -c "echo '$COMMAND' | ./minishell" /dev/null | tail -n +3 | head -n -2 >.xtmp/mshell_pipe_out
		MSHELL_OUT=$(cat .xtmp/mshell_pipe_out)
		bash -c "$COMMAND" 2>.xtmp/bash_pipe_err 1>/dev/null
		BASH_ERR=$(cat .xtmp/bash_pipe_err)
		script -q -c "echo '$COMMAND' | ./minishell" /dev/null | tail -n +3 | head -n -2 2>.xtmp/mshell_pipe_err 1>/dev/null
		MSHELL_ERR=$(cat .xtmp/mshell_pipe_err)
	elif [[ "$TYPE" == "redir" ]];then
		echo "$COMMAND" | ./minishell 1>/dev/null 2>&1
		MSHELL_OUT=$(cat .xtmp/redirect)
		bash -c "$COMMAND" 1>/dev/null 2>&1
		BASH_OUT=$(cat .xtmp/redirect)
	else
		MSHELL_OUT=$(echo "$COMMAND" | ./minishell 2>/dev/null | tail -n +2 | head -n -1 )
		MSHELL_ERR=$(echo "$COMMAND" | ./minishell 2>&1 1>/dev/null | tail -n +2 | head -n -1 )
	fi

	if [[ "$SHOW_ALL_TESTS" == 1 ]];then
		echo -e "--------------------------------\n"
		echo $COMMAND
		echo
		echo bash out
		echo $BASH_OUT
		echo
		echo minishell out
		echo $MSHELL_OUT
		echo
		echo bash err
		echo $BASH_ERR
		echo
		echo minishell err
		echo $MSHELL_ERR
		echo
		continue
	fi

	# show results stdout
	# if diff -b <(echo "$BASH_OUT") <(echo "$MSHELL_OUT") >/dev/null; then
	# 	echo "Outputs are the same (ignoring whitespace)"
	# else
	# 	echo "Outputs differ"
	# fi
	# if diff -b .xtmp/mshell_pipe_out .xtmp/bash_pipe_out; then
	# 	echo content, the same only whitespace difference
	# fi
	if diff -b <(echo "$BASH_OUT") <(echo "$MSHELL_OUT") >/dev/null; then
		echo -ne "${GREEN}"
		echo -e "stdout ok\t$COMMAND"
		echo -ne "${RESET}"
	else
		echo "-------------------------------------"
		echo -e "stdout\t\t$COMMAND"
		echo -e "\nbash"
		echo -e "${RED}$BASH_OUT${RESET}"
		echo -e "\nmshell"
		echo -e "${RED}$MSHELL_OUT${RESET}"
		echo "-------------------------------------"
	fi

	# show results stderr
	if [[ "$BASH_ERR" == "$MSHELL_ERR" ]]; then
		echo -ne "${GREEN}"
		echo -e "stdERR ok\t$COMMAND"
		echo -ne "${RESET}"
	else
		echo "-------------------------------------"
		echo -e "stdERR\t\t$COMMAND"
		echo -e "\nbash"
		echo -e "${RED}${BASH_ERR}%${RESET}"
		echo -e "\nmshell"
		echo -e "${RED}${MSHELL_ERR}%${RESET}"
		echo "-------------------------------------"
	fi
done

# cleanup
# read -p "cleanup" YN
# [[ "$YN" != "y" ]] && exit
rm -r .xtmp
