/*
Author: Gauri Gupta
CID: 01258518
Date: 22.11.2017
University: Imperial College London
Program name: DNA Sequence Database Software
*/

#include <iostream> //Includes input output stream
#include <fstream>	//Includes file stream
#include <sstream>	//Includes string stream
#include <vector>	//Enables usage of vectors
#include <string>	//Enables usage of string
#include <cstdlib>	//Enables quitting the program with exit(0); function

using namespace std;

class DNA_DB
{

private:							//All member functions of this class have access to the variables declared in private 
	vector<string> filenames;
	string filename;
	string file;
	int file_num;
	char option;
	char c;
	string file_contents;
	string summary_info;
	vector<string> summaryinfo;
	ifstream ist;
	char option2;
	vector<char> character;
	vector<char> character2;


public:								//All the member functions of the class are declared here
	void load_files();
	void show_menu();
	void show_menu2();
	void prompt1();
	int detailed_summary(int file_to_be_opened);
	int body_of_file(int file_to_be_opened);
    ~DNA_DB();
};

void DNA_DB::prompt1()				//Member function that outputs the prompt asking for the user to insert the file(s)

{
    cout << "Specify the name of DNA sequence file names you would like to load (maximum of 9). For multiple files, add a ',' between each file name." << endl;
    cout << ">";
    cin >> filename;				//The user's input is put into the string filename

}

int DNA_DB::detailed_summary( int file_to_be_opened)

{
            ifstream ist(filenames[file_to_be_opened].c_str());
            getline(ist, file_contents);									//So we don#t have to deal with the characters in the first line
            istringstream stream(file_contents);

            int N = 0;
            int G = 0;
            int A = 0;
            int T = 0;
            int C = 0;
            int R = 0;
            int Y = 0;
            int M = 0;
            int K = 0;
            int S = 0;
            int W = 0;
            int H = 0;
            int B = 0;
            int V = 0;
            int D = 0;
            int unknown = 0;
            int N_region = 0;
            int C_region = 0;


            while (!ist.eof()) {
                ist.get(c);
                if (c!='\n')
                    character.push_back(c);
                if (c=='N')
                    N++;
                if (c=='G')
                    G++;
                if (c=='A')
                    A++;
                if (c=='T')
                    T++;
                if (c=='C')
                    C++;
                if (c=='R')
                    R++;
                if (c=='Y')
                    Y++;
                if (c=='M')
                    M++;
                if (c=='K')
                    K++;
                if (c=='S')
                    S++;
                if (c=='W')
                    W++;
                if (c=='H')
                    H++;
                if (c=='B')
                    B++;
                if (c=='V')
                    V++;
                if (c=='D')
                    D++;
                if (c==' ')
                    unknown++;
            }

            for (int i=0; i<character.size(); i++) {

                if ((character[i] == 'N') && (character[i+1] != 'N')) {
                    N_region++;
                }
                if ((character[i] != 'N') && (character[i+1] == 'N')) {
                    C_region++;
                }
            }

            if ((character[0] != 'N') && (N_region == 0) && (C_region == 0)) {						//In case there are only coded regions or only gap regions
                C_region++;
            }
            else if ((character[0] == 'N') && (N_region == 0) && (C_region == 0)) {
                N_region++;
            }

            cout << "\n\n Region Characteristics: " << endl;
            cout <<"# Regions:   " << N_region+C_region << endl;
            cout <<"# N Regions: " << N_region << endl;
            cout <<"# C Regions: " << C_region << endl;

            cout << "\nBase pair characteristics: " << endl;
            cout << "# base pairs:" << G+A+T+C+R+Y+M+K+S+W+H+B+V+D+N << endl;
            cout << "G:           "<< G << endl;
            cout << "A:           "<< A << endl;
            cout << "T:           "<< T << endl;
            cout << "C:           "<< C << endl;
            cout << "R:           "<< R << endl;
            cout << "Y:           "<< Y << endl;
            cout << "M:           "<< M << endl;
            cout << "K:           "<< K << endl;
            cout << "S:           "<< S << endl;
            cout << "W:           "<< W << endl;
            cout << "H:           "<< H << endl;
            cout << "B:           "<< B << endl;
            cout << "V:           "<< V << endl;
            cout << "D:           "<< D << endl;
            cout << "N:           "<< N << endl;
            cout << "Unknown:     "<< unknown << endl;
            show_menu2();
}

int DNA_DB::body_of_file(int file_to_be_opened)							//Function that opens the appropriate file from the vector filecontents according to the integer passed into it 
{
    ifstream ist(filenames[file_to_be_opened].c_str());
    getline(ist, file_contents);										//Gets rid of the first line
    while (!ist.eof()) {												//Populates a vector of type char from the characters of the body of the file						
        ist.get(c);
        if (c!='\n')
            character.push_back(c);
    }

}

