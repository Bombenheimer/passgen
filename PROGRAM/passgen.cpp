/*
 * MIT License
 *
 * Copyright (c) 2024 Bombenheimer
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

// DEFINE MACRO FOR GNU SOURCE
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

// INCLUDING STANDARD LIBRARIES
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <ciso646>
#include <getopt.h>
#include <cstdlib>

// INCLUDING NAMESPACES
using namespace std;

// MINIMUM, MAXIMUM, AND DEFAULT NUMBER OF USERS
#define _MIN_USERS 1
#define _MAX_USERS 100

// MINIMUM AND MAXIMUM USERNAME LENGTH
#define _MIN_USERNAME_LEN 8
#define _MAX_USERNAME_LEN 32

// MINIMUM, MAXIMUM, AND DEFAULT PASSWORD LENGTH
#define _MIN_PASSWD_LEN 8
#define _DEFAULT_PASSWD_LEN 12
#define _MAX_PASSWD_LEN 50

// DEFAULT INCLUSION OF NUMBERS, LOWERCASE AND UPPERCASE LETTERS, AND SYMBOLS
#define _DEFAULT_INCLUDE true

// DEFALT BEHAVIOR FOR OPTIONAL ARGUMENTS
#define _DEFAULT_EXCLUDE false

// DEFAULT BEHAVIOR FOR FILES
#define _DEFAULT_FIND_FILE false
#define _DEFAULT_FILENAME "passwords.txt"
#define _MAX_FILENAME_LEN 30
#define _MAX_FILE_PATH_LEN 250

// CHECK OPERATING SYSTEM FOR ROOT USER PATH
#if defined(__linux__)
	#define _DEFAULT_PATH "/root"

#elif defined(__APPLE__) and defined(__MACH__)
	#define _DEFAULT_PATH "/var/root"

#endif

// MESSAGE COLORS
#define _COL_RED "\033[38;5;196m"
#define _COL_YELLOW "\033[38;5;184m"
#define _COL_GREEN "\033[38;5;46m"
#define _COL_OFF "\033[38;5;231m"

// VERSION NUMBER
#define _VER_NUM "1.0.0"

// FUNCTION PROTOTYPES
void PrintUsage(unsigned short int errorNum = 0);
int GetSystemUsers(unsigned short int numUsers = 1, string* userList = nullptr, bool findUsers = false);
void GenPasswordChars(unsigned short int numUsers = 1, unsigned short int passwordLength = 12, string* passwordList = nullptr, bool hasNumbers = true, bool hasLowers = true, bool hasUppers = true, bool hasSpecChars = true, bool useWords = false, bool findWords = false);
void GenPasswordWords(unsigned short int numUsers = 1, string* passwordList = nullptr, string* wordList = nullptr, bool useWords = true, bool findWords = false);
void WriteToFIle(unsigned short int numUsers = 1, string* userList = nullptr, string* passwordList = nullptr, string filename = _DEFAULT_FILENAME, string defaultPath = _DEFAULT_PATH, bool partitionFile = false);
void ChangeUserPasswds(unsigned short int numUsers = 1, string* userList = nullptr, string* passwordList = nullptr, bool changePasswd = false);

/*
 * FUNCTION NAME: main()
 * 
 * INPUT PARAMETERS:
 * 	argc: INTEGER TYPE VARIABLE
 * 	argv: TWO-DIMENSIONAL ARRAY OF CHARS
 *
 * DESCRIPTION:
 * 	MAIN FUNCTION THAT SERVES AS THE ENTRY AND EXIT POINT OF THE
 * 	PROGRAM AND WHERE ALL PROGRAM FUNCTIONALITY WILL OCCUR.
 * 	IT WILL FIRST INITIALIZE THE NUMBER OF USERS AND PASSWORD
 * 	LENGTH VARIABLES TO DEFAULT VALUES, WHICH ARE UNSIGNED SHORT
 * 	INTEGERS. NEXT, IT WILL INITIALIZE BOOLEANS FOR PASSWORDS
 * 	WHICH ARE SET TO TRUE BY DEFAULT. NEXT, IT WILL INITIALIZE
 * 	BOOLEANS FOR OTHER PROGRAM OPERATIONS WHICH ARE SET TO FALSE
 * 	BY DEFAULT. NEXT, IT WILL ALLOCATE NEW MEMORY TO THE USER
 * 	LIST AND PASSWORD LIST ARRAYS WHICH WILL KEEP TRACK OF
 * 	USERNAMES AND THEIR PASSWORDS, BOTH OF WHICH ARE SET TO A
 * 	SIZE DEFINED BY THE NUMBER OF USERS AND PASSWORD LENGTH
 * 	VARIABLES. NEXT, IT WILL NITIALIZE VARIABLES NEEDED FOR
 * 	FILE OPERATIONS WHICH ARE ALL STRINGS. FILENAME IS SET
 * 	TO DEFAULT VALUE, BUT USER LIST PATH, WORD LIST PATH, AND
 * 	DEFAULT PATH FOR FILE PLACEMENT DO NOT HAVE VALUES BY
 * 	DEFAULT. AFTER VARIABLE INITIALIZATION, THE PROGRAM WILL
 * 	START. FIRST, IT WILL CHECK IF THE PROGRAM IS BEING RAN
 * 	AS THE SYSTEM'S ROOT USER BY CHECKING THE EUID. IF IT IS
 * 	NOT 0 (MEANING ROOT), RAISE AN ERROR MESSAGE AND EXIT. IF
 * 	THE USER IS A ROOT USER, THEN IT WILL START EXECUTING THE
 * 	CONTENTS OF THE PROGRAM AND CONTINUE. IN THE CONTENTS, 
 * 
 * PROGRAM EXECUTION STEPS:
 * 	1 - INITIALIZE VARIABLES
 *
 * 	2 - CHECK IF THE PROGRAM IS BEING RAN AS ROOT
 *
 * 	3 - SET PROGRAM MACRO errno TO 0
 *
 * 	4 - INITIALIZE STRUCTURE TO HOLD LONG OPTIONS ALONG WITH THEIR SHORT
 * 	    FORM EQIVALENTS, WHETHER OR NOT AN ARGUMENT IS REQUIRED FOR EACH
 * 	    OPTION, AND THE FLAGS WHICH ARE SET TO nullptr
 *
 * 	5 - 
 *
 * 	6 - 
 *
 * 	7 - 
 *
 * 	8 - 
 *
 * 	9 - 
 *
 * 	10 - 
 * 	
 * 	11 - 
 * 	
 * 	12 - 
 *
 * RETURN VALUES:
 * 	0 - PROGRAM SUCCESS
 * 	1 - AN OPERATION WAS NOT ALLOWED / PROGRAM FAILURE
 * 	2 - FILE OR DIRECTORY NOT FOUND / PROGRAM FAILURE
 */
