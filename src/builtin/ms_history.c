/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_history.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 17:11:16 by marboccu          #+#    #+#             */
/*   Updated: 2024/05/22 15:38:49 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ms_history(t_var *mshell, t_list *args)
{
	t_list		*current;
	int			i;
	const int	len = lst_size(args);

	i = 1;
	current = mshell->history;
	if (len > 1)
	{
		ft_perror("history: too many arguments\n");
		*(mshell->status_code) = 1;
		return (KO);
	}
	while (current != NULL)
	{
		ft_printf("%5d %s\n", i, (char *)current->val);
		current = current->next;
		i++;
	}
	*(mshell->status_code) = 0;
	return (OK);
}