DNA_DB::~DNA_DB()					//The destructor 
{

}

void DNA_DB::load_files()			//Member function for loading files
{
	cout << "DNA Sequence Database Software \n" << endl;		
	
	string fa(".fa");				//Creating a string named fa with contents .fa to be used later on to check whether file format is correct
	
    int valid_filenames = 0;		//To be used in condition of while loop

    while (valid_filenames==0) {

        prompt1();					//Calls prompt1 function

        if (filename.find(',') != std::string::npos) {  //Using .find function to determine whether the delimiter is in the string filename inputed by the user or not

            istringstream stream(filename);				//Converts the string filename inputed by the user into a stream
            int file_num=0;

            while( getline(stream, file, ',')) {		
                filenames.push_back(file);				//Adds substrings (divided by ',') into the vector filenames of type string from the string filename. Now filenames is a vector of string elements that are the filenames given by the user.
                file_num++;								//Counts the number of filenames given
            }

            for (int i=0;i<file_num;i++) {				//for iterating through the contents of the vector filenames

                ifstream ist(filenames[i].c_str());		//opening the file
                cout << "\nLoading " << filenames[i] << "..." <<endl;
				
				if (filenames[i].find(fa) == std::string::npos) {		//Checking whether the file name has .fa 
					cout << "Failed to load " << filenames[i] <<endl;
					cout << "\nEnter correct file format for " << filenames[i] << endl;
					cout << "\n";
					valid_filenames=0;									//Making sure the condition to stay in the loop is still valid
                    filenames.clear();									//Clearing the vector filenames's contents 
                    break;												//Going back to the beginning of the loop											
				}

                else if (!ist)  {										//checking if the file is not open
                    cout << "Failed to load " << filenames[i] <<endl;
                    cout << "\nPlease insert correct filenames. \n" <<endl;
                    valid_filenames=0;
                    filenames.clear();
                    break;
                }

                else {
                    cout << "Successful loading of " << filenames[i] << "\n" <<endl;
                    valid_filenames++;									//the condition to stay in the loop is not valid so we exit the while loop
                }
            }
        }

        else {															//For the case when only one file is inputed by the user

            ifstream ist(filename.c_str());
            cout << "\nLoading " << filename << "..." <<endl;
			
				if (filename.find(fa) == std::string::npos) {
					cout << "Failed to load " << filename <<endl;
					cout << "\nEnter correct file format" << endl;
					cout << "\n";
				}

                else if (!ist)  {
                    cout << "Failed to load " << filename <<endl;
                    cout << "\nPlease insert correct filename. \n" << endl;
                }

                else {
                    cout << "Successful loading of " << filename <<endl;
                    valid_filenames++;
                }
        }
    }
}

void DNA_DB::show_menu()
{

	cout << "\n\nSelect one of the following options:" << endl;
	cout << "(S) Summary statistics of the DNA database" << endl;

	if (filename.find(',') != std::string::npos) {

		for (int i=0; i<filenames.size(); i++) {
			cout << "(" << i+1 << ")" << " Analyse " << filenames[i] << endl;   //Displaying the menu1 options is accordance to the number of files uploaded
		}
	}

	else {
		cout << "(1) Analyse " << filename << endl;								//For the case when only onw file is uploaded
	}

	cout << "(Q) Quit" << endl;													

	int valid_option = 0;														//Condition introduced to check whether the option chosen is valid or not 

	while (valid_option == 0) {

        cout << "\n>";
        cin >> option;
        int intop = option - '0';												//Converts the option of type char inserted by the user into an integer (only works from 0-9)
        if ((option == 'S') || (option == 'Q') ||((intop>0) && (intop<=filenames.size())) || (intop == 1))		//Conditions that check if option is valid
            valid_option++;														//Exit while loop

        else {
            cout << "Invalid option. Please insert valid option." << endl;
            valid_option = 0;													//Stay in loop because option was invalid
        }
	}

	if (option == 'S') {

		if (filename.find(',') != std::string::npos) {

			cout << "\n\nThe number of DNA sequences (files) loaded are: " << filenames.size() << "\n" << endl;

			for (int i=0; i<filenames.size(); i++) {

                ifstream ist(filenames[i].c_str());
                getline(ist, file_contents);				//Get the first line of the file and put it into the string file_contents 
                istringstream stream(file_contents);		

                while( getline(stream, summary_info, '|')) { //Delimiter is | for the first line of the file to get the summary info into substrings
                    summaryinfo.push_back(summary_info);
                    }

                cout << "\nSequence identifiers: \n" << endl;		//Displaying substrings of the first line appropriately (assuming the pattern is the same in each file)
                cout << "Name: " << summaryinfo[5*i+4] << endl;	
                cout << "GID: " << summaryinfo[5*i+1] << endl;
                cout << "REF: " << summaryinfo[5*i+3] << endl;
			}
			show_menu();
		}
		else  {

			cout << "\n\nThe number of DNA sequences (file) loaded is one. \n" << endl;

			ifstream ist(filename.c_str());
            getline(ist, file_contents);
            istringstream stream(file_contents);

            while( getline(stream, summary_info, '|')) {
                summaryinfo.push_back(summary_info);
                }

            cout << "Sequence identifiers: \n" << endl;
            cout << "Name: " << summaryinfo[4] << endl;
            cout << "GID: " << summaryinfo[1] << endl;
            cout << "REF: " << summaryinfo[3] << endl;
            show_menu();
		}
	}

	else if (option == 'Q') {
        exit(0);
	}

	else
        show_menu2();
}

