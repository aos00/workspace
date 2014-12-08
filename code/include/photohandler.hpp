#ifndef __PHOTOHANDLER_HPP_INCLUDED__
#define __PHOTOHANDLER_HPP_INCLUDED__

#include "photo.hpp"


class PhotoHandler
{
	public:
        void stampCoordinates(vector<Photo>&);
		void stampCoordinates(const Photo&);

};

#endif
