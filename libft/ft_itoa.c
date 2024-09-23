/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhoddy <jhoddy@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 12:46:25 by jhoddy            #+#    #+#             */
/*   Updated: 2024/03/07 13:19:42 by jhoddy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

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

static char	*cpy_nbr(int n, char *str, size_t len)
{
	size_t	i;
	int		sign;

	sign = 0;
	i = len - 1;
	if (n < 0)
	{
		sign = 1;
		n *= (-1);
	}
	str[len] = '\0';
	while (i > 0)
	{
		str[i] = '0' + n % 10;
		n /= 10;
		i--;
	}
	if (sign == 1)
		str[i] = '-';
	else
		str[i] = '0' + n % 10;
	return (str);
}

char	*ft_itoa(int n)
{
	char	*str;
	size_t	len;

	if (n == INT_MIN)
		return (ft_strdup("-2147483648"));
	len = nbr_len(n);
	str = malloc(len + 1);
	if (str == NULL)
		return (NULL);
	str = cpy_nbr(n, str, len);
	return (str);
}
