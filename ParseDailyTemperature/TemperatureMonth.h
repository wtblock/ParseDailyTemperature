/////////////////////////////////////////////////////////////////////////////
// Copyright © 2023 by W. T. Block, all rights reserved
/////////////////////////////////////////////////////////////////////////////
#pragma once
#include "SmartArray.h"

// the layout of USHCN daily data in ASCII format
// Variable Columns Type
// COOP ID	  1-  6	Character
// YEAR		  7- 10	Integer
// MONTH	 11- 12	Integer
// ELEMENT	 13- 16	Character
// VALUE1	 17- 21	Integer
// MFLAG1	 22		Character
// QFLAG1	 23		Character
// SFLAG1	 24		Character
// VALUE2	 25- 29	Integer
// MFLAG2	 30		Character
// QFLAG2	 31		Character
// SFLAG2	 32		Character
// .		.		.
// .		.		.
// .		.		.
// VALUE31	257-261 Integer
// MFLAG31	262		Character
// QFLAG31	263		Character
// SFLAG31	264		Character

// a class to contain a month of temperature data
class CTemperatureMonth
{
	// class for a single temperature reading
	class CTemperature
	{
	// protected data
	protected:
		// value in degrees Fahrenheit 
		int m_nDegrees;

		// measurement flag
		CString m_csMFlag;

		// quality flag
		CString m_csQFlag;

		// source flag
		CString m_csSFlag;

	// public properties
	public:
		// value in degrees Fahrenheit 
		inline int GetDegrees()
		{
			return m_nDegrees;
		}
		// value in degrees Fahrenheit 
		inline void SetDegrees( int value )
		{
			m_nDegrees = value;
		}
		// value in degrees Fahrenheit 
		__declspec( property( get = GetDegrees, put = SetDegrees ) )
			int Degrees;

		// measurement flag
		inline CString GetMFlag()
		{
			return m_csMFlag;
		}
		// measurement flag
		inline void SetMFlag( CString value )
		{
			m_csMFlag = value;
		}
		// measurement flag
		__declspec( property( get = GetMFlag, put = SetMFlag ) )
			CString MFlag;

		// quality flag
		inline CString GetQFlag()
		{
			return m_csQFlag;
		}
		// quality flag
		inline void SetQFlag( CString value )
		{
			m_csQFlag = value;
		}
		// quality flag
		__declspec( property( get = GetQFlag, put = SetQFlag ) )
			CString QFlag;

		// source flag
		inline CString GetSFlag()
		{
			return m_csSFlag;
		}
		// source flag
		inline void SetSFlag( CString value )
		{
			m_csSFlag = value;
		}
		// source flag
		__declspec( property( get = GetSFlag, put = SetSFlag ) )
			CString SFlag;

	// protected methods
	protected:

	// public methods
	public:

	// protected virtuals
	protected:

	// public virtuals
	public:

	// public constructor/destructor
	public:
		CTemperature()
		{
			Degrees = -9999;
		}
		~CTemperature()
		{
			Degrees = -9999;
		}

	};

// protected data
protected:
	// single line of temperature text file
	CString m_csSource;

	// station ID
	CString m_csStateID;

	// four character year
	CString m_csYear;

	// two character month
	CString m_csMonth;

	// element name (TMAX or TMIN)
	CString m_csElement;

	// key to the temperature data
	CString m_csKey;

	// a collection of temperatures for the month
	CSmartArray<CTemperature> m_Temperatures;

// public properties
public:
	// single line of temperature text file
	inline CString GetSource()
	{
		// single line of source from the stations text file
		const CString value = m_csSource;

		return value;
	}
	// single line of temperature text file
	inline void SetSource( CString value )
	{
		// persist the value
		m_csSource = value;

		// parse all of the properties
		const CString csStateID = StateID;
		const CString csYear = Year;
		const CString csMonth = Month;
		const CString csElement = Element;
		CSmartArray<CTemperature>& temps = Temperatures;
	}
	// single line of temperature text file
	__declspec( property( get = GetSource, put = SetSource ) )
		CString Source;

