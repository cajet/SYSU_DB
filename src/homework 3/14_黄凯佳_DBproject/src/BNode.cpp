#include "BNode.h"
b_node::b_node() {
	level = -1;
	left_brother = right_brother = -1;
	size = 0;
}
b_node::b_node(int left, int _level) {
	left_brother = (char)left;
	right_brother = -1;
	level = (char)_level;
	size = 0;
}
