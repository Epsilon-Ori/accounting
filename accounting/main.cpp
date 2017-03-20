
#include "accounting.h"
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;
using namespace accounting;

void splitLine(string ln, vector<string> &store) {
	store.clear();
	char delim{ ' ' };
	int pos{ 0 };
	while ((pos = ln.find(delim)) != string::npos) {
		store.push_back(ln.substr(0, pos));
		//cout << "Got token(1): \"" << store[store.size() - 1] << "\"" << endl;
		ln.erase(0, pos + 1);
	}
	//Pushing back whatever did not get erased
	if (!ln.empty()) {
		store.push_back(ln);
		//cout << "Got token(2): \"" << store[store.size() - 1] << "\"" << endl;
	} 
}

date parseDate(string dat) {
	//expected format mm/dd/yyyy
	stringstream ss(dat);
	int month, day, year;
	ss >> month;
	ss.get();
	ss >> day;
	ss.get();
	ss >> year;
	return date(month, day, year);
}

int main(int argc, char *argv[]) {
	//This program should not have any extensions
	string curLine;
	entry *curEntry = nullptr;
	accounting_book book;

	if (argc != 2) {
		std::cout << "Error: Must pass accounting book to use as an argument. If there is no accounting book file, then pass the " <<
			"desired name like this: \"accounting my_book\"" << endl;
		return 1;
	}
	else {
		std::cout << "Opening/creating book " << argv[1] << " ... ";
		ifstream bookFile(argv[1]);
		if (bookFile.good()) {
			book.read(bookFile);
			bookFile.close();
			std::cout << "successfully opened book" << endl << endl;
		}
		else {
			std::cout << "creating book " << argv[1] << endl << endl;
		}
	}
	/*
	List of Commands

	create entry <date>              //Starts creating an entry
	finish                        //Context sensitive, but currently only finishes
	                              //making an entry if making one. If the debits and credits
								  //don't match up then nothing happpens, and an error is printed
	create account <name> <type>     //Creates the account and initialized to zero
	debit <account> <amount>      //Adds a line to the current entry saying to debit 
	                              //an account a certain amount
	credit <account> <amount>     //Adds a line to the current entry saying to credit
	                              //an account a certain amount
	analyze                       //Outputs the journal and the general ledger into
	                              //a CSV file to analyze it through excel or another program
	*/

	vector<string> split;

	std::getline(cin, curLine);
	while ( curLine != "quit") {
		splitLine(curLine, split);
		//Now interpret it
		if (split.size() > 0) {
			//First token
			if (split[0] == "create") {
				if (split.size() > 1) {
					//Second token if the first one is "new"
					if (split[1] == "entry") {
						//Expecting to create a new entry, error if already one there
						if (curEntry) {
							std::cout << "Cannot create new entry, already creating one. Finish this one with \"finish\"" <<
								"before creating a new one." << endl;
						}
						else if (split.size() > 2) {
							curEntry = new entry(parseDate(split[2]));
							std::cout << "Creating entry. Enter lines and type \"finish\" to post" << endl;
						}
						else {
							std::cout << "Need a date parameter when creating new entries. It is in mm/dd/yyyy format." << endl;
						}
					}
					else if (split[1] == "account" && !curEntry) {
						if (split.size() >= 4) {
							if (book.getGeneralLedger()->findAccount(split[2]) == nullptr) {
								account_type type = INVALID;
								if (split[3] == "asset") {
									type = ASSET;
								}
								else if (split[3] == "liability") {
									type = LIABILITY;
								}
								else if (split[3] == "equity") {
									type = EQUITY;
								}
								else if (split[3] == "revenue") {
									type = REVENUE;
								}
								else if (split[3] == "expense") {
									type = EXPENSE;
								}
								else {
									cout << "Invalid account type code \"" << split[3] << "\"" << endl;
								}
								if (type != INVALID) {
									book.getGeneralLedger()->createAccount(split[2], type);
									cout << "Created new " << accountTypeToString(type) << " account with the name " << split[2] << endl;
								}
							}
							else {
								cout << "Account of name " << split[2] << " already exists." << endl;
							}
						}
						else {
							cout << "Creating accounts must be in the format \"create account [name] [type]. \"" <<
								"The type may be asset, liability, equity, revenue or expense" << endl;
						}
					}
					else {
						if (curEntry) {
							std::cout << "There is an entry being created now. Finish this one before adding new entries or accounts." << endl;
						}
						else {
							std::cout << "Can only create accounts or entries" << endl;
						}
					}
				}
				else {
					std::cout << "New without specifiying what to create" << endl;
				}
			}
			else if (curEntry) {
				//The valid commands for when curEntry exists
				if (split[0] == "debit") {
					if (split.size() >= 3) {
						account *debiting = book.getGeneralLedger()->findAccount(split[1]);
						if (debiting) {
							double amt;
							{
								stringstream ss(split[2]);
								ss >> amt;
							}
							//cout << "Debit amount is " << amt << endl;
							curEntry->addLineToEntry(line(split[1], DEBIT, amt));
						}
						else {
							cout << "The account " << split[1] << " was not found in the ledger. Exit this "
								<< "entry and create it with \"new account [name] [type]\"" << endl;
						}
					}
					else {
						cout << "What are you debiting? Include the account name and amount." << endl;
					}
				}
				else if (split[0] == "credit") {
					if (split.size() >= 3) {
						account *crediting = book.getGeneralLedger()->findAccount(split[1]);
						if (crediting) {
							double amt;
							{
								stringstream ss(split[2]);
								ss >> amt;
							}
							//cout << "Credit amount is " << amt << endl;
							curEntry->addLineToEntry(line(split[1], CREDIT, amt));
						}
						else {
							cout << "The account " << split[1] << " was not found in the ledger. Exit this "
								<< "entry and create it with \"create account [name] [type]\"" << endl;
						}
					}
					else {
						cout << "What are you crediting? Include the account name and amount." << endl;
					}
				}
				else if (split[0] == "finish") {
					//Put the newly created entry up to the books
					if (curEntry->isValid()) {
						//Presuppose that all accounts mentioned in the entry exists
						book.post(curEntry); //handles journal and ledger adding
						cout << "Posted entry to the ledger and journal" << endl;
						curEntry = nullptr; //the accounting book has the pointer now
					}
					else {
						cout << "Could not post. Invalid entry (there are either no lines or the credits did not equal debits)" << endl;
						delete curEntry;
						curEntry = nullptr;
					}
				}
				else {
					std::cout << "These lines are invalid for creating a new entry. Do [debit | credit] [account] [amount] or \"finish.\"" << endl;
				}
			}//Other first lines extended here
			else if (split[0] == "analyze") {
				//Output CSV files for journal and the ledger
				book.getGeneralLedger()->analyze_ledger("ledger.csv");
				book.getJournal()->analyze_journal("journal.csv");
				std::cout << "Wrote to journal.csv and ledger.csv" << std::endl;
			}
			else if (split[0] == "close") {
				//Close the books for this period
				//sum revenue and expenses and put into retained earnings
				//the parameter is the name of the next book, which will have
				//no journal entries, but the accounts will be retained
				//the expense and revenue accounts will all be set to 0
				if (split.size() >= 2) {
					//expect a date as a parameter
					book.closeBooks(parseDate(split[1]));
				}
				else {
					std::cout << "Must have a date for when the books are being closed in mm/dd/yyyy format." << endl;
				}

			}
		/*	else if (split[0] == "display") {
				//Output displays a summary of the accounts and the journal entries
				//for the use of debugging and fast feedback, but it has the
				//disadvantage of the user not being able to import it to an external program/

				//Use analyze for exporting it to an external program

				book.getGeneralLedger()->display_ledger();
				book.getJournal()->display_journal();
			}  Disabled for now (ledger does not have a pretty display) */
			else {
				if (split.size() > 0) {
					cout << "Invalid command: \"" << split[0] << "\"" << endl;
				}
			}
		}
		std::getline(cin, curLine);
	}

	std::cout << "Saving book " << argv[1] << " ... ";
	ofstream bookWrite(argv[1]);
	book.write(bookWrite);
	bookWrite.close();
	std::cout << "successfully saved book" << endl;

	return 0;
}