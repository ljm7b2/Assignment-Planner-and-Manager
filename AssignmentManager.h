#pragma once

#include "Assignment.h"
#include "AssignmentQueue.h"
#include <iostream>

// AssignmentManager
// This class manages a collection of Assignment objects

class AssignmentManager
{
public:
	// Constructor
	AssignmentManager();

	// Public Getters
	Assignment GetAssignment(Date assignedDate);
	AssignmentQueue GetAllAssignments();
	AssignmentQueue Save();
	bool AssignmentExists(Date assignedDate);
	bool AssignmentExists(Assignment assignment);
	const bool IsDirty() const;
	const int NumberOfClosedAssignments();
	const int NumberOfLateAssignments();
	const int NumberOfOpenAssignments();
	const int TotalNumberOfAssignments();

	// Public Setters
	bool AddAssignment(Assignment assignment);
	bool AddAssignment(Date assignedDate, Date dueDate, AssignmentStatuses status, string description);
	bool CompleteAssignment(Date assignedDate, Date completedDate);
	bool EditAssignment(Date assignedDate, Date newDueDate);
	bool EditAssignment(Date assignedDate, string newDescription);
	void AddAssignments(AssignmentQueue assignmentQueue);

private:
	// Private Data
	bool _isDirty;
	int _numberOfLateAssignments;
	list<Assignment> _assignments;
	list<Assignment> _completedAssignments;
	list<Assignment>::iterator it;
	list<Assignment>::const_iterator constIt;

	// Private Functions
	bool addToClosedList(Assignment assignment);
	bool removeFromOpenList(Assignment assignment);
};
 
// Constructor

AssignmentManager::AssignmentManager()
{
	_numberOfLateAssignments = 0;
	_isDirty = false;
}

// Public Getters

Assignment AssignmentManager::GetAssignment(Date assignedDate)
{
	if (AssignmentExists(assignedDate))
	{
		for (constIt = _assignments.begin(); constIt != _assignments.end(); ++constIt)
		{
			if (constIt->AssignedDate() == assignedDate)
			{
				return *constIt;
			}
		}
	}
	throw::exception("ASSIGNMENT NOT FOUND");
}

AssignmentQueue AssignmentManager::GetAllAssignments()
{
	AssignmentQueue allAssignments;
	constIt = _assignments.begin();
	while (constIt != _assignments.end())
	{
		allAssignments.Push(*constIt);
		++constIt;
	}
	constIt = _completedAssignments.begin();
	while (constIt != _completedAssignments.end())
	{
		allAssignments.Push(*constIt);
		++constIt;
	}
	return allAssignments;
}

AssignmentQueue AssignmentManager::Save()
{
	_isDirty = false;
	return GetAllAssignments();
}

bool AssignmentManager::AssignmentExists(Assignment assignment)
{
	if (!_assignments.empty())
	{
		constIt = _assignments.begin();
		while (constIt != _assignments.end())
		{
			if (*constIt == assignment)
			{
				return true;
			}
			++constIt;
		}
	}
	return false;
}

bool AssignmentManager::AssignmentExists(Date assignedDate)
{
	if (!_assignments.empty())
	{
		constIt = _assignments.begin();
		while (constIt != _assignments.end())
		{
			if (constIt->AssignedDate() == assignedDate)
			{
				return true;
			}
			++constIt;
		}
	}
	return false;
}

const bool AssignmentManager::IsDirty() const
{
	return _isDirty;
}

const int AssignmentManager::NumberOfClosedAssignments()
{
	return _completedAssignments.size();
}

const int AssignmentManager::NumberOfLateAssignments()
{
	return _numberOfLateAssignments;
}

const int AssignmentManager::NumberOfOpenAssignments()
{
	return _assignments.size();
}

const int AssignmentManager::TotalNumberOfAssignments()
{
	return _assignments.size() + _completedAssignments.size();
}

// Public Setters

bool AssignmentManager::AddAssignment(Assignment assignment)
{
	return AddAssignment(assignment.AssignedDate(), assignment.DueDate(), assignment.Status(), assignment.Description());
}

