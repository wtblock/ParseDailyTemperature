// ParseDailyTemperature.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ParseDailyTemperature.h"
#include "CHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object
CWinApp theApp;

using namespace std;

/////////////////////////////////////////////////////////////////////////////
// write the given station temperatures at the given path
bool WriteStation
(
	CString csPath, CString csStation,
	CSmartArray<CTemperatureMonth>& Months
)
{
	const CString csData = csPath + _T( "_" ) + csStation + _T( ".CSV" );

	CStdioFile fileWrite;
	const bool value =
		FALSE != fileWrite.Open
		(
			csData, CFile::modeWrite | CFile::modeCreate | CFile::shareDenyNone
		);

	bool bFirst = true;
	if ( value == true )
	{
		for ( auto& node : Months.Items )
		{
			// the first line of the CSV file is the header
			if ( bFirst )
			{
				const CString csHeader = node->Header;

				fileWrite.WriteString( csHeader );
				bFirst = false;
			}

			// generate a line of CSV
			const CString csOut = node->Output;

			// write the line to the output file
			fileWrite.WriteString( csOut );
		}

		fileWrite.Close();
	}

	return value;
} // WriteStation

/////////////////////////////////////////////////////////////////////////////
// read the stations text file and index it by the station 6 digit station 
// code
bool ReadTemperatures( CStdioFile& fErr, CString csPath )
{
	const CString csFolder = CHelper::GetFolder( csPath );
	const CString csFile = CHelper::GetFileName( csPath );
	const CString csExt = CHelper::GetExtension( csPath );

	bool value = false;

	// open the temperature text file
	CStdioFile fileRead;
	const bool bRead =
		FALSE != fileRead.Open
		(
			csPath, CFile::modeRead | CFile::shareDenyNone
		);

	// if the open was successful, read each line of the file and 
	// collect the temperature data properties
	if ( bRead == true )
	{
		// collection of station temperatures
		CSmartArray<CTemperatureMonth> Months;
		CString csStation;
		CString csLine;
		bool bFirst = true;
		while ( fileRead.ReadString( csLine ) )
		{
			// create a monthly temperature
			shared_ptr<CTemperatureMonth> pTemp =
				shared_ptr<CTemperatureMonth>
				(
					new CTemperatureMonth( csLine )
				);
			if ( csStation.IsEmpty() )
			{
				csStation = pTemp->Station;
			}
			else if ( csStation != pTemp->Station )
			{
				CString csState = _T( "Unknown" );
				shared_ptr<CString> pState = m_StateCodes.find( csStation.Left( 2 ) );
				if ( pState != NULL )
				{
					csState = *pState;
				}
				CString csLocation = _T( "Unknown" );
				shared_ptr<CClimateStation> pStation = m_Stations.find( csStation );
				if ( pStation != NULL )
				{
					csLocation = pStation->Location;
				}
				CString csMessage;
				csMessage.Format
				( 
					_T( "Element: %s, State: %s, Location: %s\n" ), 
					csFile, csState, csLocation
				);
				fErr.WriteString( csMessage );

				WriteStation( csFolder + csFile, csStation, Months );
				Months.clear();
				csStation = pTemp->Station;
			}
			Months.append( pTemp );
		}
		if ( !Months.Empty )
		{
			WriteStation( csFolder + csFile, csStation, Months );
			Months.clear();
		}
	}
	return value;
} // ReadTemperatures

/////////////////////////////////////////////////////////////////////////////
// read the stations text file and index it by the station 6 digit station 
// code
bool ReadStations()
{
	// open the stations text file
	CStdioFile file;
	const bool value =
		FALSE != file.Open
		(
			m_csStationPath, CFile::modeRead | CFile::shareDenyNone
		);

	// if the open was successful, read each line of the file and 
	// collect the station data properties
	if ( value == true )
	{
		CString csLine;
		while ( file.ReadString( csLine ) )
		{
			shared_ptr<CClimateStation> pStation =
				shared_ptr<CClimateStation>( new CClimateStation( csLine ) );
			const CString csKey = pStation->Station;
			m_Stations.add( csKey, pStation );
		}
	}

	return value;
} // ReadStations

