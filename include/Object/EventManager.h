/*
 * EventManager.h
 *
 *  Created on: 2018. 1. 30.
 *      Author: SDJ
 */

#ifndef EVENTMANAGER_H_
#define EVENTMANAGER_H_

#include <Object/Event/Event.h>
#include <list>
namespace SALT{
class EventManager {
public:
	EventManager();
	virtual ~EventManager();
private:
	std::list<Event*> myEventList;
};
}
#endif /* EVENTMANAGER_H_ */