int main(int argc, char* argv[])
{
	// PROGRAM-WIDE INTEGER VARIABLES
	unsigned short int numUsers = _MIN_USERS; // 1
	unsigned short int passwordLength = _DEFAULT_PASSWD_LEN; // 12

	// PROGRAM-WIDE BOOLEAN VARIABLES
	bool hasNumbers = _DEFAULT_INCLUDE, // TRUE
	     hasLowers = _DEFAULT_INCLUDE, // TRUE
	     hasUppers = _DEFAULT_INCLUDE, // TRUE
	     hasSpecChars = _DEFAULT_INCLUDE; // TRUE
	
	bool partitionFile = _DEFAULT_EXCLUDE,
	     useWords = _DEFAULT_EXCLUDE, // FALSE
	     findWords = _DEFAULT_EXCLUDE, // FALSE
	     findUsers = _DEFAULT_EXCLUDE, // FALSE
	     changePasswd = _DEFAULT_EXCLUDE; // FALSE

	// PROGRAM-WIDE VECTORS
	vector <string> userList(1); // 1 USER
	vector <string> passwordList(1); // 1 PASSWORD
	vector <string> wordList; // NO WORDS UNLESS PROVIDED BY USER

	// PROGRAM-WIDE STRING VARIABLES
	string fileName = _DEFAULT_FILENAME; // passwords.txt
	string userListPath; // NO PATH TO USER LIST FILE UNLESS PROVIDED BY USER
	string wordListPath; // NO PATH TO WORD LIST FILE UNLESS PROVIDED BY USER
	string defaultPath = _DEFAULT_PATH; // DEFAULT PATH IS BASED ON OPERATING SYSTEM
	
	// CHECK IF PROGRAM IS BEING RAN AS ROOT. IF NOT, RAISE AN ERROR
	if (geteuid() != 0)
	{
		errno = 1;
		cerr << _COL_YELLOW << "WARNING" << _COL_OFF << " : Passgen MUST be ran as root!" << endl;
		cerr << "passgen : " << strerror(errno) << endl;
		return errno;
	}
	else
	{
		// RESET ERROR NUMBER TO 0
		errno = 0;
		
		// STRUCTURE TO HOLD LONG OPTIONS
		static struct option long_options[11] =
		{
			{"help", no_argument, 0, 'h'},
			{"exclude-nums", no_argument, nullptr, 'N'},
			{"exclude-lower", no_argument, nullptr, 'L'},
			{"exclude-upper", no_argument, nullptr, 'U'},
			{"exclude-chars", no_argument, nullptr, 'S'},
			{"wordlist", required_argument, nullptr, 0},
			{"userlist", required_argument, nullptr, 0},
			{"partition", no_argument, nullptr, 'P'},
			{"use-words", no_argument, nullptr, 'W'},
			{"change-passwd", no_argument, nullptr, 'C'},
			{nullptr, 0, nullptr, 0}
		};

		// CHECK THE COMMAND LINE ARGUMENTS AND CHANGE THE VARIABLES ACCRODINGLY. IF NONE ARE GIVEN, DEFAULT VALUES WILL BE USED
		int opt, option_index = 0;
		while ((opt = getopt_long(argc, argv, "n:l:o:d:WNLUSCPh", long_options, &option_index)) != -1)
		{
			// PARSE LONG OPTIONS THAT REQUIRE AN ARGUMENT AND DON'T HAVE A SHORT FORM EQUIVILENT
			if ((opt == 0) and (optarg != nullptr))
			{
				if (strcmp(long_options[option_index].name, "userlist") == 0)
				{
					// GET FILE PATH, RESET NUM USERS TO 0, SET FIND USERS BOOLEAN TO TRUE, AND SET THE HOME DIRECTORY STRING.
          // THEN, CHECK IF THE USER HAS ADDED A TILDA IN THE FILE PATH. IF SO, EXPAND IT TO THE HOME DIRECTORY OF THE USER
					userListPath = optarg;
					numUsers = 0;
					findUsers = true;
          string homeDir = getenv("HOME");
          if (userListPath[0] == '~'){userListPath = homeDir + userListPath.substr(1);}
					
					// OPEN USER LIST FILE FOR READING
					ifstream readfile(userListPath);
					string filelines;
					
					// CHECK IF THE FILE CAN BE OPENED. IF NOT, RAISE AN ERROR
					if (not(readfile))
					{
						errno = 2;
						cerr << _COL_RED << "ERROR" << _COL_OFF << " : User list file not found or cannot be read." << endl;
						cerr << "passgen : " << strerror(errno) << endl;
						return errno;
					}
					
					// READ USERS INTO USER LIST VECTOR
					while (getline (readfile, filelines))
					{
						userList.push_back(filelines);
						numUsers++;
					}
					
					// CLOSE USER LIST FILE
					readfile.close();
				}
				else if (strcmp(long_options[option_index].name, "wordlist") == 0)
				{
          // SET FIND WORDS TO TRUE, SET THE HOME DIRECTORY STRING, AND GET FILE PATH. THEN,
          // CHECK IF THE USER HAS ADDED A TILDA IN THE FILE PATH. IF SO, EXPAND IT TO THE HOME DIRECTORY OF THE USER
					wordListPath = optarg;
          findWords = true;
          string homeDir = getenv("HOME");
          if (wordListPath[0] == '~'){wordListPath = homeDir + wordListPath.substr(1);}

					// OPEN WORDLIST FILE FOR READING
					ifstream readfile(wordListPath);
					string filelines;

					// CHECK IF FILE CAN BE OPENED. IF NOT, RAISE AN ERROR
					if (not(readfile))
					{
						errno = 2;
						cerr << _COL_RED << "ERROR" << _COL_OFF << " : User list file not found or cannot be read." << endl;
						cerr << "passgen : " << strerror(errno) << endl;
						return errno;
					}

					// READ WORDS INTO WORD LIST VECTOR
					while (getline (readfile, filelines))
					{
						wordList.push_back(filelines);
					}

					// CLOSE WORD LIST FILE
					readfile.close();
				}
				else
				{
					errno = 1;
					cerr << _COL_RED << "ERROR" << _COL_OFF << " : Illegal argument." << optarg << endl;
					PrintUsage(errno);
					return errno;
				}
			}
			else
			{
				// CONVERT OPT TO AN ASCII CHARACTER
				char charOpt = static_cast <char> (opt);

				cout << "Option character: " << charOpt << endl; // DEBUG

				// PARSE SHORT OPTIONS. LONG OPTIONS THAT HAVE CORRESPONDING SHORT OPTIONS WILL RETURN THE SAME RESULT
				switch (charOpt)
				{
					case 'n':
						numUsers = atoi(optarg);
						cout << numUsers << endl;
						break;
					case 'l':
						passwordLength = atoi(optarg);
						cout << passwordLength << endl;
						break;
					case 'o':
						fileName = optarg;
						cout << fileName << endl;
						break;
					case 'd':
						defaultPath = optarg;
						cout << defaultPath << endl;
						break;
					case 'h':
						PrintUsage();
						return 0;
						break;
					case 'N':
						hasNumbers = false;
						break;
					case 'L':
						hasLowers = false;
						break;
					case 'U':
						hasUppers = false;
						break;
					case 'S':
						hasSpecChars = false;
						break;
					case 'P':
						partitionFile = true;
						break;
					case 'W':
						useWords = true;
						break;
					case 'C':
						changePasswd = true;
						break;
					case '?':
					default:
						errno = 1;
						cerr << _COL_RED << "ERROR" << _COL_OFF << " : Illegal argument." << endl;
						PrintUsage(errno);
						return errno;
						break;

				} // END OF SWITCH STATEMNT
			} // END OF IF-ELSE STATEMENT
		} // END OF ARGUMENT PARSING

		// CHECK IF THERE ARE ANY ARGUMENTS IN ARGV THAT ARE NOT ARGUMENTS OR OPTIONS. IF SO, RAISE AN ERROR
		if (optind < argc)
		{
				errno = 1;
				cerr << _COL_RED << "ERROR" << _COL_OFF << " : An option that is not an argument has not been found." << endl;
				PrintUsage(errno);
				return errno;

		}

		// CHECK LENGTH OF USERNAMES IF THE USER PROVIDES A USER LIST FILE. IF IT IS TOO LONG, RAISE AN ERROR
		for (const string& username : userList)
		{
			if (((username.length() > _MAX_USERNAME_LEN) or (username.length() < _MIN_USERNAME_LEN)) and not(username.length() == 0))
			{
				errno = 1;
				cerr << _COL_RED << "ERROR" << _COL_OFF << " : Username length of user " << username << " outside of allowed range." << endl;
				PrintUsage(errno);
				return errno;
			}
		}

		// CHECK IF THE USER ENTERED TOO LITTLE USERS AS AN ARGUMENT, OR TOO MANY. IF SO, RAISE AN ERROR
		if ((numUsers < _MIN_USERS) or (numUsers > _MAX_USERS))
		{
			errno = 1;
			cerr << _COL_RED << "ERROR" << _COL_OFF << " : Number of users outside of allowed range." << endl;
			PrintUsage(errno);
			return errno;
		}

		// CHECK IF THE USER ENTERED A PASSWORD LENGTH TOO LARGE AS AN ARGUMENT, OR TOO SMALL. IF SO, RAISE AN ERROR
		if ((passwordLength < _MIN_PASSWD_LEN) or (passwordLength > _MAX_PASSWD_LEN))
		{
			errno = 1;
			cerr << _COL_RED << "ERROR" << _COL_OFF << " : Password length outside of allowed range." << endl;
			PrintUsage(errno);
			return errno;
		}

		// CHECK TO SEE IF THE USER ELIMINATED ALL PASSWORD OPTIONS TO GENERATE A PASSWORD. IF SO, RAISE AN ERROR
		if ((hasNumbers == false) and (hasLowers == false) and (hasUppers == false) and (hasSpecChars == false) and (useWords == false))
		{
			errno = 1;
			cerr << _COL_RED << "ERROR" << _COL_OFF << " : Unable to generate password with no options available." << endl;
			PrintUsage(errno);
			return errno;
		}
		
		// EXECUTE PROGRAM
		GetSystemUsers(numUsers, userList.data(), findUsers);
		GenPasswordChars(numUsers, passwordLength, passwordList.data(), hasNumbers, hasLowers, hasUppers, hasSpecChars, useWords, findWords);
		GenPasswordWords(numUsers, passwordList.data(), wordList.data(), useWords, findWords);
    for (unsigned short int i = 0; i < numUsers; i++)
    {
      cout << userList[i] << endl;
    }
		WriteToFIle(numUsers, userList.data(), passwordList.data(), fileName, defaultPath, partitionFile);
    for (unsigned short int i = 0; i < numUsers; i++)
    {
      cout << userList[i] << endl;
    }
		ChangeUserPasswds(numUsers, userList.data(), passwordList.data(), changePasswd);

	} // END OF ROOT USER CHECK
	return 0;
} // END OF MAIN

