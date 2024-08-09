/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putunsigned.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhoddy <jhoddy@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 15:52:03 by jhoddy            #+#    #+#             */
/*   Updated: 2024/03/05 11:08:44 by jhoddy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static size_t	unbr_len(unsigned int n)
{
	size_t	l;

	l = 0;
	if (n == 0)
		return (1);
	while (n != 0)
	{
		n /= 10;
		l++;
	}
	return (l);
}

int	ft_putunsigned(unsigned int nb)
{
	size_t	len;

	len = unbr_len(nb);
	if (nb > 9)
	{
		ft_putunsigned(nb / 10);
		ft_putunsigned(nb % 10);
	}
	else
		ft_putchar(nb + '0');
	return (len);
}
