#!/bin/bash

# settings --------------

CHECK_STDOUT=1
CHECK_STDERR=1

TEST_CASES=(
	'echo "lol"'
	'echo $PATH'
	'pwd'
	'GEEN_COMMAND'
	'cat tmp_test | grep best'
	'cat tmp_test | grep test | grep best'
	'cat tmp_test | grep GEEN_COMMAND'
	'cat tmp_test | grep GEEN_COMMAND | grep best'
	'cat tmp_test | grep test | grep GEEN_COMMAND'
	'date'
	'date > tmp_redirect'
	'cat tmp_redirect'
	'ls >tmp_redirect'
	'cat tmp_redirect'
	'pwd>tmp_redirect'
	'cat tmp_redirect'
	'< tmp_test cat'
	'<'
	'< cat'
	'< tmp_test'
)

MINISHELL="./minishell"

# -----------------------

echo "this is tmp_test" > tmp_test
echo "the best test ever" >> tmp_test
echo

RED="\033[0;31m"
GREEN="\033[0;32m"
RESET="\033[0m"

run_tests()
{
	bash -c "$TEST" > tmp_bash_stdout 2> tmp_bash_stderr
	(echo "$TEST"; echo "exit") | $MINISHELL > tmp_minishell_stdout 2> tmp_minishell_stderr
}
set_vars()
{
	BASH_STDOUT=$(cat -e tmp_bash_stdout)
	MINISHELL_STDOUT=$(cat -e tmp_minishell_stdout | tail -n +2 | head -n -1)
	BASH_STDERR=$(cat -e tmp_bash_stderr)
	MINISHELL_STDERR=$(cat -e tmp_minishell_stderr | tail -n +2 | head -n -1)
}
stdout_matches()
{
	if [[ "$BASH_STDOUT" == "$MINISHELL_STDOUT" ]]; then
		echo -e "${GREEN}stdout OK\t$TEST${RESET}"
		return 1
	fi
}
stderr_matches()
{
	if [[ "$BASH_STDERR" == "$MINISHELL_STDERR" ]]; then
		echo -e "${GREEN}stdERR OK\t$TEST${RESET}"
		return 1
	fi
}
show_stdout()
{
	echo -e "---------------------------------"
	echo -e "stdout\t\t$TEST"
	echo -e "\n${RED}BASH"
	echo "$BASH_STDOUT"
	echo -e "\nMINISHELL"
	echo -e "$MINISHELL_STDOUT${RESET}"
	echo -e "\n---------------------------------"
}
show_stderr()
{
	echo -e "---------------------------------"
	echo -e "stdERR\t\t$TEST"
	echo -e "\n${RED}BASH"
	echo "$BASH_STDERR"
	echo -e "\nMINISHELL"
	echo -e "$MINISHELL_STDERR${RESET}"
	echo -e "\n---------------------------------"
}

for TEST in "${TEST_CASES[@]}"; do
	run_tests
	set_vars
	if [[ "$CHECK_STDOUT" == 1 ]]; then 
		stdout_matches
		[[ $? != 1 ]] && show_stdout
	fi
	if [[ "$CHECK_STDERR" == 1 ]]; then 
		stderr_matches
		[[ $? != 1 ]] && show_stderr
	fi
done

do_cleanup()
{
	rm tmp_bash_stdout \
		tmp_bash_stderr \
		tmp_minishell_stdout \
		tmp_minishell_stderr \
		tmp_test \
		tmp_redirect
}
do_cleanup
