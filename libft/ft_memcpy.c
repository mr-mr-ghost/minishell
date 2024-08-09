/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhoddy <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 14:52:20 by jhoddy            #+#    #+#             */
/*   Updated: 2024/02/21 12:56:35 by jhoddy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t				i;
	unsigned char		*cd;
	const unsigned char	*cs;

	cd = dest;
	cs = src;
	i = 0;
	if (!dest && !src)
		return (NULL);
	while (i < n)
	{
		cd[i] = cs[i];
		i++;
	}
	return (dest);
}