bool AssignmentManager::AddAssignment(Date assignedDate, Date dueDate, AssignmentStatuses status, string description)
{

	if (AssignmentExists(assignedDate))
	{
		return false;
	}
	Assignment newAssignment(assignedDate, dueDate, status, description);
	if (newAssignment.Status() == AssignmentStatuses::Assigned)
	{
		if (_assignments.empty())
		{
			_assignments.push_back(newAssignment);
		}
		else
		{
			if (newAssignment.DueDate() <= _assignments.front().DueDate())
			{
				_assignments.push_front(newAssignment);
			}
			else if (newAssignment.DueDate() >= _assignments.back().DueDate())
			{
				_assignments.push_back(newAssignment);
			}
			else
			{
				constIt = _assignments.begin();
				while (newAssignment.DueDate() >= constIt->DueDate())
				{
					++constIt;
				}
				_assignments.insert(constIt, newAssignment);
			}
		}
	}
	else if (newAssignment.Status() == AssignmentStatuses::Late
		|| newAssignment.Status() == AssignmentStatuses::Completed)
	{
		if (_completedAssignments.empty())
		{
			_completedAssignments.push_back(newAssignment);
		}
		else
		{
			if (newAssignment.DueDate() <= _completedAssignments.front().DueDate())
			{
				_completedAssignments.push_front(newAssignment);
			}
			else if (newAssignment.DueDate() >= _completedAssignments.back().DueDate())
			{
				_completedAssignments.push_back(newAssignment);
			}
			else
			{
				constIt = _completedAssignments.begin();
				while (newAssignment.DueDate() >= constIt->DueDate())
				{
					++constIt;
				}
				_completedAssignments.insert(constIt, newAssignment);
			}
		}
	}
	if (newAssignment.Status() == AssignmentStatuses::Late)
	{
		++_numberOfLateAssignments;
	}
	return _isDirty = true;
}

void AssignmentManager::AddAssignments(AssignmentQueue assignmentQueue)
{
	while (!assignmentQueue.IsEmpty())
	{
		Assignment temp = assignmentQueue.Pop();
		AddAssignment(temp.AssignedDate(), temp.DueDate(), temp.Status(), temp.Description());
	}
}

bool AssignmentManager::CompleteAssignment(Date assignedDate, Date completedDate)
{
	Assignment assignmentToRemove = GetAssignment(assignedDate);
	Assignment assignmentToAdd = GetAssignment(assignedDate);
	assignmentToAdd.CompletedDate(completedDate);
	if (assignmentToAdd.Status() == AssignmentStatuses::Late)
	{
		++_numberOfLateAssignments;
	}
	if (!removeFromOpenList(assignmentToRemove))
	{
		return false;
	}
	if (!addToClosedList(assignmentToAdd))
	{
		return false;
	}
	return _isDirty = true;
}

bool AssignmentManager::EditAssignment(Date assignedDate, Date newDueDate)
{
	Assignment temp;
	it = _assignments.begin();
	if (AssignmentExists(assignedDate))
	{
		while (it->AssignedDate() != assignedDate)
		{
			++it;
		}
		it->DueDate(newDueDate);
		temp = *it;
		removeFromOpenList(*it);
		AddAssignment(temp);
		return _isDirty = true;
	}
	return false;
}

bool AssignmentManager::EditAssignment(Date assignedDate, string newDescription)
{
	if (AssignmentExists(assignedDate))
	{
		while (it->AssignedDate() != assignedDate)
		{
			++it;
		}
		it->Description(newDescription);
		return _isDirty = true;
	}
	return false;
}

// Private Functions

bool AssignmentManager::addToClosedList(Assignment assignment)
{
	if (assignment.Status() == AssignmentStatuses::Late
		|| assignment.Status() == AssignmentStatuses::Completed)
	{
		AddAssignment(assignment);
		return _isDirty = true;
	}
	return false;
}

bool AssignmentManager::removeFromOpenList(Assignment assignment)
{
	if (!_assignments.empty())
	{
		it = _assignments.begin();
		while (it != _assignments.end())
		{
			if (*it == assignment)
			{
				it = _assignments.erase(it);
				return _isDirty = true;
			}
			else
				++it;
		}
	}
	return false;
}
