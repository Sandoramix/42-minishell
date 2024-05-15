/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_history.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marboccu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 17:11:16 by marboccu          #+#    #+#             */
/*   Updated: 2024/05/13 17:18:00 by marboccu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int ms_history(t_var *mshell)
{
	t_list *current;
	int i;

	i = 1;
	current = mshell->history;
	if (!current)
	{
		ft_fprintf(2, "history: No history\n");
		*mshell->status_code = 1;
		return (KO);
	}
	while (current != NULL)
	{
		ft_printf("%d %s\n", i, (char*)current->val);
		current = current->next;
		i++;
	}
	*mshell->status_code = 0;
	return (OK);
}
