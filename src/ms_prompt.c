/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_prompt.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 12:40:27 by marboccu          #+#    #+#             */
/*   Updated: 2024/05/29 21:23:00 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ms_prompt(t_var *mshell)
{
	char	*input;

	input = readline(PROMPT " ");
	while (input)
	{
		if (str_ilen(input) > 0)
		{
			add_history(input);
			add_cmd_history(mshell, input);
			parse_and_exec(mshell, input);
		}
		free(input);
		input = readline(PROMPT " ");
	}
	cleanup(mshell, true, 0);
}
