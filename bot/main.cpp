/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doley <doley@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 13:09:00 by ndelhota          #+#    #+#             */
/*   Updated: 2025/10/18 14:48:31 by doley            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

int	main(int ac, char **argv)
{
    if (ac != 3)
    {
        std::cerr << "invalid args numbers" << std::endl;
	    return (0);
    }
    try
    {
        Bot  Kennel("Kennel" ,(std::string(argv[1])), (std::string(argv[2])));

        Kennel.veil();
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}
