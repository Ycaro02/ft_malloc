/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_fd.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfour <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/05 18:55:02 by nfour             #+#    #+#             */
/*   Updated: 2022/10/05 18:55:04 by nfour            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

#include  "../libft/libft.h"
# include <stdarg.h>

void	ft_put_lu_nbr(unsigned int nbr, int *count, int fd);
int		ft_count_digit(int nb);
int		ft_putlnbr(int nbr, int fd);
int		ft_putlstr(char *str, int fd);
int		ft_put_i_nbr(int nbr, int fd);
int		ft_printf_fd(int fd, const char *s, ...);
int		ft_display(const char *s, int i, va_list params, int fd);
int		ft_print_loop(const char *s, va_list params, int big_len, int fd);
int		ft_continue_display(const char *s, int i, va_list params, int fd);
void	ft_put_lx_nbr(unsigned int nbr, int *count, int fd);
void	ft_put_bigx_nbr(unsigned int nbr, int *count, int fd);
void	ft_put_p_nbr(unsigned long nbr, int *count, int fd);
void	ft_check_p(unsigned long nbr, int *count, int fd);
void	ft_put_size_t_nbr(size_t nbr, int *count, int fd);
// int     ft_putchar_fd(char c, int fd);
#endif
