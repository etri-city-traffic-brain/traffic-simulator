/*
 * Event.h
 *
 *  Created on: 2018. 1. 26.
 *      Author: SDJ
 */

#ifndef OBJECTT_EVENT_H_
#define OBJECTT_EVENT_H_

#include <utils/config.h>


namespace SALT{
class EventListener;

class Event {
public:
	Event();
	virtual ~Event();
//	void addListener(EventListener * listener);
//	std::vector<EventListener*> listners_;
private:
//	SALTTime myStartTime;
//	SALTTime myEndTime;
//	EventType myClass;
};
}
#endif /* OBJECTT_EVENT_H_ */
