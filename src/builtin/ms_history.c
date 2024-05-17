/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_history.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marboccu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 17:11:16 by marboccu          #+#    #+#             */
/*   Updated: 2024/05/17 16:51:47 by marboccu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ms_history(t_var *mshell, t_list *args)
{
	t_list	*current;
	int		i;
	const int len = lst_size(args);

	i = 1;
	current = mshell->history;
	if (len > 1)
	{
		ft_perror("history: too many arguments\n");
		*mshell->status_code = 1;
		return (KO);
	}
	// FIXME Serve?
	//if (!current)
	//{
	//	ft_perror("history: No history\n");
	//	*mshell->status_code = 1;
	//	return (KO);
	//}
	while (current != NULL)
	{
		ft_printf("%d %s\n", i, (char *)current->val);
		current = current->next;
		i++;
	}
	*mshell->status_code = 0;
	return (OK);
}
