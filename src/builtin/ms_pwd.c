/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marboccu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 14:36:35 by marboccu          #+#    #+#             */
/*   Updated: 2024/05/07 13:07:05 by marboccu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	*ms_pwd(t_var *mshell, t_list *args)
{
	char		*pwd;

	if (!args || lst_size(args) > 1)
	{
		ft_fprintf(2, "pwd: too many arguments\n");
		*mshell->status_code = 1;
		return (NULL);
	}
	pwd = getcwd(mshell->curr_path, mshell->curpath_len);
	if (!pwd && errno == ERANGE)
	{
		mshell->curpath_len *= 2;
		free(mshell->curr_path);
		mshell->curr_path = ft_calloc(mshell->curpath_len, sizeof(char));
		if (!mshell->curr_path)
			return (pf_errcode(ERR_MALLOC), cleanup(mshell, true, 1), NULL);
		return (ms_pwd(mshell, args), NULL);
	}
	if (str_cmp((char *)args->val, "pwd") == 0)
		printf("%s\n", mshell->curr_path);
	else
		ft_fprintf(2, "%s: command not found\n", (char *)args->val);
	
	return (NULL);
}

// int	main(int ac, char **av, char **envp)
// {
// 	t_var			mshell;
// 	t_list			*args;

// 	(void)av;
// 	(void)ac;
// 	mshell = (t_var){0};
// 	args = cmd_parse_new("pwd");
// 	if (ac != 1)
// 		return (pf_errcode(ERR_INVALID_ARGC), cleanup(&mshell, true, 1), 1);
// 	mshell._main.envp = envp;
// 	ms_init(&mshell);
// 	ms_pwd(&mshell, args);
// 	return (cleanup(&mshell, true, 0));
// }

