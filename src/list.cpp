#include "list.h"

List::List()
{
	head = new Item;
	head = nullptr;
}

List::~List()
{
	delete head;
}

List::Item::Item (void (*f)(), uint16_t p)
{
	period_ms = p;
	func = f;
	next = new Item;
}

List::Item::Item ()
{
	next = new Item;
}

List::Item::~Item ()
{
	delete next;
}

void List::addItem (void (*f)(), uint16_t p_)
{
	Item * i = new Item (f, p_);
	i->next = head;
	head = i;
}

void List::itarate ()
{
	Item * current = new Item;
	current = head;
	while (current != nullptr)
	{
		//some actions
		current = current->next;
	}
}