/*
 * FUNCTION NAME: PrintUsage()
 * 
 * OPTIONAL INPUT PARAMETERS:
 * 	errorNum: INTEGER TYPE VARIABLE
 *
 * DESCRIPTION:
 * 	PRINTS A HELP MESSAGE TO STANDARD OUTPUT BASED ON
 * 	THE ERROR NUMBER PASSED TO IT. IF NONE IS PASSED TO
 * 	IT, MEANING THAT THE USER SPECIFICALLY CHOSE TO SHOW
 * 	THE HELP OPTION, IT WILL SHOW THE LONG HELP MESSAGE
 * 	ALONG WITH THE DESCRIPTION OF OF EACH ARGUMENT AND THE
 * 	PROGRAM VERSION NUMBER WHICH IS A STRING CONSTANT.
 * 	IF THE USER GIVES INCORRECT ARGUMENTS, THIS FUNCTION
 * 	WILL ALSO BE CALLED WITH A NON-ZERO ERROR NUMBER PASSED
 * 	TO IT TO SHOW THE SHORT HELP MESSAGES TO REMIND THE USER
 * 	HOW TO USE THE PROGRAM, WHICH IS ALSO A STRING CONSTANT
 * 
 * PROGRAM EXECUTION STEPS:
 * 	1 - INITIALIZE HELP MESSAGES WHICH ARE STRING CONSTANTS
 *
 * 	2 - EVALUATE WHICH MESSAGE TO SHOW BASED ON THE ERROR NUMBER
 * 	    (IF GIVEN) PASSED TO IT
 *
 * 	3 - END FUNCTION
 *
 * RETURN VALUE: VOID
 */
