/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhoddy <jhoddy@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 17:09:32 by jhoddy            #+#    #+#             */
/*   Updated: 2024/03/04 17:49:48 by jhoddy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static size_t	nbr_len(int n)
{
	size_t	l;

	l = 0;
	if (n == INT_MIN)
		return (11);
	if (n == 0)
		return (1);
	if (n < 0)
	{
		n *= (-1);
		l++;
	}
	while (n != 0)
	{
		n /= 10;
		l++;
	}
	return (l);
}

int	ft_putnbr(int nb)
{
	size_t	len;

	ft_putnbr_fd(nb, 1);
	len = nbr_len(nb);
	return (len);
}
