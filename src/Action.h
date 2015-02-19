#pragma once
#ifndef ACTION_H
#define ACTION_H

#include <iostream> 

class Action
{

public:
	std::string from;
	std::string to;
	int doing;
	bool isLoop;
	bool isChasing;
	int aux;

	Action();
	~Action();
};
#endif
