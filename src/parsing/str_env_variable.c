/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_env_variable.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 20:35:57 by odudniak          #+#    #+#             */
/*   Updated: 2024/05/15 21:51:22 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ms_parsing.h>

bool	str_isvariable(char *s)
{
	int			i;
	bool		valid;

	valid = str_ilen(s) > 0;
	i = -1;
	while (valid && s && s[++i])
	{
		if (i == 0 && (!ft_isalpha(s[0]) && s[0] != '_'))
			return (false);
		else if (!ft_isalnum(s[i]) && s[i] != '_')
			return (false);
	}
	return (true);
}

int	str_var_ending_idx(char *s, int start)
{
	const int		len = str_ilen(s);
	int				i;

	if (start >= len)
		return (-1);
	if (s[start] == '$')
		start++;
	i = start;
	while (s && s[i])
	{
		if ((i == start && (!ft_isdigit(s[start]) || s[start] == '_'))
			|| (ft_isalnum(s[i]) || s[i] == '_'))
			;
		else
			return (i - 1);
		i++;
	}
	return (len - 1);
}