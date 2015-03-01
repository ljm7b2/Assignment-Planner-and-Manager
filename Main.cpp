#include "UserInterface.h"
#include "AssignmentManager.h"

using namespace std;

void Add(AssignmentManager& am, UserInterface& ui);
void Complete(AssignmentManager& am, UserInterface& ui);
void Edit(AssignmentManager& am, UserInterface& ui);

void main()
{
	// Initialize objects and variables
	AssignmentManager am;
	UserInterface ui;
	bool quit = false;
	char choice;
	string fileName;

	// Program Outer Loop
	while (!quit)
	{
		// Show Main Menu, Get Choice
		choice = ui.Menu_Main();

		// Evaluate choice and Perform Action
		switch (choice)
		{
		case 'A': Add(am, ui); break; // Add Assignment
		case 'B': Edit(am, ui); break; // Edit Assignment
		case 'C': Complete(am, ui); break; // Complete Assignment
		case 'D': ui.Print_Assignments(am.GetAllAssignments()); break; // Print Assignments to Screen
		case 'E': ui.Message_NumberOfLateAssignments(am.NumberOfLateAssignments()); break; // Display Number of Late Assignments		
		case 'I': am.AddAssignments(ui.Import()); break; // Import
		case 'S': ui.Export(am.Save(), fileName, am.IsDirty()); break; // Save
		case 'Q': quit = true; break; // Quit
		default: break;
		}
	}
}

void Add(AssignmentManager& am, UserInterface& ui)
{
	AssignmentStatuses status;
	Date assignedDate;
	Date dueDate;
	string description;

	while (true)
	{
		dueDate = ui.GetDueDateFromUser(); // due date (no date range check)
		while (true)
		{
			assignedDate = ui.GetAssignedDateFromUser(dueDate); // assigned date (with date range check)
			if (!am.AssignmentExists(assignedDate)) // check for existence
			{
				break;
			}
			ui.Message_AssignmentAlreadyExists(); // assignment already exists
		}
		status = ui.GetStatusFromUser(); // status
		description = ui.GetDescriptionFromUser(); // description
		if (!am.AddAssignment(assignedDate, dueDate, status, description)) // attempt an add
		{
			ui.Message_AssignmentAlreadyExists(); // assignment already exists, did not add
		}
		ui.Message_Success(); // add was successful
		break;
	}
}

void Complete(AssignmentManager& am, UserInterface& ui)
{
	Date assignedDate;
	Date completedDate;

	while (true)
	{
		assignedDate = ui.GetAssignedDateFromUser(); // which assignment?
		if (am.AssignmentExists(assignedDate)) // check for existence
		{
			break;
		}
		ui.Message_AssignmentDoesNotExist();
	}
	completedDate = ui.GetCompletedDateFromUser(assignedDate); // get completed date (with range check)
	if (!am.CompleteAssignment(assignedDate, completedDate)) // attempt to complete
	{
		ui.Message_Failed();
	}
	ui.Message_Success();
}

void Edit(AssignmentManager& am, UserInterface& ui)
{
	Date assignedDate;
	Date newDueDate;
	string newDescription;

	ui.Message_WhichAssignment();

	while (true)
	{
		// Choose which assignment to edit
		assignedDate = ui.GetAssignedDateFromUser(); // which assignment?
		if (am.AssignmentExists(assignedDate)) // check for existence
		{
			break;
		}
		ui.Message_AssignmentDoesNotExist();
	}
	// Show Edit Assignment Menu, get choice
	char choice = ui.Menu_EditAssignment();
	switch (choice)
	{
	case 'A': // Edit Due Date
		newDueDate = ui.GetDueDateFromUser(assignedDate); // new due date (with date range check)
		if (!am.EditAssignment(assignedDate, newDueDate)) // attempt an edit
		{
			ui.Message_Failed(); // edit failed
		}
		ui.Message_Success(); // edit was successful
		break;
	case 'B': // Edit Description
		newDescription = ui.GetDescriptionFromUser(); // new description
		if (!am.EditAssignment(assignedDate, newDescription)) // attempt an edit
		{
			ui.Message_AssignmentDoesNotExist();
		}
		ui.Message_Success();
		break;
	case 'Q': // Quit Edit Menu
	default:
		break;
	}
}
