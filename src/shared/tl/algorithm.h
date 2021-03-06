/*
 * Copyright (C) 2016 necropotame (necropotame@gmail.com)
 * 
 * This file is part of TeeUniverse.
 * 
 * TeeUniverse is free software: you can redistribute it and/or  modify
 * it under the terms of the GNU Affero General Public License, version 3,
 * as published by the Free Software Foundation.
 *
 * TeeUniverse is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with TeeUniverse.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * Some parts of this file comes from other projects.
 * These parts are itendified in this file by the following block:
 * 
 * FOREIGN CODE BEGIN: ProjectName *************************************
 * 
 * FOREIGN CODE END: ProjectName ***************************************
 * 
 * If ProjectName is "TeeWorlds", then this part of the code follows the
 * TeeWorlds licence:
 *      (c) Magnus Auvinen. See LICENSE_TEEWORLDS in the root of the
 *      distribution for more information. If you are missing that file,
 *      acquire a complete release at teeworlds.com.
 */

#ifndef __SHARED_TL_ALGORITHM__
#define __SHARED_TL_ALGORITHM__

/* FOREIGN CODE BEGIN: TeeWorlds **************************************/

#include "range.h"


/*
	insert 4
		v
	1 2 3 4 5 6

*/


template<class R, class T>
R partition_linear(R range, T value)
{
	concept_empty::check(range);
	concept_forwarditeration::check(range);
	concept_sorted::check(range);

	for(; !range.empty(); range.pop_front())
	{
		if(!(range.front() < value))
			return range;
	}
	return range;
}


template<class R, class T>
R partition_binary(R range, const T& value)
{
	concept_empty::check(range);
	concept_index::check(range);
	concept_size::check(range);
	concept_slice::check(range);
	concept_sorted::check(range);

	if(range.empty())
		return range;
	if(range.back() < value)
		return R();

	while(range.size() > 1)
	{
		unsigned pivot = (range.size()-1)/2;
		if(range.index(pivot) < value)
			range = range.slice(pivot+1, range.size()-1);
		else
			range = range.slice(0, pivot+1);
	}
	return range;
}

template<class R, class T>
R find_linear(R range, T value)
{
	concept_empty::check(range);
	concept_forwarditeration::check(range);
	for(; !range.empty(); range.pop_front())
		if(value == range.front())
			break;
	return range;
}

template<class R, class T>
R find_binary(R range, T value)
{
	range = partition_linear(range, value);
	if(range.empty()) return range;
	if(range.front() == value) return range;
	return R();
}


template<class R>
void sort_bubble(R range)
{
	concept_empty::check(range);
	concept_forwarditeration::check(range);
	concept_backwarditeration::check(range);

	// slow bubblesort :/
	for(; !range.empty(); range.pop_back())
	{
		R section = range;
		typename R::type *prev = &section.front();
		section.pop_front();
		for(; !section.empty(); section.pop_front())
		{
			typename R::type *cur = &section.front();
			if(*cur < *prev)
				swap(*cur, *prev);
			prev = cur;
		}
	}
}

/*
template<class R>
void sort_quick(R range)
{
	concept_index::check(range);
}*/


template<class R>
void sort(R range)
{
	sort_bubble(range);
}


template<class R>
bool sort_verify(R range)
{
	concept_empty::check(range);
	concept_forwarditeration::check(range);

	typename R::type *prev = &range.front();
	range.pop_front();
	for(; !range.empty(); range.pop_front())
	{
		typename R::type *cur = &range.front();

		if(*cur < *prev)
			return false;
		prev = cur;
	}

	return true;
}

/* FOREIGN CODE END: TeeWorlds ****************************************/

#endif
