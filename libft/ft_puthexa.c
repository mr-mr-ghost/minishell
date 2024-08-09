/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_puthexa.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhoddy <jhoddy@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 16:11:11 by jhoddy            #+#    #+#             */
/*   Updated: 2024/03/08 11:28:39 by jhoddy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	hex_size(unsigned long nb)
{
	int				len;
	unsigned int	num;

	len = 0;
	num = (unsigned int)nb;
	while (num > 0)
	{
		num /= 16;
		len++;
	}
	return (len);
}

static void	to_hex(unsigned long nb, char format)
{
	char			*base;
	unsigned int	num;

	if (format == 'x')
		base = "0123456789abcdef";
	else if (format == 'X')
		base = "0123456789ABCDEF";
	num = (unsigned int)nb;
	if (num >= 16)
	{
		to_hex((num / 16), format);
		to_hex((num % 16), format);
	}
	else
		ft_putchar(base[num]);
}

int	ft_puthexa(unsigned long nb, char format)
{
	int	len;

	if ((unsigned int)nb == 0)
	{
		write(1, "0", 1);
		return (1);
	}
	else
	{
		to_hex(nb, format);
		len = hex_size(nb);
		return (len);
	}
}
