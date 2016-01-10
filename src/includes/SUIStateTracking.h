/*
 * SUIStateTracking.h
 *
 *  Created on: Aug 10, 2014
 *      Author: malcalypse
 */

#ifndef SUISTATETRACKING_H_
#define SUISTATETRACKING_H_

#include "SUIConfig.h"

#ifdef BUILDING_DRUID

#ifndef SUI_FLASHSTRING_PTR
#define SUI_FLASHSTRING_PTR	char *
#endif
#else

#include "SUIPlatform.h"
#endif


#ifdef SUI_ENABLE_STATE_TRACKER
namespace SUI {
typedef enum TrackedTypeEnum {
	SUITracked_Bool='b',
	SUITracked_UInt='i',
	SUITracked_Float='f'
} TrackedType;

typedef struct TrackedStateVariableDetailsStruct {

	TrackedType type;
	SUI_FLASHSTRING_PTR name;
	union {
		float * ptr_float;
		unsigned long *	ptr_int;
		bool *	ptr_bool;
	};

	union {
		float lastval_float;
		unsigned long 	lastval_int;
		bool	lastval_bool;
	};

	TrackedStateVariableDetailsStruct(SUI_FLASHSTRING_PTR name, TrackedType type, void * variable );

	bool hasChanged();


} TrackedStateVariableDetails;

} /* namespace SUI */

#endif




#endif /* SUISTATETRACKING_H_ */