void PrintUsage(unsigned short int errorNum)
{
	// LONG HELP MESSAGE
	const string usageMessage = "passgen v1.0.0\n"
		                          "Optional arguments:\n"
				                        "	-n NUM_USERS		Specify number of users to generate passwords for. (default: 1, max: 100)\n"
				                        "	-l PASSWORD_LENGTH	Specify password length for user(s). (default: 12, min: 8, max: 50)\n"
				                        "	-o FILENAME		Specify output filename. (default: passwords.txt)\n"
				                        "	-d DIR_PATH		Specify directory to place file(s). (default: root user home directory)\n"
				                        "	-h, --help		Show this help message.\n"
				                        "	-N, --exclude-nums	Exclude numbers from password(s).\n"
				                        "	-L, --exclude-lower	Exclude lowercase letters from password(s).\n"
				                        "	-U, --exclude-upper	Exclude uppercase letters from password(s).\n"
				                        "	-S, --exclude-chars	Exclude special characters from password(s).\n"
				                        "	-P, --partition 	Split file contents for each user.\n"
				                        "	-W, --use-words 	Specify to use words in passwords for ease.\n"
				                        "	-C, --change-passwd	Change password with the provided one.\n" 
				                        "	--wordlist FILE_PATH	Specify wordlist file for password(s).\n"
				                        "	--userlist FILE_PATH	Specify userlist file for passwords.\n";
	// SHORT HELP MESSAGE
	const string shortUsageMsg = "usage: passgen [-WNLUSCPh] [-n NUM_USERS] [-l PASSWORD_LENGTH] [-o FILENAME]\n"
		                           "               [-d DIR_PATH] [--exclude-nums] [--exclude-lower] [--exclude-upper]\n"
				                       "               [--exclude-chars] [--wordlist FILE_PATH] [--userlist FILE_PATH]\n"
				                       "               [--change-passwd] [--use-words]\n";

	// PRINT HELP MESSAGE TO STDOUT.
	// IF ITS PRINTED AS A RESULT OF AN ERROR, PRINT THE SHORT ONE.
	// OTHERWISE, PRINT THE LONG HELP MESSAGE
	if (errorNum == 0)
	{
		cout << usageMessage;
	}
	else
	{
		cerr << shortUsageMsg;
	}
} // END OF PRINT USAGE FUNCTION

