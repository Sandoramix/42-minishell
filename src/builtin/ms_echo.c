/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marboccu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 16:22:46 by marboccu          #+#    #+#             */
/*   Updated: 2024/05/18 13:27:08 by marboccu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int ms_echo(t_var *mshell, t_list *args)
{
	t_list *current;
	const int len = lst_size(args);

	current = args;
	if (len > 0)
		current = current->next;
	if (len > 1 && str_cmp(args->next->val, "-n") == 0)
		current = current->next;
	while (current != NULL)
	{
		ft_printf("%s", (char *)current->val);
		current = current->next;
		if (current != NULL)
			ft_printf(" ");
	}
	if (len == 1 || (len > 1 && str_cmp(args->next->val, "-n") != 0))
		ft_printf("\n");
	*mshell->status_code = 0;
	return (OK);
}
