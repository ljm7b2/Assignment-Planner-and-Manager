#pragma once

#include <algorithm>
#include "Assignment.h"
#include "AssignmentQueue.h"
#include <ctype.h>
#include "Date.h"
#include <fstream>
#include <iostream>
#include <limits>
#include <math.h>
#include <stdio.h>
#include <sstream>
#include <string>

using namespace std;

const string ARROW = "--> "; // decorative arrow symbol
const string COMSPACE = ", "; // to seperate output values to file
const string EXT = ".txt"; // valid file extension
const unsigned int MAX_LINE = 100; // max length of user input line for getline()
const unsigned int MAX_STRING = 50; // max length of user input string for cin>>

class UserInterface
{
public:
	// Constructor
	UserInterface();

	// Public Displays to User
	char Menu_EditAssignment();
	char Menu_Main();
	void Print_Assignments(AssignmentQueue assignments) const; 
	void Message_AssignmentAlreadyExists();
	void Message_AssignmentDoesNotExist();
	void Message_Failed();
	void Message_NumberOfLateAssignments(int number) const;
	void Message_Success();
	void Message_WhichAssignment();

	// Public User Input Methods
	AssignmentStatuses GetStatusFromUser();
	Date GetAssignedDateFromUser();
	Date GetAssignedDateFromUser(Date dueDate);
	Date GetCompletedDateFromUser();
	Date GetCompletedDateFromUser(Date assignedDate);
	Date GetDueDateFromUser();
	Date GetDueDateFromUser(Date assignedDate);
	string GetDescriptionFromUser();
	string GetFileNameFromUser(int minLength = 1, int maxLength = MAX_STRING, string validExtension = "");

	// Public Export/Import
	void Export(AssignmentQueue assignments, string fileName, bool dirty);
	AssignmentQueue Import();

private:
	// Private Data Field
	string userFileName;

	// Private Data Checks / Conversions
	AssignmentStatuses convertStringToAssignmentStatuses(string status);
	bool isInString(string s1, string s2) const;
	bool isNumeric(char c) const;
	bool isValidDateRange(Date firstDate, Date secondDate) const;
	bool stringIsValidAssignmentStatus(string status) const;
	bool stringIsValidDate(string d) const;

	// Private User Input
	Date getDateFromUser();
	char getUserMenuChoice(string validInput);
	string getLineFromUser();
	string getUserString(int minLength, int maxLength, string validInput);
	string getFileName();

	// Private Print Methods
	void print_Assignment(Assignment assignment) const;
};

// Default Constructor

UserInterface::UserInterface()
{
	userFileName = "";
}

// Public Displays to User

char UserInterface::Menu_EditAssignment()
{
	cout << "   What would you like to edit? " << endl
		 << "   [A]: Edit Due Date " << endl
		 << "   [B]: Edit Description " << endl
		 << "   [Q]: Quit " << endl << ARROW;
	return getUserMenuChoice("ABQabq");
}

char UserInterface::Menu_Main()
{
	cout << "Choose from one of the following: " << endl
		 << "[A]: Add Assignment " << endl
		 << "[B]: Edit Assignment " << endl
		 << "[C]: Complete Assignment " << endl
		 << "[D]: Display Assignments " << endl
		 << "[E]: Display Number of Late Assignments " << endl
		 << "[I]: Import Assignments " << endl
		 << "[S]: Save " << endl
		 << "[Q]: Quit  " << endl << ARROW;
	return getUserMenuChoice("ABCDEISQabcdeisq");
}

void UserInterface::Print_Assignments(AssignmentQueue assignments) const
{
	if (assignments.IsEmpty())
	{
		cout << "There are no assignments to print.\n\n";
		return;
	}
	int countA = 0, countCL = 0;
	while (!assignments.IsEmpty())
	{
		Assignment temp = assignments.Pop();
		if (temp.StatusToString() == "Assigned" && countA == 0)
		{
			cout << "\n--ASSIGNED-HOMEWORK--\n";
			++countA;
		}
		if ((temp.StatusToString() == "Completed" || temp.StatusToString() == "Late") && countCL == 0)
		{
			cout << "\n--COMPLETED/LATE-HOMEWORK--\n";
			++countCL;
		}
		print_Assignment(temp);
	}
}

