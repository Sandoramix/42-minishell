/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_history.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 17:11:16 by marboccu          #+#    #+#             */
/*   Updated: 2024/06/22 11:10:54 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

t_state	ms_history(t_var *mshell, t_list *args)
{
	t_list		*current;
	int			i;
	const int	len = lst_size(args);

	i = 1;
	current = mshell->history;
	if (len > 1)
		return (g_set_status(1),
			ft_perror("history: too many arguments\n"), KO);
	while (current != NULL)
	{
		ft_printf("%5d %s\n", i, current->val);
		current = current->next;
		i++;
	}
	return (g_set_status(0), OK);
}
