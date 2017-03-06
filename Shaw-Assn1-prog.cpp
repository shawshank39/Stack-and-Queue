//****************************************************************************************
// Code FileName: Shaw-Assn1-prog.cpp
// Description: XYZ Parking lot program. Program first reads in two files and verifies they
//  can be read. Then the program reads in Vehicle information from a file.
//  From the file it will store vehicles into records and then from the records into
//  two separate stacks displaying a message for each vehicle it stores into each stack. The
//  program then displays a message to the screen with a total of current vehicles in each stack.
//  Next, the program will read data from a Checked out Vehicles and returned vehicles file.
//  The information in this file contains check out data (used to checkout vehicles) and return
//  data (used to return vehicles). As information is fed into the program from this file, it will
//  be used to assign employees to vehicles based upon how many employees are waiting. Waiting
//  employees are placed into a queue and are then called by the program and assigned to vehicles.
//  The program will display a message to the user of how many users are assigned to each vehicle and
//  How many employees are left waiting. After all data has been read from the file, the program will
//  store the data about each vehicle parked in the lot back into a new text file. Before exiting,
//  the program will display a message with how many vehicles are remaining and how many employees are waiting.
// Class/Term: CS372 Spring 8 Week 1
// Designer: Kevin Shaw
// Functions:
// inFilesRead - Function opens both files and verifies if they can be read. Displays a message if unsuccessful.
// processEmployeeLot - Creates both stacks and a queue, then calls other functions. *Manager of the program*
// readVehicleFile - Reads data from vehicle file and stores it into 2 stacks.
// printStatusVehicle - Prints total vehicles available to the user.
// readCheckOutFile - Reads Checkout/Return file, pops vehicles off stacks, enqueues and dequeues employees.
// saveLotStatus - Stores vehicle information on remaining vehicles.
// printLotStatus - Displays a message to the screen with total vehicles and waiting employees to the user.
// passengerCalc - Calculates the number of passengers remaining.
// createEmptyStack - Creates an empty stack
// isEmptyStack - Checks to see if the stack is empty.
// isFullStack - Displays an error message if the stack if full.
// getStackCount - Gets the current count of the stack.
// pushStack - Pushes a record onto the stack
// popStack - Pops a record off the stack.
// createEmptyQueue - Creates an empty queue
// isEmptyQueue - Checks to see if the queue is empty.
// isFullQueue - Displays an error message if the queue is full.
// enQueue - Adds a record to the back of the queue.
// deQueue - Removes a record from the front of the queue.
//****************************************************************************************
#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
using namespace std;

// Constants
const string MEMORY_ERROR_1 = "Error - Out of Memory."; // Out of Memory error message.
const string MEMORY_ERROR_2 = "Cannot allocate space."; // Cannot allocate space error message.
const string MEMORY_ERROR_3 = "Program will exit.";     // Program will exit error message.
const string LINE = "****************************************";
const int VAN_MAX = 8;  // Max passengers for a van
const int AUTO_MAX = 4; // Max passengers for a automobile.
const string STATUS_FILE = "LotStatus.txt"; // String for the output file.


// Structures
struct vehicleRec{ // Vehicle Record
	char vehicleType;
	string licensePlate; // String for the License Plate #
	int maxOccupants;	 // integer for the max capacity of the vehicle
	int count;
	vehicleRec* next;    // Pointer to the next record
}; // End of vehicleRec definition
struct waitRec { // Waiting Record
	int numWaiting;
	waitRec* next;
	waitRec* front;
	waitRec* back;
}; // End of waitRec definition


// Prototypes
//bool inFilesRead(char*, char*, ifstream &vehicleFileIn, ifstream &checkOutReturnFileIn);
bool inFilesRead(string, string, ifstream &vehicleFileIn, ifstream &checkOutReturnFileIn);
void processEmployeeLot(ifstream& vehicleFileIn, ifstream& checkOutReturnFileIn, string vehicleFile, string checkOutReturnFile);
void readVehicleFile(ifstream &vehicleFileIn, string vehicleFile, vehicleRec* &stackTopAuto, vehicleRec* &stackTopVan);
void printStatusVehicle(vehicleRec* &stackTopAuto, vehicleRec* &stackTopVan);
void readCheckOutFile(ifstream &checkOutReturnFileIn, string checkOutReturnFile, waitRec* &waitQueue, vehicleRec* &stackTopAuto, vehicleRec* &stackTopVan);
void saveLotStatus(vehicleRec* stackTopVan, vehicleRec* stackTopAuto, waitRec* waitQueue);
void printLotStatus(waitRec* waitQueue, int vanCount, int autoCount);
int passengerCalc(int newGroupNum, int num);
void createEmptyStack(vehicleRec* &stackTop);
bool isEmptyStack(vehicleRec* &stackTop);
int isFullStack();
int getStackCount(vehicleRec* stackTop);
void pushStack(vehicleRec* & stackTop, vehicleRec* &tempNode, bool &success);
void popStack(vehicleRec* &stackTop, vehicleRec* &tempNode, bool &success);
void createEmptyQueue(waitRec* &waitQueue);
bool isEmptyQueue(waitRec* &waitQueue);
int isFullQueue();
void enQueue(waitRec* &waitQueue, int &numWaiting, int &newGroupNum, bool &success);
void deQueue(waitRec* &waitQueue, int &numWaiting, bool &success);