/*
 * FUNCTION NAME: GetSystemUsers()
 * 
 * OPTIONAL INPUT PARAMETERS:
 * 	numUsers: UNSIGNED SHORT INTEGER TYPE VARIABLE
 * 	userList: STRING ARRAY TYPE VARIABLE
 * 	findUsers: BOOLEAN TYPE VARIABLE
 *
 * DESCRIPTION:
 * 	GETS THE USERNAMES OF USERS TO GENERATE PASSWORDS FOR
 * 
 * PROGRAM EXECUTION STEPS:
 * 	1 - USE IF STATEMENT GUARD TO EVALUATE IF NUM USERS IS 0
 * 	    OR IF USER LIST IS STILL A NULL POINTER TO CONTINUE
 *
 * 	2 - EVALUATE IF THE USER HAS PROVIDED A USER LIST FILE
 * 	    OR NOT. IF SO, EXIT THE FUNCTION. IF NOT, CONTINUE
 *
 * 	3 - PROMPT THE USER TO ENTER A USERNAME FOR EACH USER
 * 	    AND TAKE INPUT UNTIL A NEWLINE IS ENCOUNTERED
 *
 * 	4 - END FUNCTION
 *
 * RETURN VALUE:
 * 	Integer 0 - Program success
 */
int GetSystemUsers(unsigned short int numUsers, string* userList, bool findUsers)
{
	// IF STATEMENT GUARD
	if ((numUsers != 0) and (userList != nullptr))
	{
		if (findUsers == true)
		{
      cout << "Using userlist file..." << endl;
			return 0;
		}
		else
		{
			for (unsigned short int i = 0; i < numUsers; i++)
			{
				const string usernamePrompt = "Enter the username of user ";
				cout << usernamePrompt << i + 1 << ": ";
				while ((getline(cin, userList[i]) and userList[i].length() < 32) and not(userList[i].empty())) {break;}

        //DEBUG
        if (userList[i].empty() == true)
        {
          cout << "ERROR PASSWORD GEN CHARS" << endl;
          exit(1);
        }
			}
		}

	} // END OF IF STATEMENT GUARD
	
	return 0;

} // END OF GET SYSTEM USERS FUNCTION