void UserInterface::Message_AssignmentAlreadyExists()
{
	cout << "\n\nAssignment already exists.\nDid not add.\n\n" << endl;
}

void UserInterface::Message_AssignmentDoesNotExist()
{
	cout << "\n\nAssignment does not exist.\nCannot edit.\n\n" << endl;
}

void UserInterface::Message_Failed()
{
	cout << "\n\nOperation failed.\n\n";
}

void UserInterface::Message_NumberOfLateAssignments(int number) const
{
	cout << "\n\nThere are " << number << " late assignments.\n\n";
}

void UserInterface::Message_Success()
{
	cout << "\n\nOperation was successful.\n\n";
}

void UserInterface::Message_WhichAssignment()
{
	cout << "Which assignment would you like to edit?\n";
}

// Public User Input

AssignmentStatuses UserInterface::GetStatusFromUser()
// Gets a status from user
{
	while (true)
	{
		cout << "Status: ";
		string userString = getUserString(4, MAX_STRING, "");
		if (stringIsValidAssignmentStatus(userString))
		{
			return convertStringToAssignmentStatuses(userString);
		}
		cout << "\nInvalid Status.\nValid statuses include:\n1. Assigned\n2. Late\n3. Completed\n\n-->";
	}
}

Date UserInterface::GetAssignedDateFromUser()
// Gets Assigned Date from user
// does not perform date range check
{
	cout << "Assigned Date: ";
	return getDateFromUser();
}

Date UserInterface::GetAssignedDateFromUser(Date dueDate)
// Gets Assigned Date from user
// Performs a date range check
{
	cout << "Assigned Date: ";
	while (true)
	{
		Date assignedDate = getDateFromUser();
		if (isValidDateRange(assignedDate, dueDate))
		{
			return assignedDate;
		}
		cout << "Assigned Date must be before Due Date." << endl << ARROW;
	}
}

Date UserInterface::GetCompletedDateFromUser()
// Gets a Completed Date from user
// no date range check
{
	cout << "Completed Date: ";
	return getDateFromUser();
}

Date UserInterface::GetCompletedDateFromUser(Date assignedDate)
// Gets a Completed Date from user
// Performs a date range check
{
	cout << "Completed Date: ";
	while (true)
	{
		Date completedDate = getDateFromUser();
		if (isValidDateRange(assignedDate, completedDate))
		{
			return completedDate;
		}
		cout << "Completed Date must be after Assigned Date." << endl << ARROW;
	}
}

Date UserInterface::GetDueDateFromUser()
// Gets a Due Date from user
// does not perform date range check
{
	cout << "Due Date: ";
	return getDateFromUser();
}

Date UserInterface::GetDueDateFromUser(Date assignedDate)
// Gets a Due Date from user
// Performs a date range check
{
	cout << "Due Date: ";
	while (true)
	{
		Date dueDate = getDateFromUser();
		if (isValidDateRange(assignedDate, dueDate))
		{
			return dueDate;
		}
		cout << "Due Date must be after Assigned Date." << endl << ARROW;
	}
}

string UserInterface::GetDescriptionFromUser()
// Gets a description from the user
{
	cout << "Description: ";
	return getLineFromUser();
}

string UserInterface::GetFileNameFromUser(int minLength, int maxLength, string validExtension)
// Gets a filename from user.
// Checks for length within bounds, alpha-numeric characters,
// dashes, underscores, and valid file extension.
{
	while (true)
	{
		unsigned int index = 0;
		string fileName = "";
		string fileExtension = "";
		cout << "Please Enter Your Filename with " << EXT << " Extension " << ARROW;
		fileName = getUserString(minLength, maxLength, "");
		while (fileName[index] != '.' && index < fileName.length())
		{
			index++;
		}
		while (index < fileName.length())
		{
			fileExtension += fileName[index];
			index++;
		}
		if (fileExtension != validExtension) cout << "Invalid file. " << endl;
		else if (fileExtension == validExtension)
		{
			ifstream fin;
			fin.open(fileName);
			if (fin.good())
			{
				fin.close();
				userFileName = fileName;
				return fileName;
			}
			else
			{
				cout << "Could not open file. Please try again. " << endl;
				fin.close();
			}
		}
	}
}