//---------------------------------------------------------------------------------------
// Function: Main
// Description: Function main associates both parameter files with a variable. The function
//	then calls the inFilesRead function to decide if the program should continue. If successful
//  the program will then call the processEmploymentLot function to continue running the program.
// Output: Returns 0 if successful.
// Calls to: inFilesRead, processEmployeeLot
//---------------------------------------------------------------------------------------
int main(int argc, char* argv[])
{
	//Local Variables
	ifstream vehicleFileIn, checkOutReturnFileIn;
	string vehicleFile;
	bool argument1, argument2;
	string checkOutReturnFile;
	char* file1 = NULL;
	char* file2 = NULL;

	cout << "XYZ Company Vehicle Lot Control Program 1.0" << endl << endl;

        // If statement used to determine if the Vehicle File command line argument is missing
	if (argv[1] != NULL) // If not missing... do this
	{
		file1 = argv[1]; // Assign argument 1 to file 1.
		argument1 = true; // argument1 was entered
	}
	else // If missing, argument false.
	{
		argument1 = false;
	}
        
        // If statement used to determine if the CheckOut/ Return command line argument is missing.
	if (argv[2] != NULL) // If not missing... do this
	{
		file2 = argv[2]; // Assign argument 2 to file 2
		argument2 = true; // argument2 was entered
	}
	else // If missing, argument2 is false.
	{
		argument2 = false;
	}
        
        // If both command line arguments were entered proceed... else issue error and terminate.
	if ((argument1) && (argument2))
	{       
                // Both of the following lines convert char* to string for use in later functions.
		vehicleFile = static_cast<string>(file1); 
		checkOutReturnFile = static_cast<string>(file2); 

		if (inFilesRead(vehicleFile, checkOutReturnFile, vehicleFileIn, checkOutReturnFileIn))
		{
			processEmployeeLot(vehicleFileIn, checkOutReturnFileIn, vehicleFile, checkOutReturnFile);
		}

		
	}
	else
	{
		cout << "One or both of the file arguments was not entered." << endl
                     << "Make sure to enter both files as Command Line Arguments" << endl
                     << "This Program will Terminate.";
	}

        cout << endl << endl;

	system("PAUSE");

	return 0;

}

//---------------------------------------------------------------------------------------------
// Function: inFilesRead
// Description: Function inFilesRead displays a message if it receives both files to the user.
//  The function then opens each file and stores a boolean for it the file opened successfully.
//  The function then returns a boolean for if both files were opened successfully or not.
// Input:
//		Parameters:
//			vehicleFile - string for the name of the vehicleFile.
//			checkOutReturnFile - string for the name of the Checkout/Return File.
//			vehicleFileIn - ifstream variable name for the vehicle file.
//			checkoutReturnFileIn - ifstream variable name for the Checkout/Return File.
//		Files:
//			vehicleFileIn - nothing read in, just checks to see if it opens.
//			checkOutReturnFileIn - nothing read in, just checks to see if it opens.
// Output:
//		Returns: bothValid - bool value for if both files open successfully.
//		Parameters: vehicleFileIn and checkOutReturnFileIn are both returned to main.
//---------------------------------------------------------------------------------------------
bool inFilesRead(string vehicleFile, string checkOutReturnFile, ifstream &vehicleFileIn, ifstream &checkOutReturnFileIn)
{
	bool bothValid;
	bool vehicleValid, checkOutValid;
        
        /* Checks the string for the substring ".txt" if not found, the file type is not valid.
         * Will return False if not valid, true if valid. (False will cause the program to terminate). */
	if (vehicleFile.find(".txt") != string::npos)
	{
		vehicleValid = true;
	}
	else
	{
		vehicleValid = false;
	}

        /* Checks the string for the substring ".txt" if not found, the file type is not valid.
         * Will return False if not valid, true if valid. (False will cause the program to terminate). */
	if (checkOutReturnFile.find(".txt") != string::npos)
	{
		checkOutValid = true;
	}
	else
	{
		checkOutValid = false;
	}

	if ((vehicleValid) && (checkOutValid)) // If both files valid, display the file names that were read in.
	{
		cout << "File Names Entered are;" << endl
			<< vehicleFile << endl
			<< checkOutReturnFile << endl << endl;

		vehicleFileIn.open(vehicleFile); // Opens the vehicle file.
		checkOutReturnFileIn.open(checkOutReturnFile); // Opens the Checkout/Return File

		if((!vehicleFileIn) && (!checkOutReturnFileIn)) //If... both files don't open properly
		{
			cout << "Both files did not open properly.\n\nThis Program will close.";
			bothValid = false;
		}
		else if (!vehicleFileIn) //else if... the vehicle file doesn't open properly.
		{
			cout << vehicleFile << " file didn't open properly.\n\nThis Program will close." << endl;
			bothValid = false;
		}
		else if (!checkOutReturnFileIn) // else if... the Check Out file doesn't open properly
		{
			cout << checkOutReturnFile << " file didn't open properly.\n\nThis Program will close." << endl;
			bothValid = false;
		}
		else // Else... both files opened properly
                {
			bothValid = true;
		}

	}
	else if((!checkOutValid) && (vehicleValid)) // If Checkout file is not valid and Vehicle file is valid
        {

		cout << "Check Out / Return file is not a valid file type (.txt).\n\nThis Program will close" << endl;
		bothValid = false;
	}
	else if((!vehicleValid) && (checkOutValid)){ // If Vehicle file is not valid and CheckOut file is valid
		cout << "Vehicle file is not a valid file type (.txt).\n\nThis Program will close" << endl;
		bothValid = false;
	}
	else // Else... both files are not valid
	{
		cout << "Both of the files were not valid file types(.txt).\n\nThis Program will close." << endl;
		bothValid = false;
	}

	// Close both files.
	vehicleFileIn.close();
	checkOutReturnFileIn.close();

	return bothValid;
}

