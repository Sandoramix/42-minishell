/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 14:33:51 by odudniak          #+#    #+#             */
/*   Updated: 2024/06/15 09:04:46 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H
# include <fcntl.h>
# include <limits.h>
# include <pthread.h>
# include <signal.h>
# include <stdarg.h>
# include <stdbool.h>
# include <stddef.h>
# include <stdint.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/time.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <time.h>
# include <unistd.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/ioctl.h>

# include <ft_structs.h>
# include <ft_colors.h>
# include <ft_errors.h>

# include <ft_char.h>
# include <ft_convert.h>
# include <ft_file.h>
# include <ft_gnl.h>
# include <ft_lists.h>
# include <ft_math.h>
# include <ft_mem.h>
# include <ft_printf.h>
# include <ft_str.h>
# include <ft_structs.h>
# include <ft_unistd.h>
# include <ft_system.h>

# define BASE10 "0123456789"
# define BASE16 "0123456789abcdef"
# define BASE16UPPER "0123456789ABCDEF"

# ifndef DEBUG
#  define DEBUG false
# endif

# ifndef FILE_HEREDOC
#  define FILE_HEREDOC ".heredoc"
# endif

# ifndef FILE_HEREDOC_PREFIX
#  define FILE_HEREDOC_PREFIX ".heredoc_"
# endif

typedef struct s_var	t_var;

/**
 * @brief General purpose Cleanup funcion
 * @param var gobal variable
 * @param doexit if the function should use `exit` or not
 * @param statuscode status code of the `exit` call.
 */
int		cleanup(t_var *var, bool doexit, int statuscode);

/**
 * @brief Check whether libft was compiled in debug mode or not.
 * @return `true` if it is, `false` otherwise.
 */
bool	ft_isdebug(void);

#endif
