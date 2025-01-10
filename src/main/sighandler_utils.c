/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sighandler_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 16:49:17 by bclaeys           #+#    #+#             */
/*   Updated: 2025/01/10 17:18:11 by bclaeys          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include <signal.h>

void	set_sigaction_for_heredoc(struct sigaction *signal_struct_sigint,
									struct sigaction *signal_struct_sigquit,
									void (*handle_signal_heredoc)
									(int, siginfo_t *, void *))
{
	signal_struct_sigint->sa_sigaction = handle_signal_heredoc;
	signal_struct_sigquit->sa_sigaction = 0;
}

void	set_sigaction_for_main(struct sigaction *signal_struct_sigint,
									struct sigaction *signal_struct_sigquit,
									void (*handle_signal_parent)
									(int, siginfo_t *, void *))

{
	signal_struct_sigquit->sa_handler = SIG_DFL;
	signal_struct_sigint->sa_sigaction = handle_signal_parent;
}

void	set_sigaction_for_exec(struct sigaction *signal_struct_sigint,
									struct sigaction *signal_struct_sigquit,
									void (*handle_signal_child)
									(int, siginfo_t *, void *))
{
	signal_struct_sigint->sa_sigaction = handle_signal_child;
	signal_struct_sigint->sa_sigaction = 0;
	signal_struct_sigquit->sa_handler = SIG_IGN;
}