/////////////////////////////////////////////////////////////////////////////
// read the state data and make a cross reference between the two digit code
// and the two letter postal code that is used to identify states
bool ReadStates()
{
	bool value = false;
	const CString states[] = 
	{
		_T( "01,AL,Alabama" ),
		_T( "02,AZ,Arizona" ),
		_T( "03,AR,Arkansas" ),
		_T( "04,CA,California" ),
		_T( "05,CO,Colorado" ),
		_T( "06,CT,Connecticut" ),
		_T( "07,DE,Delaware" ),
		_T( "08,FL,Florida" ),
		_T( "09,GA,Georgia" ),
		_T( "10,ID,Idaho" ),
		_T( "11,IL,Illinois" ),
		_T( "12,IN,Indiana" ),
		_T( "13,IA,Iowa" ),
		_T( "14,KS,Kansas" ),
		_T( "15,KY,Kentucky" ),
		_T( "16,LA,Louisiana" ),
		_T( "17,ME,Maine" ),
		_T( "18,MD,Maryland" ),
		_T( "19,MA,Massachusetts" ),
		_T( "20,MI,Michigan" ),
		_T( "21,MN,Minnesota" ),
		_T( "22,MS,Mississippi" ),
		_T( "23,MO,Missouri" ),
		_T( "24,MT,Montana" ),
		_T( "25,NE,Nebraska" ),
		_T( "26,NV,Nevada" ),
		_T( "27,NH,New Hampshire" ),
		_T( "28,NJ,New Jersey" ),
		_T( "29,NM,New Mexico" ),
		_T( "30,NY,New York" ),
		_T( "31,NC,North Carolina" ),
		_T( "32,ND,North Dakota" ),
		_T( "33,OH,Ohio" ),
		_T( "34,OK,Oklahoma" ),
		_T( "35,OR,Oregon" ),
		_T( "36,PA,Pennsylvania" ),
		_T( "37,RI,Rhode Island" ),
		_T( "38,SC,South Carolina" ),
		_T( "39,SD,South Dakota" ),
		_T( "40,TN,Tennessee" ),
		_T( "41,TX,Texas" ),
		_T( "42,UT,Utah" ),
		_T( "43,VT,Vermont" ),
		_T( "44,VA,Virginia" ),
		_T( "45,WA,Washington" ),
		_T( "46,WV,West Virginia" ),
		_T( "47,WI,Wisconsin" ),
		_T( "48,WY,Wyoming" )
	};

	for ( auto& node : states )
	{
		int nStart = 0;
		CString csToken;
		const CString csDelim = _T( "," );
		do
		{
			const CString csCode = node.Tokenize( csDelim, nStart );
			if ( csCode.IsEmpty() )
			{
				break;
			}
			const CString csPostal = node.Tokenize( csDelim, nStart );
			const CString csName = node.Tokenize( csDelim, nStart );
			shared_ptr<CState> pState( new CState );
			pState->Code = csCode;
			pState->Postal = csPostal;
			pState->Name = csName;

			m_States.add( csPostal, pState );
			shared_ptr<CString> pPostal( new CString( csPostal ));
			m_StateCodes.add( csCode, pPostal );

		} while ( true );

		value = true;
	}

	return value;
} // ReadStates

/////////////////////////////////////////////////////////////////////////////
// crawl through the directory tree looking for supported image extensions
void RecursePath( CStdioFile& fErr, LPCTSTR path )
{
	USES_CONVERSION;

	// start trolling for files we are interested in
	CFileFind finder;
	BOOL bWorking = finder.FindFile( path );
	while ( bWorking )
	{
		bWorking = finder.FindNextFile();

		// skip "." and ".." folder names
		if ( finder.IsDots() )
		{
			continue;
		}

		// if it's a directory, recursively search it
		if ( finder.IsDirectory() )
		{
			const CString str = finder.GetFilePath() + _T( "\\*.*" );

			RecursePath( fErr, str );
		}
		else // process if it is a valid filename
		{
			const CString csPath = finder.GetFilePath().MakeUpper();
			const CString csData = CHelper::GetDataName( csPath );
			if ( csData == _T( "TMAX.TXT" ) || csData == _T( "TMIN.TXT" ) )
			{
				ReadTemperatures( fErr, csPath );
			}
		}
	}

	finder.Close();

} // RecursePath