	// station ID
	inline CString GetStateID()
	{
		CString value = m_csStateID;

		if ( ParseValue( value, 0, 6 ) )
		{
			StateID = value;
		}

		return value;
	}
	// station ID
	inline void SetStateID( CString value )
	{
		m_csStateID = value;
	}
	// station ID
	__declspec( property( get = GetStateID, put = SetStateID ) )
		CString StateID;

	// four character year
	inline CString GetYear()
	{
		CString value = m_csYear;

		if ( ParseValue( value, 6, 4 ) )
		{
			Year = value;
		}

		return value;
	}
	// four character year
	inline void SetYear( CString value )
	{
		m_csYear = value;
	}
	// four character year
	__declspec( property( get = GetYear, put = SetYear ) )
		CString Year;

	// two character month
	inline CString GetMonth()
	{
		CString value = m_csMonth;

		if ( ParseValue( value, 10, 2 ) )
		{
			Month = value;
		}

		return value;
	}
	// two character month
	inline void SetMonth( CString value )
	{
		m_csMonth = value;
	}
	// two character month
	__declspec( property( get = GetMonth, put = SetMonth ) )
		CString Month;

	// element name (TMAX or TMIN)
	inline CString GetElement()
	{
		CString value = m_csElement;

		if ( ParseValue( value, 12, 4 ) )
		{
			Element = value;
		}

		return value;
	}
	// element name (TMAX or TMIN)
	inline void SetElement( CString value )
	{
		m_csElement = value;
	}
	// element name (TMAX or TMIN)
	__declspec( property( get = GetElement, put = SetElement ) )
		CString Element;

	// unique key to a temperature record
	inline CString GetKey()
	{
		CString value = m_csKey;

		if ( ParseValue( value, 0, 16 ) )
		{
			Key = value;
		}

		return value;
	}
	// unique key to a temperature record
	inline void SetKey( CString value )
	{
		m_csKey = value;
	}
	// unique key to a temperature record
	__declspec( property( get = GetKey, put = SetKey ) )
		CString Key;

	// a collection of temperatures for a month
	CSmartArray<CTemperature>& GetTemperatures()
	{
		int nStart = 16;
		if ( m_Temperatures.Empty && !m_csSource.IsEmpty())
		{
			for ( int nIndex = 0; nIndex < 31; nIndex++ )
			{
				shared_ptr<CTemperature> pTemp = 
					shared_ptr<CTemperature>
					(
						new CTemperature
					);
				CString csDegrees, csMFlag, csQFlag, csSFlag;
				ParseValue( csDegrees, nStart, 5 );
				nStart += 5;
				ParseValue( csMFlag, nStart++, 1 );
				ParseValue( csQFlag, nStart++, 1 );
				ParseValue( csSFlag, nStart++, 1 );

				pTemp->Degrees = atoi( csDegrees );
				pTemp->MFlag = csMFlag;
				pTemp->QFlag = csQFlag;
				pTemp->SFlag = csSFlag;

				m_Temperatures.append( pTemp );
			}
		}
		return m_Temperatures;
	}
	// a collection of temperatures for a month
	__declspec( property( get = GetTemperatures ) )
		CSmartArray<CTemperature>& Temperatures;

// protected methods
protected:
	// parse a value from source
	inline bool ParseValue( CString& input, int nStart, int nCount )
	{
		bool value = input.IsEmpty();
		if ( value )
		{
			if ( !m_csSource.IsEmpty() )
			{
				input = m_csSource.Mid( nStart, nCount );
			}
		}

		return value;
	}

// public methods
public:

// protected virtuals
protected:

// public virtuals
public:

// public constructor/destructor
public:
	// construct from a line of temperature data file
	CTemperatureMonth( LPCTSTR source )
	{
		Source = source;
	}
	
	// destructor
	virtual ~CTemperatureMonth()
	{

	}
};

