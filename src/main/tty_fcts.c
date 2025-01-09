/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tty_fcts.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 13:09:23 by bclaeys           #+#    #+#             */
/*   Updated: 2025/01/07 13:10:48 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	restore_tty(t_var_data *var_data)
{
	if (tcsetattr(STDIN_FILENO, TCSANOW, &var_data->original_termios) == -1)
		return (ft_printf_fd(2, "Err: tcsetattr\n"), 1);
	return (0);
}

int	handle_signals_through_termios(t_var_data *var_data)
{
	struct termios	termios_p;

	if ((var_data->termios_backup_check))
		return (0);
	if (tcgetattr(STDIN_FILENO, &var_data->original_termios) == -1)
		return (ft_printf_fd(2, "Err: tcgetattr\n"), 1);
	termios_p = var_data->original_termios;
	termios_p.c_cc[VQUIT] = -1;
	termios_p.c_lflag &= ~ECHOCTL;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &termios_p) == -1)
		return (ft_printf_fd(2, "Err: tcsetattr\n"), 1);
	return (0);
}
