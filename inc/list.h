#include "MK02F12810.h"

#ifndef LIST_H_
#define LIST_H_

class List {
public:
	class Item
	{
		void (*func)();
		uint16_t period_ms;
		Item * next;
		Item (void (*f)(), uint16_t p);
		Item ();
		~Item();
		friend class List;
	};
private:
	Item * head;
public:
	List();
	~List();
	void addItem (void (*f)(), uint16_t p_);
	void itarate ();
	//void addToHead ()

};



#endif /* LIST_H_ */
