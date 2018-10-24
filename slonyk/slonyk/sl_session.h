#pragma once
#include "slonyk_segment.h"

class SlSession
{
public :
	virtual void onSegmentReceived(SlSegment & recSegment) = 0;
	virtual ~SlSession(){};
};
