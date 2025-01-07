#!/bin/bash

# settings --------------

CHECK_STDOUT=1
CHECK_STDERR=1

TEST_CASES=(
	'echo "lol"'
	'echo $PATH'
	'pwd'
	'GEEN_COMMAND'
	'cat .xtmp/test | grep best'
	'cat .xtmp/test | grep test | grep best'
	'cat .xtmp/test | grep GEEN_COMMAND'
	'cat .xtmp/test | grep GEEN_COMMAND | grep best'
	'cat .xtmp/test | grep test | grep GEEN_COMMAND'
	'date'
	'date > .xtmp/redirect'
	'cat .xtmp/redirect'
	'ls >.xtmp/redirect'
	'cat .xtmp/redirect'
	'pwd>.xtmp/redirect'
	'cat .xtmp/redirect'
	'< .xtmp/test cat'
	'<'
	'< cat'
	'< .xtmp/test'
)

MINISHELL="./minishell"

# -----------------------

mkdir -p .xtmp

echo "this is .xtmp/test" > .xtmp/test
echo "the best test ever" >> .xtmp/test
echo

RED="\033[0;31m"
GREEN="\033[0;32m"
RESET="\033[0m"

run_tests()
{
	bash -c "$TEST" > .xtmp/bash_stdout 2> .xtmp/bash_stderr
	(echo -e "$TEST"; echo "exit") | $MINISHELL > .xtmp/minishell_stdout 2> .xtmp/minishell_stderr
}
set_vars()
{
	BASH_STDOUT=$(cat -e .xtmp/bash_stdout)
	MINISHELL_STDOUT=$(cat -e .xtmp/minishell_stdout | tail -n +2 | head -n -1)
	BASH_STDERR=$(cat -e .xtmp/bash_stderr)
	MINISHELL_STDERR=$(cat -e .xtmp/minishell_stderr | tail -n +2 | head -n -1)
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
	rm -r .xtmp
}
do_cleanup