//----------------------------------------------------------------------------------------
// Function: processEmployeeLot
// Description: Function acts as a manager of the program. First creates 2 empty stacks and
//  then an empty queue. Next, the program calls the readVehicleFile, printStatusVehicle,
//  readCheckOutFile, and saveLotStatus functions to move the program along. Before the stacks
//  and queue are created, it creates a new record for each stack and queue to be used in functions.
// Input:
//	Parameters:
//		vehicleFileIn - ifstream variable for the vehicle file.
//		checkOutReturnFileIn - ifstream variable for the Checkout/Return file.
//		vehicleFile - String for the name of the vehicleFile.
//		checkOutReturnFile - String for the name of the vehicleFile.
// Output:
//	Parameters: Both vehicleFileIn and checkOutReturnFileIn are return parameters.
// Calls to: createEmptyStack, createEmptyQueue, readVehicleFile, printStatusVehicle, readCheckOutFile,
//			 saveLotStatus.
//----------------------------------------------------------------------------------------
void processEmployeeLot(ifstream& vehicleFileIn, ifstream& checkOutReturnFileIn, string vehicleFile, string checkOutReturnFile)
{
	// Local Variables
	vehicleRec *stackTopVan, *stackTopAuto;
	waitRec *waitQueue;

	stackTopVan = new (nothrow)vehicleRec; // Creates new top Van Record of type vehicleRec.
        
        // If the stackTopVan record was created, initialize empty stack.
	if (stackTopVan != NULL)
	{
		createEmptyStack(stackTopVan);
	}
	else
	{
		isFullStack();
	}

	stackTopAuto = new (nothrow)vehicleRec; // Creats new top Automobile record of type vehicleRec.

        // If the stackTopAuto record was created, initialize empty stack.
	if (stackTopAuto != NULL)
	{
		createEmptyStack(stackTopAuto);
	}
	else
	{
		isFullStack();

	}

	waitQueue = new (nothrow)waitRec; // Creates new waitQueue record of type waitRec.

        // If the waitQueue record was created, initialize empty stack.
	if (waitQueue != NULL)
	{

		createEmptyQueue(waitQueue);
	}
	else
	{
		isFullQueue();
	}
        
        // calls the readVehicleFile function to read data from vehicle file into the program.
	readVehicleFile(vehicleFileIn, vehicleFile, stackTopAuto, stackTopVan);
        
        // calls the print StatusVehicle function to display the parking lot status to the screen
	printStatusVehicle(stackTopAuto, stackTopVan);
        
        // calls the readCheckOutFile function to grab data from the CheckOut file and process the data as it's read.
	readCheckOutFile(checkOutReturnFileIn, checkOutReturnFile, waitQueue, stackTopAuto, stackTopVan);
        
        // calls the saveLotStatus function to store the parking lot status data once both files have been processed.
	saveLotStatus(stackTopVan, stackTopAuto, waitQueue);

}