/////////////////////////////////////////////////////////////////////////////
// a console application that can crawl through the file
// system and troll for climate data
int _tmain( int argc, TCHAR* argv[], TCHAR* envp[] )
{
	HMODULE hModule = ::GetModuleHandle( NULL );
	if ( hModule == NULL )
	{
		_tprintf( _T( "Fatal Error: GetModuleHandle failed\n" ) );
		return 1;
	}

	// initialize MFC and error on failure
	if ( !AfxWinInit( hModule, NULL, ::GetCommandLine(), 0 ) )
	{
		_tprintf( _T( "Fatal Error: MFC initialization failed\n " ) );
		return 2;
	}

	// do some common command line argument corrections
	vector<CString> arrArgs = CHelper::CorrectedCommandLine( argc, argv );
	size_t nArgs = arrArgs.size();

	CStdioFile fErr( stderr );
	CString csMessage;

	// display the number of arguments if not 1 to help the user 
	// understand what went wrong if there is an error in the
	// command line syntax
	if ( nArgs != 1 )
	{
		fErr.WriteString( _T( ".\n" ) );
		csMessage.Format
		(
			_T( "The number of parameters are %d\n.\n" ), nArgs - 1
		);
		fErr.WriteString( csMessage );

		// display the arguments
		for ( int i = 1; i < nArgs; i++ )
		{
			csMessage.Format
			(
				_T( "Parameter %d is %s\n.\n" ), i, arrArgs[ i ]
			);
			fErr.WriteString( csMessage );
		}
	}

	// two arguments if a pathname to the climate data is given
	// three arguments if the station text file name is also given
	if ( nArgs != 2 && nArgs != 3 )
	{
		fErr.WriteString( _T( ".\n" ) );
		fErr.WriteString
		(
			_T( "ParseDailyTemperature, Copyright (c) 2023, " )
			_T( "by W. T. Block.\n" )
		);

		fErr.WriteString
		(
			_T( ".\n" )
			_T( "A Windows command line program to read daily climate \n" )
			_T( "  history and output folders of state data.\n" )
			_T( ".\n" )
		);

		fErr.WriteString
		(
			_T( ".\n" )
			_T( "Usage:\n" )
			_T( ".\n" )
			_T( ".  ParseDailyTemperature pathname [station_file_name]\n" )
			_T( ".\n" )
			_T( "Where:\n" )
			_T( ".\n" )
		);

		fErr.WriteString
		(
			_T( ".  pathname is the folder to be scanned that contains\n" )
			_T( ".    all of the state folders that contain TMAX.TXT \n" )
			_T( ".    and/or TMIN.TXT. \n" )
			_T( ".  station_file_name is the optional station filename: \n" )
			_T( ".    defaults to: \"stations.txt\"\n" )
			_T( ".\n" )
		);

		return 3;
	}

	// display the executable path
	csMessage.Format( _T( "Executable pathname: %s\n" ), arrArgs[ 0 ] );
	fErr.WriteString( _T( ".\n" ) );
	fErr.WriteString( csMessage );
	fErr.WriteString( _T( ".\n" ) );

	// retrieve the pathname from the command line
	m_csPath = arrArgs[ 1 ];

	// trim off any wild card data
	const CString csFolder = CHelper::GetFolder( m_csPath );

	// test for current folder character (a period)
	bool bExists = m_csPath == _T( "." );

	// if it is a period, add a wild card of *.* to retrieve
	// all folders and files
	if ( !bExists )
	{
		if ( ::PathFileExists( csFolder ) )
		{
			bExists = true;
		}
	}

	if ( !bExists )
	{
		csMessage.Format( _T( "Invalid pathname:\n\t%s\n" ), m_csPath );
		fErr.WriteString( _T( ".\n" ) );
		fErr.WriteString( csMessage );
		fErr.WriteString( _T( ".\n" ) );
		return 4;

	}
	else
	{
		csMessage.Format( _T( "Given pathname:\n\t%s\n" ), m_csPath );
		fErr.WriteString( _T( ".\n" ) );
		fErr.WriteString( csMessage );
	}

	// read the station data
	m_csStationPath = _T( "stations.txt" );
	if ( nArgs == 3 )
	{
		m_csStationPath = arrArgs[ 2 ];
	}

	if ( !::PathFileExists( m_csStationPath ) )
	{
		csMessage.Format
		(
			_T( "Invalid Station path name:\n\t%s\n" ), m_csStationPath
		);
		fErr.WriteString( _T( ".\n" ) );
		fErr.WriteString( csMessage );
		fErr.WriteString( _T( ".\n" ) );
		return 5;

	}

	// start up COM
	AfxOleInit();
	::CoInitialize( NULL );

	// the pathname of the executable
	const CString csExe = arrArgs[ 0 ];

	// create a context block so the project goes 
	// out of context before the program ends to prevent
	// memory leaks
	{
		ReadStates();
		ReadStations();
		const CString csRight = m_csPath.Right( 1 );
		if ( csRight != _T( "\\" ) )
		{
			m_csPath += _T( "\\" );
		}
		m_csPath += _T( "*.*" );

		RecursePath( fErr, m_csPath );
	}

	// all is good
	return 0;

} // _tmain

  /////////////////////////////////////////////////////////////////////////////