void DNA_DB::show_menu2()
{
    cout << "\n\nSelect one of the following options:" << endl;
    cout << "(H) Help" << endl;
    cout << "(S) Summary statistics of the DNA sequence" << endl;
    cout << "(1) Analyse gap region" << endl;
    cout << "(2) Analyse coded region" << endl;
    cout << "(3) Analyse base pair range" << endl;
    cout << "(4) Find DNA sequence by manual input" << endl;
    cout << "(5) Find DNA sequence by file input" << endl;
    cout << "(R) Return to the previous menu" << endl;
    cout << "(Q) Quit" << endl;
    cout << ">";
    cin >> option2;

    if (option2 == 'H') {
        cout << "\n\nCharacter Specification " << endl;
        cout << "Code Base Description " << endl;
        cout << "G Guanine" << endl;
        cout << "A Adenine" << endl;
        cout << "T Thymine (Uracil in RNA)" << endl;
        cout << "C Cytosine" << endl;
        cout << "R Purine (A or G)" << endl;
        cout << "Y Pyramidine (C or T or U)" << endl;
        cout << "M Amino (A or C)" << endl;
        cout << "K Ketone (G or T) " << endl;
        cout << "S Strong interaction (C or G) " << endl;
        cout << "W Weak interaction (A or T) " << endl;
        cout << "H Not-G(A or C or T) H follows G in the alphabet" << endl;
        cout << "B Not-A(C or G or T) B follows A in the alphabet" << endl;
        cout << "V Not-T(not-U)(A or C or G) V follows U in the alphabet" << endl;
        cout << "D Not-C(A or G or T)D follows C in the alphabet" << endl;
        cout << "N Any(A or C or G or T) \n" << endl;
        show_menu2();
    }

    else if (option2 == 'S') {


        if (filename.find(',') != std::string::npos) {

            int intop = option -'0';

            ifstream ist(filenames[intop-1].c_str());
            getline(ist, file_contents);
            istringstream stream(file_contents);

            while( getline(stream, summary_info, '|')) {
                summaryinfo.push_back(summary_info);
            }

            cout << "\n\nSequence identifiers: " << endl;
            cout << "Name: " << summaryinfo[4] << endl;
            cout << "GID: " << summaryinfo[1] << endl;
            cout << "REF: " << summaryinfo[3] << endl;

            detailed_summary(intop-1); 		//Passing the correct integer to the function

        }
        else {

            filenames.push_back(filename);	//One file is inserted so we have a vector with just one element (the 0th element) to pass it into the function that expects integer arguments

            ifstream ist(filename.c_str());
            getline(ist, file_contents);
            istringstream stream(file_contents);

            while( getline(stream, summary_info, '|')) {
                summaryinfo.push_back(summary_info);
            }
            cout << "\n\nSequence identifiers: " << endl;
            cout << "Name: " << summaryinfo[4] << endl;
            cout << "GID: " << summaryinfo[1] << endl;
            cout << "REF: " << summaryinfo[3] << endl;

            detailed_summary(0);

        }
    }

    else if (option2 == '1') {

        int n;
        cout << "\n\nPlease insert the number for the nth gap region" << endl;
        cout << ">";
        cin >> n;
        int N_region = 0;

        if (filename.find(',') != std::string::npos) {

            int intop = option -'0';

            body_of_file(intop-1);					

            cout << "\nSelected Sequence:" << endl;
            cout << "Gap region number: " << n <<endl;

            for (int i=0; i<character.size(); i++) {
                int y = i;
                int x = 0;
                    if ((character[i] == 'N') && (character[i+1] != 'N')) {
                        N_region++;
                        if (N_region==n) {

                            cout << "\nSequence: " << endl;
                            while (character[y] == 'N') {
                                cout << character[y];							//Essentiall going backwards from the point a gap reagion ended to the point where there is again no N
                                y--;
                                x++;
                            }
                            cout << "\n\nBase Pair Range: (" << y+2 << ",";		//Adjusting the range to account for the fact that array starts with the element 0 and that y is still decremented by 1 at the end of the loop
                            cout << y+x+1;
                            cout <<")" << endl;
                        }
                    }
            }
            show_menu2();
        }
        else {

            filenames.push_back(filename);

            body_of_file(0);

            cout << "\nSelected Sequence:" << endl;
            cout << "Gap region number: " << n <<endl;

            for (int i=0; i<character.size(); i++) {
                int y = i;
                int x = 0;
                    if ((character[i] == 'N') && (character[i+1] != 'N')) {  
                        N_region++;
                        if (N_region==n) {

                            cout << "\nSequence: " << endl;
                            while (character[y] == 'N') {
                                cout << character[y];
                                y--;
                                x++;
                            }
                            cout << "\n\nBase Pair Range: (" << y+2 << ",";
                            cout << y+x+1;
                            cout <<")" << endl;
                        }
                    }
            }
            show_menu2();
        }
    }

    else if (option2=='2') {
        int n;
        cout << "\nPlease insert the number for the nth coded region" << endl;
        cout << ">";
        cin >> n;
        int C_region = 0;

        if (filename.find(',') != std::string::npos) {

            int intop = option -'0';

            body_of_file(intop-1);

            cout << "\nSelected Sequence:" << endl;
            cout << "Coded region number: " << n <<endl;

            for (int i=0; i<character.size(); i++) {
                int y = i;
                int x = 0;
                    if ((character[i] != 'N') && (character[i+1] == 'N')) {					//opposite of the gap region 
                        C_region++;
                        if (C_region==n) {
                            cout << "\nSequence: " << endl;
                            while (character[y] != 'N') {
                                cout << character[y];
                                y--;
                                x++;
                            }
                            cout << "\n\nBase Pair Range: (" << y+2 << ",";
                            cout << y+x+1;
                            cout <<")" << endl;
                        }
                    }

                }

            if ((character[0] != 'N') && (C_region == 0)) {									//in case there is only coded region and no gap region like in gnFOXF1.fa
                for (int i=0; i<character.size(); i++) {
                    cout << character[i];

                    }

                }
            show_menu2();

        }
        else {

            filenames.push_back(filename);

            body_of_file(0);

            cout << "\nSelected Sequence:" << endl;
            cout << "Coded region number: " << n <<endl;

            for (int i=0; i<character.size(); i++) {
                int y = i;
                int x = 0;
                    if ((character[i] != 'N') && (character[i+1] == 'N')) {
                        C_region++;
                        if (C_region==n) {
                            cout << "\nSequence: " << endl;
                            while (character[y] != 'N') {
                                cout << character[y];
                                y--;
                                x++;
                            }
                            cout << "\n\nBase Pair Range: (" << y+2 << ",";
                            cout << y+x+1;
                            cout <<")" << endl;
                        }
                    }
                }

            if ((character[0] != 'N') && (C_region == 0)) {
                for (int i=0; i<character.size(); i++) {
                    cout << character[i];
                    }
                }

            show_menu2();
        }
    }

    else if (option2 == '3') {
        int m;
        int n;
        cout << "\nPlease provide the lower limit of the base pair range: " << endl;
        cout << ">";
        cin >> m;
        cout << "\nPlease provide the upper limit of the base pair range: " << endl;
        cout << ">";
        cin >> n;

        cout << "\nSelected sequence: " << endl;
        cout << "Base Pair Range: (" << m << ",";
        cout << n;
        cout << ")\n" << endl;

        if (filename.find(',') != std::string::npos) {

            int intop = option -'0';

            body_of_file(intop-1);

            cout << "Sequence: " << endl;
            for (int i=m; i<=n; i++) {
                cout << character[i-1]; //minus one because it starts at element 0
            }
            show_menu2();
        }
        else {

            filenames.push_back(filename);

            body_of_file(0);

            cout << "Sequence: " << endl;
            for (int i=m; i<=n; i++) {
                cout << character[i];
            }
            show_menu2();
        }
    }

    else if (option2 == '4') {         //NOTE: This option only works for 12 base pairs inputed by the user.
        string us; //us stands for user sequence 
        cout << "\n\nSpecify the DNA sequence of 12 nucleotides you would like to find: " << endl;
        cout << ">";
        cin >> us;
        int occurances = 0;
		
		character.clear(); //Clear the vector! Because it pushes back every time you populate it in the previous functions

        if (filename.find(',') != std::string::npos) {

            int intop = option -'0';

            body_of_file(intop-1);

            for (int i=0; i<(character.size()-11); i++) {  //-11 because the array is not defined for further elements in the loop when we iterate through it 
                if ((us[0] == character[i]) && (us[1] == character[i+1]) && (us[2] == character[i+2]) && (us[3] == character[i+3]) && (us[4] == character[i+4]) && (us[5] == character[i+5]) && (us[6] == character[i+6]) && (us[7] == character[i+7]) && (us[8] == character[i+8]) && (us[9] == character[i+9]) && (us[10] == character[i+10]) && (us[11] == character[i+11])) {
                    occurances++;
                    cout << "\n" << endl;
                    cout << occurances << ". Base Pair Range: (" << i+1 << ",";
                    cout << i+12;
                    cout << ")" << endl;
                    cout << "\nThe sequence of 20 nucleotides before the specified sequence:" << endl;
                    for (int t=1; t<21; t++) {
                        cout << character[i-t];
                    }
                    cout << "\nThe specified sequence:" << endl;
                    for (int t=0; t<12; t++) {
                        cout << us[t];
                    }
                    cout << "\nThe sequence of 20 nucleotides after the specified sequence:" << endl;
                    for (int t=1; t<21; t++) {
                        cout << character[i+11+t];
                    }
                }
            }
            show_menu2();
        }
        else {

            filenames.push_back(filename);

            body_of_file(0);

            for (int i=0; i<(character.size()-11); i++) {
                if ((us[0] == character[i]) && (us[1] == character[i+1]) && (us[2] == character[i+2]) && (us[3] == character[i+3]) && (us[4] == character[i+4]) && (us[5] == character[i+5]) && (us[6] == character[i+6]) && (us[7] == character[i+7]) && (us[8] == character[i+8]) && (us[9] == character[i+9]) && (us[10] == character[i+10]) && (us[11] == character[i+11])) {
                    occurances++;
                    cout << "\n" << endl;
                    cout << occurances << ". Base Pair Range: (" << i+1 << ",";
                    cout << i+12;
                    cout << ")" << endl;
                    cout << "\nThe sequence of 20 nucleotides before the specified sequence:" << endl;
                    for (int t=1; t<21; t++) {
                        cout << character[i-t];
                    }
                    cout << "\nThe specified sequence:" << endl;
                    for (int t=0; t<12; t++) {
                        cout << us[t];
                    }
                    cout << "\nThe sequence of 20 nucleotides after the specified sequence:" << endl;
                    for (int t=1; t<21; t++) {
                        cout << character[i+11+t];
                    }
                }
            }
            show_menu2();
        }
    }

    else if (option2 == '5') {   //NOTE: This option doesn't fully work yet, but the idea is esentially to convert the body of the file uploaded into a vector of characters and compare it to the body of the previous file uploaded
								 //NOTE: If I can find a way to loop through AND conditions (using two for loops instead of one most likely) in option 4 without plugging them in by brute force then I can also do this option, but I can't so this is all I have. 	
        int valid_filename = 0;
        while (valid_filename==0) {
            string filename2;
            cout << "\n\nSpecify the name of DNA sequence file name you would like to load." << endl;
            cout << ">";
            cin >> filename2;
            ifstream ist(filename2.c_str());
            cout << "\nLoading " << filename2 << "..." <<endl;

                if (!ist)  {
                    cout << "Failed to load " << filename2 <<endl;
                    cout << "\nPlease insert correct file name. \n" << endl;
                }

                else {
                    cout << "Successful loading of " << filename2 <<endl;
                    string header;
                    getline(ist, header);
                    char l;
                    while (!ist.eof()) {
                        ist.get(l);
                        if (l!='\n')
                            character2.push_back(l);
                    }
                    valid_filename++;
                }
        }
        int occurances = 0;

        if (filename.find(',') != std::string::npos) {

            int intop = option -'0';

            body_of_file(intop-1);

            show_menu2();
        }

        else {

            filenames.push_back(filename);

            body_of_file(0);
			
			show_menu2();

        }
        
    }


    else if (option2 == 'R')
        show_menu();

    else if (option2 == 'Q')
        exit(0);

    else {
        cout << "\nInvalid option";
        show_menu2();
    }
}

int main()
{
	DNA_DB dna_db;								//Creates an object of the type class
	dna_db.load_files();
	dna_db.show_menu();

return 0;

}



