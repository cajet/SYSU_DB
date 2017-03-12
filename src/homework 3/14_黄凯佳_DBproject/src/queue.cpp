#include "queue.h"

queue::queue() {
 	front = rear_now = rear_next = 0;
 }

void queue::pop() {
	front = (front + 1) % length;
}

void queue::reset() {
	rear_now = rear_next;
}

void queue::clear() {
 	front = rear_now = rear_next = 0;
}

bool queue::full(bool isLeaf) {
	if (isLeaf) {
		return (rear_now+1)%length == front;	
	} else {
		return (rear_next + 1)%length == front;
	}
}

bool queue::empty() {
	return rear_now == front;
}

bool queue::push_back(int element_id, double & Euc_distance, bool isLeaf) {
	if (full(true)||full(false)) return false;
	else {
		if (isLeaf) {
			Que[rear_now].element_id = element_id;
			Que[rear_now].Euc_distance = Euc_distance;
			rear_now = (rear_now + 1) % length;
			rear_next = rear_now;
		} else {
			Que[rear_next].element_id = element_id;
			Que[rear_next].Euc_distance = Euc_distance;
			rear_next = (rear_next + 1) % length;
		}
		return true;
	}
}

bool queue::Top(int & element_id, double & Euc_distance) {
	if (empty()) {
		return false;
	} else {
		element_id = Que[front].element_id;
		Euc_distance = Que[front].Euc_distance;
		pop();
		return true;
	}
}
