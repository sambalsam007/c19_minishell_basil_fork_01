#!/bin/bash

echo "this is tmp_test" > tmp_test
echo "the best test ever" >> tmp_test
echo

[[ "$1" == 1 ]] && SHOW_STDERR=1
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
}
compare_outputs()
{
	if [[ "$BASH_STDOUT" == "$MINISHELL_STDOUT" ]]; then
		echo -e "${GREEN}stdout OK\t$TEST${RESET}"
		return 1
	fi
}
show_difference()
{
	echo
	echo -e "$TEST"

	echo
	echo -e "${RED}BASH stdout"
	echo "$BASH_STDOUT"

	echo
	echo -e "MINISHELL stdout"
	echo -e "$MINISHELL_STDOUT${RESET}"
	echo
}
show_stderr()
{
	if [[ "$SHOW_STDERR" == 1 ]]; then
		echo -e "${RED}BASH stderr"
		cat tmp_bash_stderr

		echo -e "\nMINISHELL stderr"
		cat tmp_minishell_stderr
		echo -e "${RESET}"
	fi
}
for TEST in "${TEST_CASES[@]}"; do

	run_tests
	set_vars
	compare_outputs
	[[ $? == 1 ]] && continue
	show_difference
	show_stderr
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