// Public Export/Import

void UserInterface::Export(AssignmentQueue assignments, string fileName, bool dirty)
{
	if (dirty)
	{
		ofstream fout("output3.txt"); //change to fileName for final cut

		while (!assignments.IsEmpty())
		{
			Assignment temp = assignments.Pop();

			//temp.AssignedDate().set_format(DateFormat::US);
				
			fout << temp.AssignedDate().toString() + COMSPACE + temp.Description() + COMSPACE +
				    temp.DueDate().toString() + COMSPACE + temp.StatusToString() << endl;
		}
		fout.close();
		cout << "\n--SAVE COMPLETE--\n" << endl;
	}
	else
	{
		cout << "\n--NO CHANGES DETECTED--\n" << endl;
	}
}

AssignmentQueue UserInterface::Import()
{
	int assignmentsNotImported = 0;
	int totalAssignmentsAttempted = 0;
	AssignmentStatuses tempStatus;
	AssignmentQueue assignmentQueue;
	ifstream inputFile(GetFileNameFromUser(4, 20, EXT));
	string tempAssignDate, tempDescription, tempDueDate, tempLine;

	while (inputFile.good())
	{
		++totalAssignmentsAttempted;
		getline(inputFile, tempLine);
		if (tempLine != "")
		{
			String_Tokenizer token(tempLine, ",");
			int counter = 0;
			while (token.has_more_tokens())
			{
				tempLine = token.next_token();
				tempLine.erase(0, tempLine.find_first_not_of(' '));
				tempLine.erase(tempLine.find_last_not_of(' ') + 1);
				
				switch (counter)
				{
				case 0: tempAssignDate = tempLine; break;
				case 1: tempDescription = tempLine; break;
				case 2: tempDueDate = tempLine; break;
				case 3: tempStatus = convertStringToAssignmentStatuses(tempLine); break;
				}
				++counter;
			}
			if (tempStatus == AssignmentStatuses::None)
			{
				++assignmentsNotImported;
				continue;
			}
			try
			{
				Date tempDateAssn(tempAssignDate, DateFormat::US);
				Date tempDateDue(tempDueDate, DateFormat::US);
				//tempDateAssn.set_format(DateFormat::Standard);
				//tempDateDue.set_format(DateFormat::Standard);
				if (tempDateAssn < tempDateDue)
				{
					Assignment tempAssn(tempDateAssn, tempDateDue, tempStatus, tempDescription);
					assignmentQueue.Push(tempAssn);
				}
				else
					++assignmentsNotImported;
			}
			catch (exception)
			{
				++assignmentsNotImported;
			}
		}
	}
	if (assignmentsNotImported > 0)
	{
		cout << "\n--WARNING: " << assignmentsNotImported << " Assignments out of "
			<< totalAssignmentsAttempted << " Not Imported Because of Incorrect Dates/Status.--" << endl
			<< "--Please Re-Check Your Input File and Import Again.--\n" << endl;
	}
	inputFile.close();
	return assignmentQueue;
}

// Private Data Field

string UserInterface::getFileName()
{
	return userFileName;
}

// Private Data Checks / Conversions

AssignmentStatuses UserInterface::convertStringToAssignmentStatuses(string status)
{
	if (stringIsValidAssignmentStatus(status))
	{
		char statusChar = toupper(status[0]);
		if (statusChar == 'A') return AssignmentStatuses::Assigned;
		else if (statusChar == 'L') return AssignmentStatuses::Late;
		else if (statusChar == 'C') return AssignmentStatuses::Completed;
	}
	return AssignmentStatuses::None;
}

