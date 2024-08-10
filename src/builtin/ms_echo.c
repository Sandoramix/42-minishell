/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 16:22:46 by marboccu          #+#    #+#             */
/*   Updated: 2024/08/10 08:54:51 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

/**
 * @brief Function that checks whether the given char `c`
 * is a valid flag value for `echo` command
 * @param c character to check
 * @return `true` if the `c` character is allowed flag, `false` otherwise.
 * @note the subject mentions that we have only to implement `-n`,
 * but if needed we can extend the allowed flags (`e` and `E`)
 * without implementing their features only for the combination of the flags
 * which are handled in bash (e.g. "echo -enEEnnnn hello" > "hello%")
 * @example
 * ```
 * return (c == 'n' || c == 'e' || c == 'E');
 * ```
 */
static bool	ms_valid_echo_flag(char c)
{
	return (c == 'n');
}

/**
 * @brief Skip possible -n... flags, because of a weird args like:
 * `echo -nnn -n -n -n hello world`
 * @param args pointer to commands's args,
 * including the command's node ("echo")
 * @return `true` if the echo command should print a `\n` at the end,
 * `false` otherwise
 */
static bool	skip_flags(t_list **args)
{
	t_list	*curr;
	bool	result;

	result = true;
	curr = *args;
	if (curr && str_equals(curr->val, "echo"))
		curr = curr->next;
	while (curr && str_startswith(curr->val, "-") && str_ilen(curr->val) > 1)
	{
		if (str_every(curr->val + 1, ms_valid_echo_flag))
			result = false;
		else
			break ;
		curr = curr->next;
	}
	*args = curr;
	return (result);
}

t_state	ms_echo(t_var *mshell, t_list *args)
{
	t_list		*current;
	bool		should_nl;

	(void)mshell;
	current = args;
	should_nl = skip_flags(&current);
	dbg_printf(CGRAY"[echo] should print \\n: %s\n"CR, ft_boolstr(should_nl));
	while (current != NULL)
	{
		ft_printf("%s", (char *)current->val);
		current = current->next;
		if (current != NULL)
			ft_printf(" ");
	}
	if (should_nl)
		ft_printf("\n");
	return (setstatus(mshell, 0), OK);
}