/*
 * FUNCTION NAME: GenPasswordChars()
 * 
 * OPTIONAL INPUT PARAMETERS:
 * 	numUsers: UNSIGNED SHORT INTEGER TYPE VARIABLE
 * 	passwordLength: UNSIGNED SHORT INTEGER TYPE VARIABLE
 * 	userList: STRING ARRAY TYPE VARIABLE
 * 	passwordList: STRING ARRAY TYPE VARIABLE
 * 	hasNumbers: BOOLEAN TYPE VARIABLE
 * 	hasLowers: BOOLEAN TYPE VARIABLE
 * 	hasUppers: BOOLEAN TYPE VARIABLE
 * 	hasSpecChars: BOOLEAN TYPE VARIABLE
 * 	useWords: BOOLEAN TYPE VARIABLE
 * 	findWords: BOOLEAN TYPE VARIABLE
 *
 * DESCRIPTION:
 * 	GENERATES CHARACTER PASSWORDS FOR EACH USER AND PUTS THEM IN A LIST
 * 
 * PROGRAM EXECUTION STEPS:
 * 	1 - USE IF STATEMENT GUARD TO EVALUATE IF useWords IS FALSE
 * 	    AND IF findWords FALSE. IF SO, CONTINUE
 *
 * 	2 - INITIALIZE VARIABLES FOR RANDOM GENERATION AND INITIALIZE
 * 	    THE PASSWORD CHARSETS INCLUDING NUMBERS, LOWERCASE LETTERS
 * 	    UPPERCASE LETTERS, AND SPECIAL CHARACTERS
 *
 * 	3 - PRINT GENERATING PASSWORD STATUS TO STDOUT
 *
 * 	4 - INITIALIZE OUTER FOR LOOP FOR NUMBER OF USERS AND INNER
 * 	    FOR LOOP FOR PASSWORD GENERATION
 *
 * 	5 - INITIALIZE PASSWORD BUFFER STRING IN OUTER FOR LOOP
 *
 * 	6 - INITIALIZE RANDOM CHARSET AND CHARSET CHARACTER LENGTH,
 * 	    THEN EVALUTATE IF THE BOOLEAN IS TRUE FOR EACH CHARACTER
 * 	    TYPE AND IF THE CORRESPONDING CHARSET MATCHES IN THE INNER
 * 	    FOR LOOP, THEN ADD THAT CHAR TO THR PASSWORD BUFFER
 *
 * 	7 - PLACE THE GENERATED PASSWORD FROM THE BUFFER TO THE PASSWORD LIST
 *
 * 	8 - REPEAT 6 AND 7 FOR EACH USER
 *
 * 	9 - END FUNCTION
 *
 * RETURN VALUE: VOID
 */
