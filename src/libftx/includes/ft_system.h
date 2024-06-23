/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_system.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 15:46:10 by odudniak          #+#    #+#             */
/*   Updated: 2024/06/23 22:52:48 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SYSTEM_H
# define FT_SYSTEM_H

# include <libft.h>

typedef enum e_timeunit
{
	SECONDS,
	MILLISECONDS,
	NANOSECONDS
}	t_timeunit;

typedef struct timeval	t_time;
typedef struct timeval	t_timeval;

/**
 * @brief Generate an array of all setted `PATH` paths0
 * @param env node of the `PATH` environment variable
 * @return mtx of the paths, or `NULL` if an error occurred.
 */
char		**env_load_paths(t_list *env);
/**
 * @brief Generate the list of env variables.
 * @param envp third paramether passed to main
 * @return list of the environment variables.
 * `NULL` if there's been an allocation error.
 */
t_list		*env_load(char **envp);

/**
 * @brief Find the absolute path of a `cmd` command.
 * @param paths array of path where to lookup the given `cmd`
 * @param cmd the requested command
 * @return absolute path of the `cmd` or if it doesn't exist a `NULL`
 * @attention Uses: `malloc`
 *  `free`
 */
char		*sys_findcmdpath(char **paths, char *cmd);

/**
 * @brief Get current timestamp
 * @param unit show timestamp in `unit` units
 * (`SECONDS` | `MILLISECONDS` | `NANOSECONDS`).
 * @return Timestamp as a number.
*/
t_ulong		timestamp(t_timeunit unit);

/**
 * @brief Custom sleep function
 * @param value value for the sleep call
 * @param unit unit of the `value` (`SECONDS` | `MILLISECONDS` | `NANOSECONDS`).
*/
void		ssleep(t_ulong value, t_timeunit unit);

#endif