//----------------------------------------------------------------------------------------
// Function: readVehicleFile
// Description: Function readVehicleFile opens and then reads data from a vehicle file. As
//  data is read in from the file, the data is placed into linked list of records and then
//  sorted by vehicleType into 2 stacks (one for automobiles and one for vans). Each time
//  a vehicle is placed onto a stack, the program will display a message to the user with
//  this information. If an error occurs in the reading of the file, it will display an error
//  message.
// Input:
//	Parameters:
//		vehicleFileIn - ifstream variable for the vehicle file.
//		vehicleFile - string of the name of the vehicle file.
//		stackTopAuto - top record in the automobile stack.
//		stackTopVan - top record in the van stack.
//  File: vehicleFileIn - vehicle file contains a char vehicleType, string license plate #,
//				and integer capacity.
// OutPut:
//	Parameters: vehicleFileIn, stackTopAuto, and stackTopVan are also return parameters.
// Calls to: isFullStack, pushStack.
//----------------------------------------------------------------------------------------
void readVehicleFile (ifstream &vehicleFileIn, string vehicleFile, vehicleRec* &stackTopAuto, vehicleRec* &stackTopVan)
{
	vehicleRec *tempNode;
	bool pushSuccess;
	char tempVehicleType;

	vehicleFileIn.open(vehicleFile);

	if (vehicleFileIn)
	{
		tempNode = new (nothrow) vehicleRec;

		// priming read
		vehicleFileIn >> tempVehicleType;
		tempVehicleType = toupper(tempVehicleType);

                // While there is data to be read in... Loop
		while (vehicleFileIn && !vehicleFileIn.eof())
		{
			if (tempNode != NULL)
			{
				tempNode->vehicleType = tempVehicleType;
				vehicleFileIn >> tempNode->licensePlate;
				vehicleFileIn >> tempNode->maxOccupants;
			}
			else
			{
				isFullStack();
			}

			if (tempNode->vehicleType == 'A')
			{
				pushStack(stackTopAuto, tempNode, pushSuccess); // Push record onto the top of the automobile stack.

				if (pushSuccess)
					cout << "Placing " << tempNode->licensePlate << " into Automobile area." << endl << endl;
				else
					isFullStack();
			}
			else
			{
				pushStack(stackTopVan, tempNode, pushSuccess); // Push record onto the top of the van stack.

				if (pushSuccess)
					cout << "Placing " << tempNode->licensePlate << " into Van area." << endl << endl;
				else
					isFullStack();
			} //end else

			//priming read for next loop
			vehicleFileIn >> tempVehicleType;
			tempVehicleType = toupper(tempVehicleType);

		} // end While

	} // end if
	else
	{
		cout << "Error - The File could not be opened\n\nExiting Program..." << endl << endl;
	}

	vehicleFileIn.close(); // close the file


} // end function readVehicleFile

//----------------------------------------------------------------------------------------
// Function: printStatusVehicle
// Description: Function printStatusVehicle calls the getStackCount function to get the total
//  count for van's and automobiles and prints the total number of vans and automobiles available
//  to the screen.
// Input:
//	Parameters:
//          stackTopAuto - Top record of the automobile stack.
//          stackTopVan - Top record of the van stack.
// Output:
//  Parameters: Both input parameters are also return parameters.
// Calls to: getStackCount.
//----------------------------------------------------------------------------------------
void printStatusVehicle(vehicleRec* &stackTopAuto, vehicleRec* &stackTopVan)
{
	int autoCount, vanCount;

	autoCount = getStackCount(stackTopAuto);

	vanCount = getStackCount(stackTopVan);

	cout << endl << LINE << endl
		 << "Current parking lot status:" << endl
		 << autoCount << " automobiles available." << endl
		 << vanCount << " vans available." << endl << LINE << endl << endl;

	system("PAUSE");

}