void GenPasswordChars(unsigned short int numUsers, unsigned short int passwordLength, string* passwordList, bool hasNumbers, bool hasLowers, bool hasUppers, bool hasSpecChars, bool useWords, bool findWords)
{
	// IF STATEMENT GUARD
	if ((useWords == false) and (findWords == false))
	{
		random_device seed;
		mt19937 gen(seed());
		vector <string> passwdCharsets = {"0123456789", "abcdefghijklmnopqrstuvwxyz", "ABCDEFGHIJKLMNOPQRSTUVWXYZ", "!@#$&*+=?%%"};
		uniform_int_distribution <int> dis(0, 3);

		cout << "Generating passwords..." << endl;

		// OUTER LOOP FOR USERS
		for (unsigned short int i = 0; i < numUsers; i++)
		{
      // PASSWORD BUFFER
      string passwordBuffer;
			
			// INNER LOOP FOR PASSWORDS
			for (unsigned short int j = 0; j < passwordLength;)
			{
        // GENERATE A NUMBER TO PICK THE CHARSET AND GET THE LENGTH OF THE CHARSET
				unsigned short int charSet = dis(gen);
				unsigned short int charSetLen = passwdCharsets[charSet].length();
        
        // GENERATE A RANDOM CHAR FROM THE CHARSET AND INCREMENT THE INNER LOOP
        // ONLY WHEN BOOLEANS ARE TRUE AND MATCH THEIR CHARSETS
				if ((hasNumbers and charSet == 0) or (hasLowers and charSet == 1) or 
				(hasUppers and charSet == 2) or (hasSpecChars and charSet == 3))
				{
					uniform_int_distribution <int> dis(1, charSetLen - 1);
					passwordBuffer.push_back(passwdCharsets[charSet][dis(gen)]);
          j++;
				}
			}

      // PUSH CONTENTS OF PASSWORD BUFFER TO PASSWORD LIST
      passwordList[i] = passwordBuffer;

		} // END OF OUTER FOR LOOP
	} // END OF IF STATEMENT GUARD
} // END OF GEN PASSWORD CHARS FUNCTION

