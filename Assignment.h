#pragma once

#include "AssignmentStatus.h"
#include "Date.h"
#include <list>
#include <string>

// Assignment
// This class models an individual homework assignment

class Assignment 
{
public:
	// Constructors
	Assignment();
	Assignment(Date assignedDate, Date dueDate, AssignmentStatuses status, string description);

	// Public Getters
	const AssignmentStatuses Status() const;
	const bool IsLate()const;
	const int ID() const;
	const Date AssignedDate()const;
	const Date CompletedDate()const;
	const Date DueDate()const;
	const int RevisionNumber()const;
	const string Description()const;
	const string StatusToString()const;

	// Public Setters
	void AssignedDate(Date assignedDate);
	void CompletedDate(Date completedDate);
	void Description(string description);
	void DueDate(Date dueDate);
	void Status(AssignmentStatuses status);

	// Overloaded Operator
	bool operator ==(Assignment other) const;

private:
	// Private Data
	AssignmentStatus _status;
	Date _assignedDate;
	Date _completedDate;
	Date _dueDate;
	int _id;
	int _revisionNumber;
	string _description;

	// Private Methods
	int generateID();
};

// Constructors

Assignment::Assignment()
{
	_status.Value(AssignmentStatuses::None);
	_description = "";
	_revisionNumber = 0;
	_id = generateID();
}

Assignment::Assignment(Date assignedDate, Date dueDate, AssignmentStatuses status, string description = "")
{
	_assignedDate = assignedDate;
	_dueDate = dueDate;
	_status.Value(status);
	_description = description;
	_revisionNumber = 0;
	_id = generateID();
}

// Public Getters

const AssignmentStatuses Assignment::Status() const
{
	return _status.Value();
}

const bool Assignment::IsLate() const
{
	return _status.Value() == AssignmentStatuses::Late;
}

const int Assignment::ID() const
{
	return _id;
}

const Date Assignment::AssignedDate() const
{
	return _assignedDate;
}

const Date Assignment::CompletedDate() const
{
	return _completedDate;
}

const Date Assignment::DueDate() const
{
	return _dueDate;
}

const int Assignment::RevisionNumber() const
{
	return _revisionNumber;
}

const string Assignment::Description() const
{
	return _description;
}

const string Assignment::StatusToString() const
{
	return _status.ValueToString();
}

// Public Setters

void Assignment::AssignedDate(Date assignedDate)
{
	if (assignedDate != _assignedDate)
	{
		_assignedDate = assignedDate;
		++_revisionNumber;
	}
}

void Assignment::CompletedDate(Date completedDate)
{
	if (completedDate != _completedDate)
	{
		_completedDate = completedDate;
		++_revisionNumber;
	}
	if (_completedDate > _dueDate)
	{
		_status.Value(AssignmentStatuses::Late);
	}
	else
	{
		_status.Value(AssignmentStatuses::Completed);
	}
}

void Assignment::Description(string description)
{
	if (description != _description)
	{
		_description = description;
		++_revisionNumber;
	}
}

void Assignment::DueDate(Date dueDate)
{
	if (dueDate != _dueDate)
	{
		_dueDate = dueDate;
		++_revisionNumber;
	}
}

void Assignment::Status(AssignmentStatuses status)
{
	if (status != _status.Value())
	{
		_status.Value(status);
		++_revisionNumber;
	}
}

// Overloaded Operators

bool Assignment::operator==(Assignment other) const
{
	return _description == other.Description()
		&& _status.Value() == other.Status()
		&& _dueDate == other.DueDate() 
		&& _assignedDate == other.AssignedDate();
}

// Private Methods

int Assignment::generateID()
{
	static int uid = 1;
	return uid++;
}
