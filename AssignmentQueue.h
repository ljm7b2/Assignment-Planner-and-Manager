#pragma once

#include "Assignment.h"
#include <queue>


// Assignment Queue
// This class encapsulates a queue of Assignment objects
// Used to pass groups of Assignment objects between the AssignmentManager and UserInterace classes

class AssignmentQueue
{
public:
	AssignmentQueue();
	Assignment Pop();
	const bool IsEmpty() const;
	const int Size() const;
	void Push(Assignment assignment);
private:
	queue<Assignment> _assignments;
};

AssignmentQueue::AssignmentQueue()
{ ; }

Assignment AssignmentQueue::Pop()
{
	Assignment nextAssignment = _assignments.front();
	_assignments.pop();
	return nextAssignment;
}

const bool AssignmentQueue::IsEmpty() const
{
	return _assignments.empty();
}

const int AssignmentQueue::Size() const
{
	return _assignments.size();
}

void AssignmentQueue::Push(Assignment assignment)
{
	_assignments.push(assignment);
}