//----------------------------------------------------------------------------------------
// Function: readCheckOutFile
// Description: Function readCheckOutFile read in data from a text file. Data is sorted by the
//  first character in each line indicating whether the data is for a checkout or return. If the
//  char indicates it's a (C)heckout, the program will read in the number of employees waiting.
//  Then will display a message to the screen as to how many employees waiting and will find the
//  largest vehicle possible to transport them. If a vehicle is found that can accomidate them, the
//  program will pop a vehicle of the appropriate size to accomidate them and display a message of the
//  vehicle type, license plate and capacity of the vehicle and will decrement the waiting queue accordingly.
//  If the status character indicates it's a (R)eturn, the program will read in the vehicle type, license
//  plate #, and capacity of the vehicle. If there are employee's currently waiting for a vehicle, the program
//  will dequeue the waiting queue to accomidate the waiting passengers. Function will loop until the entire file
//  has been read in.
// Input:
//  Parameters:
//		checkOutReturnFileIn - ifstream variable for the checkout/return file.
//		checkOutReturnFile - string for the checkout/return file name.
//		waitQueue - Empty record for the waitQueue.
//		stackTopAuto - Top Record for the automobile stack.
//		stackTopVan - Top record for the van stack.
//  File: checkOutReturnFile - This file contains Checkout/Return data (used to check out vehicles and return
//			vehicles. Checkout Data is a char status for (C)heckout, and the number of people waiting. Return
//			data is a char status for (R)eturn, char for vehicle type, string for the license plate #, and integer
//			for the passenger capacity of the vehicle.
// OutPut:
//  Parameters: checkOutReturnFileIn, waitQueue, stackTopAuto, and stackTopVan are also return parameters
// Calls to: isEmptyStack, popStack, isFullStack, passengerCalc, enQueue, isFullQueue, deQueue, pushStack.
//----------------------------------------------------------------------------------------
void readCheckOutFile(ifstream &checkOutReturnFileIn, string checkOutReturnFile, waitRec* &waitQueue, vehicleRec* &stackTopAuto, vehicleRec* &stackTopVan)
{
	vehicleRec *tempNode;
	bool pushSuccess;
	bool popSuccess = true;
	bool enQueSuccess;
	bool deQueSuccess;
	char outOrReturn;
	int groupNum, newGroupNum, numWaiting = 0;

	checkOutReturnFileIn.open(checkOutReturnFile);

	if (checkOutReturnFileIn)
	{
		//priming read
		checkOutReturnFileIn >> outOrReturn;
		outOrReturn = toupper(outOrReturn);

		while (checkOutReturnFileIn && !checkOutReturnFileIn.eof()) // While there is data to be read, continue looping.
		{
			switch (outOrReturn) // Switch sorts by status type ((C)heckout or (R)eturn).
			{
				case 'C':
					tempNode = new (nothrow) vehicleRec;
                                        
                                        //Priming Read
					checkOutReturnFileIn >> groupNum;
					newGroupNum = groupNum;

					// If either the van or automobile stacks are NOT empty display message.
					if ((!isEmptyStack(stackTopAuto)) || (!isEmptyStack(stackTopVan)))
					{
						cout << endl << "For " << groupNum << " employees in group, vehicles checked out are:" << endl;
					}
                                        else // Else display a message that another group has arrived but no vehicles are available.
                                        {   
                                            if (groupNum > 0)
                                                cout << endl << "A Group of employees need a ride, no vehicles available. " << endl;
                                        }

					while (newGroupNum > 0)
					{

                                                /* If (newGroupNum is greater than 4 AND the Van Stack is NOT empty) OR 
                                                 * (VanStack is NOT empty AND the AutoStack is Empty) */
						if (((newGroupNum > AUTO_MAX) && (!isEmptyStack(stackTopVan))) || 
                                                     ((!isEmptyStack(stackTopVan)) && (isEmptyStack(stackTopAuto))))
						{   
                                                   
							/* if the TempNode is NOT empty, pop the top record from the van stack and store it
							   into tempnode. */
							if (tempNode != NULL)
								popStack(stackTopVan, tempNode, popSuccess);
							else
								isFullStack();

							/* If the pop is successful, call the passengerCalc function to calculate the new
							   group number and display a message to the user, else add the waiting members to the
							   wait queue.*/
							if (popSuccess)
							{
								newGroupNum = passengerCalc(newGroupNum, VAN_MAX);

								cout << "Van " << tempNode->licensePlate << " holds " << tempNode->maxOccupants
									<< " passengers." << endl;
							} // End if

						} 
                                                /* Else if... (Group Number is less than or equal to 4 AND Auto Stack is NOT empty) OR
                                                   (AutoStack is NOT empty AND Van Stack is empty)*/
						else if (((newGroupNum <= AUTO_MAX) && (!isEmptyStack(stackTopAuto))) || ((!isEmptyStack(stackTopAuto)) &&(isEmptyStack(stackTopVan)))) 
						{   
                                                    
                                                        /* If the TempNode is NOT empty, pop the top record from the automobile stack and store
							   it into tempNode.*/
							if (tempNode != NULL)
							{
								popStack(stackTopAuto, tempNode, popSuccess);
							}
							else
								isFullStack();

							/* If the pop is successful, call the passengerCalc function to calculate the new group number
							    and display a message of the vehicle and the amount leaving in the vehicle(s), else add the
								waiting members to the wait queue.*/
							if (popSuccess)
							{
								newGroupNum = passengerCalc(newGroupNum, AUTO_MAX);

								cout << "Automobile " << tempNode->licensePlate << " holds " << tempNode->maxOccupants
									<< " passengers." << endl;

							} // End inner if
                                                    

						} // end else if
                                                /* Else if... group number is greater than 0 AND both stacks are empty */
                                                else if ( (isEmptyStack(stackTopAuto)) && (isEmptyStack(stackTopVan)) && (newGroupNum > 0))
						{
                                                   
                                                    if (newGroupNum > 0)
                                                    {
							enQueue(waitQueue, numWaiting, newGroupNum, enQueSuccess);
                                                                
							if (enQueSuccess)
							{
								newGroupNum = 0;
								cout << numWaiting << " employees in group, waiting for a vehicle." << endl;
							} // End if
							else
								isFullQueue();
                                                    }            
						} // end else
                                                
					} // End inner while

					delete tempNode;
					break;


				case 'R':
                                    
					tempNode = new (nothrow) vehicleRec;

					// If tempNode is NOT NULL, read in vehicle data from the file.
					if (tempNode != NULL)
					{
						checkOutReturnFileIn >> tempNode->vehicleType;
						checkOutReturnFileIn >> tempNode->licensePlate;
						checkOutReturnFileIn >> tempNode->maxOccupants;

						// Switch statement sorts the data by the vehicle type being returned.
						switch (tempNode->vehicleType)
						{
							case 'V':
								cout << endl;
								cout << "Returning " << tempNode->licensePlate << " into the Van area." << endl;

								deQueue(waitQueue, numWaiting, deQueSuccess);

								/* If the dequeue was successful, calculate the new total of employees waiting and
								   display a message of how many waiting members were placed into the transport vehicle, else
								   push the vehicle into the van stack.*/
								if (deQueSuccess)
								{
									newGroupNum = numWaiting;

									newGroupNum = passengerCalc(newGroupNum, VAN_MAX);

									cout << "For " << numWaiting << " employees in group waiting, vehicles checked out are:" << endl;
									cout << "Van " << tempNode->licensePlate << " holds " << tempNode->maxOccupants;
									cout << " passengers." << endl;

									/* If there is still members waiting, add them to the wait queue and display a message
									   of how many employees remain.*/
									if (newGroupNum > 0)
									{
										enQueue(waitQueue, numWaiting, newGroupNum, enQueSuccess);

										if (enQueSuccess)
										{
											cout << numWaiting << " employees in group, still waiting for a vehicle." << endl;

										}
										else
										{
											isFullQueue();
										}
									}

								}
								else
								{
									//PUSHSTACK
									pushStack(stackTopVan, tempNode, pushSuccess);

									if (!pushSuccess)
									{
										isFullStack();
									}
								}
								break;

							case 'A':

								cout << endl << "Returning "<< tempNode->licensePlate << " into the automobile area." << endl;

								deQueue(waitQueue, numWaiting, deQueSuccess);

								/* If the dequeue was successful, calculate the new total of employees waiting and
								display a message of how many waiting members were placed into the transport vehicle, else
								push the vehicle into the auto stack.*/
								if (deQueSuccess)
								{
									newGroupNum = numWaiting;

									newGroupNum = passengerCalc(newGroupNum, AUTO_MAX);

									cout << "For " << numWaiting << " employees in group waiting, vehicles checked out are:" << endl;
									cout << "Automobile " << tempNode->licensePlate << " holds " << tempNode->maxOccupants;
									cout << " passengers." << endl << endl;

									/* If there is still members waiting, add them to the wait queue and display a message
									of how many employees remain.*/
									if (newGroupNum > 0)
									{
										enQueue(waitQueue, numWaiting, newGroupNum, enQueSuccess);

										if (enQueSuccess)
										{
											cout << numWaiting << " employees still waiting for a vehicle." << endl << endl;

										}
										else
										{
											isFullQueue();
										}
									} // End Inner if

								} // End if
								else
								{
									//PUSHSTACK
									pushStack(stackTopAuto, tempNode, pushSuccess);

									if (!pushSuccess)
									{
										isFullStack();
									}
								} // End else

							default:
								break;
						} // End inner switch
					} // End if
					else
						isFullStack();

					delete tempNode;
					break;

				default:
				break;

			} // End switch

			// Priming read
			checkOutReturnFileIn >> outOrReturn;
		} // End while


	} // End if
	else
	{
		cout << "Error - The File could not be opened\n\nExiting Program..." << endl << endl;
	}// End else

	checkOutReturnFileIn.close();
	cout << endl;

	system("PAUSE");
	return;

} // End Function readCheckOutFile

