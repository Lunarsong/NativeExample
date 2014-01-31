/*
 * ExampleStateListener.cpp
 *
 *  Created on: Jan 31, 2014
 *      Author: Shanee
 */

#include "ExampleStateListener.h"
#include <AndroidLog.h>

ExampleStateListener::ExampleStateListener()
{

}

ExampleStateListener::~ExampleStateListener()
{

}

void ExampleStateListener::OnStateLoaded( int iStatusCode, int iStateKey, const Android::AppStateData* pData )
{
	LOGV( "[ExampleStateListener::OnStateLoaded] Code: %i, Key: %i, Data: %s.", iStatusCode, iStateKey, pData->Data() );
}

void ExampleStateListener::OnStateConflict( int iStateKey, const Android::AppStateData* pLocaData,
								const Android::AppStateData* pServerData, Android::AppStateData* pResolvedData )
{

}
