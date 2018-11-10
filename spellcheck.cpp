#include <iostream>
#include <string>
#include "hash.h"
#include <istream>
#include <fstream>
#include <ctime>

using namespace std;


unsigned int averageSize = 50000;
hashTable dictionary(averageSize);


// Helper method to convert UPPERCASE to lowercase
void lowerCase(char &c){
    if((c >= 65) && (c <= 90)){ 
	c += 32; 
	}
}


// Helper method to check if the value is numeric
void checkNumeric(char &c, int &numFlag){
    if((c >= 48) && (c <= 57)){
	numFlag = 1; 
	}
}


// Loading dictionary into hash table
void loadDictionary(string &dictionaryName){
    
    ifstream dict;
    dict.open(dictionaryName, ios::in);
    
    char currentChar;
    string currentWord;
    
    while (!dict.eof()) {
            
        dict.get(currentChar);
        
        // Ignore new line
        if(currentChar == '\n'){
            continue;
        }
        
        // If upper case, convert to lower case.
         lowerCase(currentChar);
        
        currentWord += currentChar;
        
        // Insert word into hash table if newline/eof
        if  ((dict.peek() == '\n') || (dict.peek() == -1)){
            dictionary.insert(currentWord);
            currentWord.clear();
            dict.get(currentChar);
        }
    }
}

// Spellchecker
void spellCheck(string &in_file, string &out_file){

    // Open input file 
    ifstream inputFile;
    inputFile.open(in_file, ios::in);
    
    // Open output file
    ofstream outputFile;
    outputFile.open(out_file);
    
    
    char currentChar;
    string currentWord;
    
    // Flag that specifies if the current word has a digit in it
    int numFlag = 0;
    int lineNumber = 1;

    while (inputFile.peek() != -1) {
        
        inputFile.get(currentChar);
        
        // If upper case, convert to lower case.
         lowerCase(currentChar);

        // Check to see if current character is a digit & update numFlag
        checkNumeric(currentChar, numFlag);
        
        // Invalid character check
      	    if  (!(((currentChar == 39) || (currentChar == 45)) || ((currentChar >= 48) && (currentChar <= 57)) || ((currentChar >= 97) && (currentChar <= 122)))) {   
            int currentLine = lineNumber;
            if( currentChar == '\n' ){ lineNumber++; }
            
            // Numeric check
            if((numFlag == 1) || (currentWord.size() == 0)){
                currentWord.clear();
                numFlag = 0;
                continue;
            }
            
            // Long check
            if(currentWord.size() > 20){
                outputFile << "Long word at line " << currentLine << flush;
                outputFile<< ", starts: " << currentWord.substr(0, 20) << endl;
            }
            
            // Dictionary check
            else if(!dictionary.contains(currentWord)){
                outputFile << "Unknown word at line " << currentLine << ": " << flush;
                outputFile << currentWord << endl;
            }
            currentWord.clear();
            numFlag = 0;
        }
        
        // Otherwise add it to the current word
        else{ 
		currentWord += currentChar; 
	}    
    }
}


int main() {  
    
    // Request user to enter name of dictionary to be used
    string dictionaryName;
    cout << "Enter name of dictionary: " << flush;
    cin >> dictionaryName;
    
    // Load in dictionary
    clock_t s = clock();
    loadDictionary(dictionaryName);
    clock_t f = clock();
    double timeDiff = ((double)(f - s)) / CLOCKS_PER_SEC;
    
    cout << "Total time (in seconds) to load dictionary: " << timeDiff << endl;
    
    // Request user to enter names of input & output files
    string inputName;
    cout << "Enter name of input file: " << flush;
    cin >> inputName;
    
    string outputName;
    cout << "Enter name of output file: " << flush;
    cin >> outputName;
    
    // Spell check document & write to output file
    s = clock();
    spellCheck(inputName, outputName);
    f = clock();
    timeDiff = ((double)(f - s)) / CLOCKS_PER_SEC;
    
    cout<< "Total time (in seconds) to check document: "<< timeDiff << endl;

    return 0;
}