// GENERATE PASSWORDS WITH WORDS
void GenPasswordWords(unsigned short int numUsers, string* passwordList, string* wordList, bool useWords, bool findWords)
{
	// IF STATEMENT GUARD
	if ((useWords == true) and (findWords == false))
	{
    // PRINT STATUS TO STDOUT
    cout << "Generating passwords..." << endl;

    // DEFINE WORD BANK FOR PASSWORD GENERATION
    vector <string> wordBank = {"recession", "deviation", "labortory", "proportion", "chimpanzee",
                             "secretary", "litigation", "destruction", "therapist", "conglomerate",
                             "development", "possibility", "requirement" ,"conservative", "character",
                             "deficiency", "preparation", "attachment", "horoscope", "manufacture",
                             "chauvinist", "separation", "combination", "unpleasent", "infection",
                             "psychology", "progressive", "inspector", "anniversary", "satisfied",
                             "reference", "recommend", "orchestra", "negotiation", "attractive",
                             "insurance", "concentrate", "tournament", "hospitality", "constitution", 
                             "fabricate", "acquisition", "acceptable", "cooperate", "compromise",
                             "beginning", "conventional", "sculpture", "community", "distribute",
                             "astonishing", "multimedia", "suffering", "intergrated", "legislation",
                             "embarrassment", "brainstorm", "circulation", "knowledge", "representative",
                             "essential", "interface", "preparation", "publicity", "hemisphere",
                             "rebellion", "performance", "attachment", "disappoint", "conscious",
                             "basketball", "authorise", "disipline", "production", "technology",
                             "ostracize", "calculation", "achievement", "neighbour", "biography",
                             "overcharge", "administration", "agreement", "observation", "nomination",
                             "assignment", "conclusion", "dictionary", "exhibition", "authority",
                             "strategic", "precedent", "correspond", "conductor", "landscape",
                             "innovation", "coincidence", "community", "unfortunate"};
      random_device seed;
      mt19937 gen(seed());
      uniform_int_distribution <int> dis(0, wordBank.size() - 1);

    // GENERATE PASSWORDS FROM WORDBANK
    for (unsigned short int i = 0; i < numUsers; i++)
    {
      string firstWord = wordBank[dis(gen)];
      string secondWord = wordBank[dis(gen)];
      string lastWord = wordBank[dis(gen)];
      string fullPassword = firstWord + "-" + secondWord + "-" + lastWord;

      passwordList[i] = fullPassword;
    }
	}
  else if ((useWords == true) and (findWords == true))
  {
    // PRINT STATUS TO STDOUT
    cout << "Generating passwords with wordlist..." << endl;

    random_device seed;
    mt19937 gen(seed());
    uniform_int_distribution <int> dis(0, wordList->size() - 1);
    
    // GENERATE PASSWORDS FROM USER PROVIDED WORDLIST
    for (unsigned short int i = 0; i < numUsers; i++)
    {
      string firstWord = wordList[dis(gen)];
      string secondWord = wordList[dis(gen)];
      string lastWord = wordList[dis(gen)];
      string fullPassword = firstWord + "-" + secondWord + "-" + lastWord;

      passwordList[i] = fullPassword;
    } // END OF FOR LOOP
  }// END OF IF STATEMENT GUARD
} // END OF GEN PASSWORD WORDS FUNCTION

// WRITE CONTENT TO A FILE
void WriteToFIle(unsigned short int numUsers, string* userList, string* passwordList, string filename, string defaultPath, bool partitionFile)
{
	// IF STATEMENT GUARD
	if ((userList != nullptr) and (passwordList != nullptr) and (filename.empty() == false) and (defaultPath.empty() == false))
	{
    // GET FILE PATH FROM DIRECTORY AND FILENAME
    string homeDir = getenv("HOME");
    if (defaultPath[0] == '~'){ defaultPath = homeDir + defaultPath.substr(1);}
    string filePath = defaultPath + "/" + filename;

    // WRITE CONTENT TO FILES
    if (partitionFile == false)
    {
      cout << "Writing to file " << filePath << "..." << endl;

      ofstream writefile(filePath);

      writefile << "\tKEEP THIS FILE FROM UNAUTHORIZED USERS" << endl;
      writefile << "\t\tGENERATED BY PASSGEN" << endl << endl;

      for (unsigned short int i = 0; i < numUsers; i++)
      {
        if (userList[i].empty() == true)
        {
          continue;
        }
        writefile << "The password of user " << userList[i] << " is " << passwordList[i] << endl;
      }

      writefile.close();
    }
    else
    {
      cout << "Writing to and partitioning files in directory " << defaultPath << endl;
    }

	} // END OF IF STATEMENT GUARD
} // END OF WRITE TO FILE FUNCTION

// CHANGE THE PASSWORDS OF USERS
void ChangeUserPasswds(unsigned short int numUsers, string* userList, string* passwordList, bool changePasswd)
{
	// IF STATEMENT GUARD
	if ((userList != nullptr) and (passwordList != nullptr) and (changePasswd == true))
	{
		cout << "Picked change passwords" << endl;

	} // END OF IF STATEMENT GUARD
} // END OF CHANGE USER PASSWORDS FUNCTION
