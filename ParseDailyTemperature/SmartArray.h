/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2022 by W. T. Block, All Rights Reserved
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include <vector>
#include <memory>

using namespace std;

// template class to manage arrays of pointers to any class
template<class TYPE> class CSmartArray
{
// protected data
protected:
	// array of items
	vector< shared_ptr<TYPE> > m_arrItems;
	
// properties
public:
	// array of items
	inline vector<shared_ptr<TYPE> >& GetItems()
	{ 
		return m_arrItems; 
	}
	// array of items
	__declspec( property( get=GetItems )) 
		vector<shared_ptr<TYPE> > Items;

	// number of Items
	inline long GetCount()
	{ 
		return (long)m_arrItems.size(); 
	}
	// number of Items
	__declspec( property( get=GetCount ))
		long Count;


// methods
public:
	// valid index?
	inline bool valid( long lIndex )
	{
		const long nItems = Count;
		return ( 0 <= lIndex && lIndex < nItems );
	}

	// clear all Items from array
	inline void clear()
	{
		// empty the array
		m_arrItems.clear();
	}
	
	// resize Item array
	inline void resize( long nSize )
	{
		m_arrItems.resize( nSize );
	}

	// add a new Item and return its index
	inline long add()
	{
		{
			shared_ptr<TYPE> pItem = shared_ptr<TYPE>( new TYPE );
			m_arrItems.push_back( pItem );
		}

		return Count - 1;
	}
	
	// remove an item
	inline bool remove( long lIndex )
	{
		bool bOK = false;
		if ( valid( lIndex ))
		{
			// remove the item's position from the array
			vector<shared_ptr<TYPE>>::iterator pos = m_arrItems.begin();
			pos += lIndex;
			m_arrItems.erase( pos ); 
			bOK = true;	
		}

		return bOK;
	}
	
	// Item at an index, NULL if out of range
	// if the optional release is true, the method copying the pointer
	// is responsible for deleting it and the array's pointer is set to zero.
	inline shared_ptr<TYPE> get( long lIndex )
	{
		shared_ptr<TYPE> pItem;
		if ( valid( lIndex ))
		{
			pItem = m_arrItems[ lIndex ];
		}

		return pItem;
	}

	// Item pointer at an index--will extend the size if necessary,
	// delete the contents of the existing pointer, and copy the
	// given pointer to the location
	inline void set( long lIndex, shared_ptr<TYPE> pItem )
	{
		// resize if necessary
		if ( lIndex >= Count )
		{
			resize( lIndex + 1 );
		}
		
		// put the item in the array
		if ( valid( lIndex ))
		{
			m_arrItems[ lIndex ] = pItem;
		}
	}

	// Item at an index--will extend the size if necessary,
	// and replace the contents of the existing element
	inline void append( shared_ptr<TYPE> pItem )
	{
		m_arrItems.push_back( pItem );
	}

// construction / destruction
public:
	CSmartArray()
	{
	}

	virtual ~CSmartArray()
	{
		clear();
	}

};