//---------------------------------------------------------------------------------------
// Function: saveLotStatus
// Description: Function saveLotStatus Opens a file for reading, creates a temp record used to
//  write data to a file, then writes each stack of vehicle into the text file (Van and Automobiles).
//  See Output File information below for what data is written. Once the file has been written to
//  the file is saved and the function then call the PrintLotStatus function to display a lot status
//  to the user.
// Input:
//  Parameters:
//		stackTopVan - Top record in the van stack.
//		stackTopAuto - Top record in the automobile stack.
//		waitQueue - current wait queue.
// Output:
//  File: lotStatusOut - Output file contains record information on vehicle data. This data includes;
//			character value of the vehicle type (van or automobile), vehicle license plate #,
//			vehicle capacity.
// Calls to: popStack, isFullStack, printLotStatus, getStackCount;
//---------------------------------------------------------------------------------------
void saveLotStatus(vehicleRec* stackTopVan, vehicleRec* stackTopAuto, waitRec* waitQueue)
{
	ofstream lotStatusOut;
	vehicleRec *tempNode;
	int vanCount = 0, autoCount = 0;
	bool popSuccess = true;
	char tempVehicleType;
	string tempLicensePlate;
	int tempMaxOccupants;

	lotStatusOut.open(STATUS_FILE.c_str());

	tempNode = new (nothrow) vehicleRec;

	if (tempNode != NULL)
	{
		vanCount = getStackCount(stackTopVan);

		while (lotStatusOut && popSuccess)
		{
			popStack(stackTopVan, tempNode, popSuccess);

			if (popSuccess)
			{
				tempVehicleType = tempNode->vehicleType;
				lotStatusOut << tempVehicleType << " ";

				tempLicensePlate = tempNode->licensePlate;
				lotStatusOut << tempLicensePlate << " ";

				tempMaxOccupants = tempNode->maxOccupants;
				lotStatusOut << tempMaxOccupants << endl;
			}

		}

		autoCount = getStackCount(stackTopAuto);

		popSuccess = true;

		while (lotStatusOut && popSuccess)
		{
			popStack(stackTopAuto, tempNode, popSuccess);

			if (popSuccess)
			{
				tempVehicleType = tempNode->vehicleType;
				lotStatusOut << tempVehicleType << " ";

				tempLicensePlate = tempNode->licensePlate;
				lotStatusOut << tempLicensePlate << " ";

				tempMaxOccupants = tempNode->maxOccupants;
				lotStatusOut << tempMaxOccupants << endl;
			}

		}
	}
	else
	{
		isFullStack();
	}

	delete tempNode;
	delete stackTopVan;
	delete stackTopAuto;

	lotStatusOut.close();

	printLotStatus(waitQueue, vanCount, autoCount);


	return;

}// End Function saveLotStatus

