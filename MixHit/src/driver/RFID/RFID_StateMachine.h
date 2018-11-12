#ifndef _RFID_STATEMACHINE_H_
#define _RFID_STATEMACHINE_H_

enum eRFIDStateMachine
{
	RFID_Idle,				// Idle state, waiting for new card
	RFID_RotateTable,		// Rotate table
	RFID_Filling,			// Prepare mixing cocktail
	RFID_FullGlassInStation	// Just mixed a cocktail, determine next action
};

#endif
