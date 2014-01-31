/*
 * ExampleStateListener.h
 *
 *  Created on: Jan 31, 2014
 *      Author: Shanee
 */

#ifndef EXAMPLESTATELISTENER_H_
#define EXAMPLESTATELISTENER_H_

#include <AppState.h>

class ExampleStateListener : public Android::IAppStateListener
{
public:
	ExampleStateListener();
	virtual ~ExampleStateListener();

	virtual void OnStateLoaded( int iStatusCode, int iStateKey, const Android::AppStateData* pData );
	virtual void OnStateConflict( int iStateKey, const Android::AppStateData* pLocaData,
									const Android::AppStateData* pServerData, Android::AppStateData* pResolvedData );
};

#endif /* EXAMPLESTATELISTENER_H_ */