//-----------------------------------------------------------------------------------------
// Function: printLotStatus
// Description: Function printLotStatus calls the deQueue function get the total amount of
//  employees still waiting. The function then displays the total automobiles, vans, and
//  waiting employees to the user.
// Input:
//  Parameters:
//		waitQueue - current wait queue.
//		vanCount - total van's available in the lot.
//		autoCount - total automobiles available in the lot.
// Calls to: deQueue
//-----------------------------------------------------------------------------------------
void printLotStatus(waitRec* waitQueue, int vanCount, int autoCount)
{
	int numWaiting;
	int totalWaiting = 0;
	bool deQueSuccess = true;

	while (deQueSuccess)
	{
		deQueue(waitQueue, numWaiting, deQueSuccess);

		// If deQueue is successful, add the employee to the total waiting.
		if (deQueSuccess)
		{
			totalWaiting = totalWaiting + numWaiting;
		}

	}
	cout << endl;
	cout << "****Parking Lot Status****" << endl;
	cout << vanCount << " vans available." << endl;
	cout << autoCount << " automobiles available."<< endl;

	if (totalWaiting > 0)
	{
		cout << totalWaiting << " employees waiting for a vehicle." << endl << endl;
	}

	delete waitQueue;

	return;

}// End Function printLotStatus

//---------------------------------------------------------------------------------------
// Function: passengerCalc
// Description: Function passengerCalc, calculates the number of employees waiting.
// Input:
//  Parameters:
//      newGroupNum - total waiting.
//      num - total capacity of vehicle.
// OutPut:
//  Returns: newGroupNum - total employees waiting.
//---------------------------------------------------------------------------------------
int passengerCalc(int newGroupNum, int num)
{
	if (newGroupNum >= num)
	{
		newGroupNum = newGroupNum - num;

	}
	else 
	{
		newGroupNum = 0;
	}
        //cout << "New Group Number in passengerCalc... " << newGroupNum << endl;
	return newGroupNum;

}// End Function passengerCalc

//----------------------------------------------------------------------------------------
// Function: createEmptyStack
// Description: Function createEmptyStack creates an empty stack.
// Input:
//	Parameters:
//		stackTop - top of the current stack.
// Output:
//	Parameters:
//		stackTop - top of the newly created stack.
//----------------------------------------------------------------------------------------
void createEmptyStack(vehicleRec* &stackTop)
{
	stackTop->next = NULL;
	stackTop->count = 0;


}

//----------------------------------------------------------------------------------------
// Function: isEmptyStack
// Description: Function isEmptyStack checks to see if a stack is empty.
// Input:
//	Parameters:
//		stackTop - top of the current stack.
// OutPut:
//	Parameters:
//		stackTop - unaltered top of the current stack.
//  Returns: stackEmpty - bool value for if the stack is empty or not.
//----------------------------------------------------------------------------------------
bool isEmptyStack(vehicleRec* &stackTop)
{
	bool stackEmpty;

	if (stackTop->next == NULL)
	{
		stackEmpty = true;
	}
	else
	{
		stackEmpty = false;
	}

	return stackEmpty;
}

//----------------------------------------------------------------------------------------
// Function: isFullStack
// Description: Displays error message if called and returns 1;
// Output:
//  Returns: 1
//----------------------------------------------------------------------------------------
int isFullStack()
{
	cout << MEMORY_ERROR_1 << endl
		<< MEMORY_ERROR_2 << endl << endl
		<< MEMORY_ERROR_3 << endl << endl;

	return 1;
}

//----------------------------------------------------------------------------------------
// Function getStackCount
// Description: Function getStackCount gets the current count for the top of the stack.
// Input:
//  Parameters:
//		stackTop - top of the current stack.
// Output:
//  Returns: stackCount - current count of the top of the stack.
//----------------------------------------------------------------------------------------
int getStackCount(vehicleRec* stackTop)
{
	int stackCount;

	stackCount = stackTop->count;

	return stackCount;
}

//----------------------------------------------------------------------------------------
// Function: pushStack
// Description: Function pushStack pushes a record to the top of the stack.
// Input:
//  Parameters:
//		stackTop - current top of the stack.
//		tempNode - new record that will become the top of the stack.
//		success - "Empty" boolean to be used in the function.
// Output:
//  Parameters:
//		stackTop - new top of the stack
//		tempNode - tempnode that will be deleted.
//		success - bool value for whether or not the push was successful.
//----------------------------------------------------------------------------------------
void pushStack(vehicleRec* &stackTop, vehicleRec* &tempNode, bool &success)
{
	vehicleRec *newNode;
	newNode = new (nothrow)vehicleRec;

	if (newNode != NULL)
	{
		newNode->vehicleType = tempNode->vehicleType;
		newNode->licensePlate = tempNode->licensePlate;
		newNode->maxOccupants = tempNode->maxOccupants;
		newNode->next = stackTop->next;
		stackTop->next = newNode;
		stackTop->count++;
		success = true;
	}
	else
		success = false;

	return;

} // End function pushStack