bool UserInterface::isInString(string s1, string s2) const
// Returns True of string s1 is in string s2.
// Otherwise, returns False.
{
	return s2.find(s1) != string::npos;
}

bool UserInterface::isNumeric(char c) const
// True if char c is numeric, otherwise false
{
	return c >= 48 && c <= 57;
}

bool UserInterface::isValidDateRange(Date firstDate, Date secondDate) const
// True if firstDate is less than or equal to the secondDate,
// otherwise False
{
	return firstDate < secondDate;
}

bool UserInterface::stringIsValidAssignmentStatus(string status) const
// returns True if string status matches one of the three
// valid assignment status types, otherwise returns false
{
	return (isInString(status, "assignedAssignedASSIGNED") && status.length() == 8)
		|| (isInString(status, "lateLateLATE") && status.length() == 4)
		|| (isInString(status, "completedCompletedCOMPLETED") && status.length() == 9);
}

bool UserInterface::stringIsValidDate(string date) const
// returns True if string "date" represents a valid format
// for dates passed into the Date.h object, otherwise false
// CURRENTLY ONLY SUPPORTS US DATE FORMAT (00/00/0000)
{
	// check length
	if (date.length() != 10)
	{
		return false;
	}
	// check for 00-00-0000 string before entering try block
	for (unsigned int i = 0; i < date.length(); i++)
	{
		if (i == 2 || i == 5)
		{
			// if is digit, invalid
			if (isNumeric(date[i]))
			{
				return false;
			}
		}
		else
		{
			// if not digit, invalid
			if (!isNumeric(date[i]))
			{
				return false;
			}
		}
	}
	// Try to create an instance of Date obj
	try
	{
		Date date(date, DateFormat::US);
		return true;
	}
	catch (exception)
	{
		return false;
	}
}

// Private User Input

char UserInterface::getUserMenuChoice(string validInput)
// gets a single CHAR from user for menu options
{
	string userString = getUserString(1, 1, validInput);
	userString = toupper(userString[0]);
	return userString[0];
}

Date UserInterface::getDateFromUser()
// Gets valid Date from user
{
	while (true)
	{
		string userString = getUserString(4, 10, "");
		if (stringIsValidDate(userString))
		{
			Date userDate(userString, DateFormat::US);
			return userDate;
		}
		cout << "Invalid Date. Retry. Makes sure date is in (MM/DD/YYYY) format. " << endl << ARROW;
	}
}

string UserInterface::getLineFromUser()
// Gets a line from the user
{
	while (true)
	{
		string desc;
		cin.ignore();
		getline(cin, desc);
		if (desc.size() <= MAX_LINE)
		{
			return desc;
		}
		else
		{
			cout << "Too long. Upper limit is " << MAX_LINE << " characters." << endl;
		}
	}
}

string UserInterface::getUserString(int minLength, int maxLength, string validInput)
// Checks user input against a validInput string that's passed into the function.
// If you don't want to check input against validInput string, pass an empty string "" into
// the function.
// Returns the valid string.
{
	string userString = "";
	while (true)
	{
		cin >> userString;
		if (validInput != "" && !isInString(userString, validInput)) cout << "Invalid input. " << endl << ARROW;
		else if (static_cast<int>(userString.length()) < minLength) cout << "Too short. Lower limit is " << minLength << " characters. " << endl << ARROW;
		else if (static_cast<int>(userString.length()) > maxLength) cout << "Too long. Upper limit is " << maxLength << " characters. " << endl << ARROW;
		else return userString;
	}
}

// Private Print Methods

void UserInterface::print_Assignment(Assignment assignment) const
// prints one assignment to the screen
{
	
   cout << "Assigned Date: " << assignment.AssignedDate().toString() << endl
		<< "Due Date: " << assignment.DueDate().toString() << endl
		<< "Description: " << assignment.Description() << endl
		<< "Status: " << assignment.StatusToString() << endl << endl;
}
