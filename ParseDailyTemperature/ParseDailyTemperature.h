/////////////////////////////////////////////////////////////////////////////
// Copyright © 2023 by W. T. Block, all rights reserved
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include "ClimateStation.h"
#include "State.h"
#include "CHelper.h"
#include "KeyedCollection.h"
#include "resource.h"

// collection of stations keyed by station ID
CKeyedCollection<CString, CClimateStation> m_Stations;

// collection of states by postal name
CKeyedCollection<CString, CState> m_States;

// cross reference of state codes to postal names
CKeyedCollection<CString, CString> m_StateCodes;

// path to the stations file
CString m_csStationPath;

// path to the data file
CString m_csPath;


