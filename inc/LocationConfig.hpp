/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConfig.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oruban <oruban@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 19:40:12 by oruban            #+#    #+#             */
/*   Updated: 2025/01/28 19:41:22 by oruban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* 
by roi
 */
#ifndef LOCATIONCONFIG_HPP
# define LOCATIONCONFIG_HPP

#include "Webserv.hpp"

class LocationConfig
{
    private:
        std::string                     _path;
        std::string                     _root;			//корневая директория
        std::string                     _index;			// начальная страница
        bool                            _autoindex;
        std::vector<std::string>        _methods;		//все допустимые методы
        std::string                     _return;
        std::vector<std::string>        _cgi_path;
        std::vector<std::string>        _cgi_ext;
};
#endif