#!/bin/bash
#	use:
#	turn on valgrind
#	./autocompile.sh 1
SRCS="src/*/*.c"
PROGRAM="minishell"
VALG=$1
OLD_MOD=0
GREEN='\033[42m'
RED='\033[41m'
NEUTRAL='\033[0m'
no_leak()
{
	echo -e "${GREEN}No Leaks${NEUTRAL}"
}
yes_leak()
{
	echo -e "${RED}Leaks${NEUTRAL}"
}
lets_execute()
{
	./$PROGRAM
}
lets_compile()
{
	make || make re
}
lets_valgrind()
{
	valgrind --leak-check=full \
			--show-leak-kinds=all \
			--track-origins=yes \
			./$PROGRAM
}
lets_cleanup()
{
	make fclean
	rm valg.log
	rm $PROGRAM
	exit
}
while true; do
	# for FILE in $SRCS; do
		# NEW_MOD=$(stat -c %Y $FILE)
		# if [[ "$NEW_MOD" > "$OLD_MOD" ]];then 
		# 	OLD_MOD=$NEW_MOD
			clear
			lets_compile
			if [[ "$?" == 0 ]]; then
				lets_valgrind &> valg.log
				grep "no leaks" valg.log
				if [[ "$?" == 0 ]]; then
					lets_execute
					no_leak
				else
					[[ "$VALG" ]] && lets_valgrind || lets_execute
					yes_leak
				fi
			fi
		# fi
	# done
	# read -t 2 CLEAN && [[ "$CLEAN" ]] && lets_cleanup 
done
