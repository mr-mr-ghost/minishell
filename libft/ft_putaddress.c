/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putaddress.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhoddy <jhoddy@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 09:44:03 by jhoddy            #+#    #+#             */
/*   Updated: 2024/03/08 12:51:00 by jhoddy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	to_phex(void *ptr, char *cpy)
{
	unsigned long	address;
	char			*base;
	int				rmdr;
	int				len;
	int				i;

	address = (unsigned long)ptr;
	base = "0123456789abcdef";
	len = 0;
	i = 0;
	while (address != 0)
	{
		rmdr = address % 16;
		cpy[i++] = base[rmdr];
		address /= 16;
		len++;
	}
	return (len);
}

int	ft_putaddress(void *ptr)
{
	char			cpy[20];
	int				j;
	size_t			len;

	if (!ptr)
	{
		write(1, "(nil)", 5);
		return (5);
	}
	len = 0;
	len += ft_putstr("0x");
	len += to_phex(ptr, cpy);
	j = len - 3;
	while (j >= 0)
	{
		write(1, &cpy[j], 1);
		j--;
	}
	return (len);
}