//----------------------------------------------------------------------------------------
// Function: popStack
// Description: Function popStack pops off the top of the stack and assigns the record to
//  tempnode. The original top of the stack is then deleted and the function returns the new
//  top of the stack.
// Input:
//  Parameters:
//		stackTop - current top of the stack.
//		tempNode - "Empty" record to store the popped node into.
//		success - "Empty" bool value to be calculated in the function.
// Output:
//	Parameters:
//		stackTop - new top of the stack.
//		tempNode - original top of the stack.
//		success - bool value for whether or not the pop was successful.
//----------------------------------------------------------------------------------------
void popStack(vehicleRec* &stackTop, vehicleRec* &tempNode, bool &success)
{
	vehicleRec *popNode;

	if (isEmptyStack(stackTop))
		success = false;
	else
	{
		popNode = stackTop->next;
		tempNode->vehicleType = popNode->vehicleType;
		tempNode->licensePlate = popNode->licensePlate;
		tempNode->maxOccupants = popNode->maxOccupants;
		stackTop->next = popNode->next;
		stackTop->count--;
		delete popNode;
		success = true;

	} // end else

	return;
}

//-------------------------------------------------------------------------------------
// Function: createEmptyQueue
// Description: Function createEmptyQueue creates and empty queue.
// Input:
//	Parameters:
//		waitQueue - record to be emptied to create the new queue.
// Output:
//	Parameters:
//		waitQueue - newly emptied queue.
//-------------------------------------------------------------------------------------
void createEmptyQueue(waitRec* &waitQueue)
{
	//Set queue to empty queue
	waitQueue->front = NULL;
	waitQueue->back = NULL;

	return;
}

//--------------------------------------------------------------------------------------
// Function: isEmptyQueue
// Description: Function isEmptyQueue checks to see if the queue is empty.
// Input:
//  Parameters:
//		waitQueue - current queue to be processed.
// Output:
//	Parameters: waitQueue - returns the unaltered queue.
//	Returns: queueEmpty - bool value for if the queue is empty.
//--------------------------------------------------------------------------------------
bool isEmptyQueue(waitRec* &waitQueue)
{
	bool queueEmpty;

	if (waitQueue->front == NULL)
	{
		queueEmpty = true;
	}
	else
	{
		queueEmpty = false;
	}

	return queueEmpty;
}

//--------------------------------------------------------------------------------------
// Function: isFullQueue
// Description: Function isFullQueue displays and error message if called and returns 1;
// Output:
//	Returns: 1
//--------------------------------------------------------------------------------------
int isFullQueue()
{
	cout << MEMORY_ERROR_1 << endl
		<< MEMORY_ERROR_2 << endl << endl
		<< MEMORY_ERROR_3 << endl << endl;

	return 1;
}

//--------------------------------------------------------------------------------------
// Function: enQueue
// Description: Function enQueue adds a record to the back of the queue.
// Input:
//	Parameters:
//		waitQueue - current queue.
//		numWaiting - Empty value to feed in the number of waiting employees.
//		newGroupNum - current waiting employees.
//		success - Empty bool value to be calculated in the function.
// Output:
//	Parameters:
//		waitQueue - new queue after adding members to the queue.
//		numWaiting - new value of the number of waiting employees (to be displayed in another function).
//		groupNum - original value the waiting employees.
//		success - bool value for if the record was added to the queue.
//--------------------------------------------------------------------------------------
void enQueue(waitRec* &waitQueue, int &numWaiting, int &newGroupNum, bool &success)
{
	waitRec* newQNode;

	newQNode = new (nothrow)waitRec;

	if (newQNode != NULL)
	{
		numWaiting = newGroupNum;
		newQNode->numWaiting = newGroupNum;
		newQNode->next = NULL;

		/* If the current queue is empty, assign the record to the front and back of the queue,
		   else, add the record to the back of the queue.*/
		if (isEmptyQueue(waitQueue))
		{
			waitQueue->front = newQNode;
			waitQueue->back = newQNode;

		}
		else
		{
			waitQueue->back->next = newQNode;
			waitQueue->back = newQNode;
		}

		success = true;
	}
	else
		success = false;

	return;
}

//--------------------------------------------------------------------------------------
// Function: deQueue
// Description: Function deQueue removes a record from the front of the queue if there is
//	current records in the queue.
// Input:
//  Parameters:
//		waitQueue - current queue.
//		numWaiting - empty value to assign the number of waiting employees.
//              success - empty bool value to be calculated by the function.
// Output:
//	Parameters:
//		waitQueue - newly adjusted queue.
//		numWaiting - new value for current waiting employees.
//		success - bool value for if the deQueue process was successful.
//--------------------------------------------------------------------------------------
void deQueue(waitRec* &waitQueue, int &numWaiting, bool &success)
{
	waitRec* temp;

	if (!isEmptyQueue(waitQueue))
	{
		numWaiting = waitQueue->front->numWaiting;
		temp = waitQueue->front;
		waitQueue->front = waitQueue->front->next;

		delete temp; // Delete the first node;

		if (waitQueue->front == NULL) //If after deletion the queue is empty set the queue Rear to NULL
			waitQueue->back = NULL;

		success = true;
	}
	else
		success = false;
}

