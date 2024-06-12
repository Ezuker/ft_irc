/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcarolle <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 15:32:46 by bcarolle          #+#    #+#             */
/*   Updated: 2024/06/12 15:39:39 by bcarolle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "global.hpp"
# include "Client.hpp"

class Channel
{
	public:

	private:
		std::vector<Client *> _clients;
		std::vector<Client *> _operators;
};

#endif
