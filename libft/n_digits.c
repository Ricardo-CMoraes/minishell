/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   n_digits.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdcm <rdcm@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 23:16:15 by rdcm              #+#    #+#             */
/*   Updated: 2025/10/04 17:00:45 by rdcm             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

unsigned long long	n_digits(unsigned long long n, size_t base)
{
	size_t	count;

	count = 0;
	if (n == 0)
		return (1);
	while (n != 0)
	{
		count += 1;
		n /= base;
	}
	return (count);
}
