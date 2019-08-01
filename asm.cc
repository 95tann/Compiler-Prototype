#include "kind.h"
#include "lexer.h"
#include <vector>
#include <string>
#include <iostream>
#include <cstdio>
#include <map>

// Use only the neeeded aspects of each namespace
using std::string;
using std::vector;
using std::map;
using std::endl;
using std::cerr;
using std::cin;
using std::cout;
using std::getline;
using ASM::Token;
using ASM::Lexer;

void map_print(map<string,int> label_map){
  map<string,int>::iterator mapit;
  for (mapit = label_map.begin(); mapit != label_map.end(); mapit++){
        int len = (mapit->first).length();
	string nocolon = (mapit->first).substr(0,len-1);
	cerr << nocolon << " " << mapit->second << endl;
  }
}

// Iterate over the lines of tokens and print them
// to standard error
void input_checker(vector< vector<Token*> > tokLines,map<string,int> &labelMap, int pass) {
    
    int pc = 0;
    int curr_token = 0;
    bool isword = 0;
    bool error = 0;    
    
    vector<vector<Token*> >::iterator it;
    for(it = tokLines.begin(); it != tokLines.end(); ++it){
      vector<Token*>::iterator it2;
      curr_token = 0;
      Token* prev_token = NULL;

      for(it2 = it->begin(); it2 != it->end(); ++it2){
        
	curr_token++;
	string kind = (*it2)->toString();
        string lexeme = (*it2)->getLexeme();

	if (kind == "DOTWORD"){
           if ((it->size() - curr_token) != 1){   //((curr_token) == it->size() || (it->size() > curr_token + 2)){
               if (pass == 1) error = 1;
               cerr << "ERROR: .word should have 1 arg" << endl;
           }
           else{ 
               isword = 1;
               pc += 4;
               prev_token = *it2;
           }
        }
	
	else if ((kind == "HEXINT" && error == 0)){
	   int  lex_int = (*it2)->toInt() ;
           isword = 0;
           putchar((lex_int >> 24) & 0xff);
           putchar((lex_int >> 16) & 0xff);
           putchar((lex_int >> 8) & 0xff);
           putchar((lex_int & 0xff));
           prev_token = *it2;    
	}
        else if ((kind == "INT" && error == 0)){
           int  lex_int = (*it2)->toInt() ;
           isword = 0;
           putchar((lex_int >> 24) & 0xff);
           putchar((lex_int >> 16) & 0xff);
           putchar((lex_int >> 8) & 0xff);
           putchar((lex_int & 0xff));
           prev_token = *it2;
	}
        else if ((kind == "LABEL") && (pass == 1)){
           if ((prev_token != NULL) && (prev_token->getKind() != ASM::LABEL)){ 
		cerr << "ERROR: invalid label" << endl;
                if (pass == 1) error = 1;
                break;
           }
	   if (labelMap.find(lexeme) == labelMap.end() && (pass == 1)) {
        	labelMap[lexeme] = pc;
                prev_token = *it2;
           }
           else if (pass == 1){ 
                error = 1;
                cerr << "ERROR: duplicate label" << endl;
           }
           isword = 0;
        }   
        else if (kind == "ID"){
             if (pass == 1){
                 //if (labelMap.find(lexeme+":") == labelMap.end()){
		 //    cerr << "ERROR: label " << lexeme << " not defined (pass 1)" << endl;
                 //    error = 1;
                 //    isword = 0;
                 // }
                 //else 
                 isword = 0;
             }
	     if (pass == 2){
                string labelcolon = lexeme+":";
                //cout << labelMap[lexeme] << "with" << labelMap[lexeme+":"] << endl; // delete this   
 	     if (labelMap.find(labelcolon) != labelMap.end()){
			int address = labelMap[labelcolon]; 
			putchar((address >> 24) & 0xff);
                	putchar((address >> 16) & 0xff);
                	putchar((address >> 8) & 0xff);
                	putchar(address & 0xff);
                	prev_token = *it2;
                } 
             	else{
                	cerr << "ERROR: label " << lexeme << " not defined (pass 2)" << endl;
                	error = 1;
                        isword = 0;
                }  
             }
        }
        else if (error == 0 && pass == 1){
	   cerr << "ERROR: generic" << endl;
        }       
       //cerr<< "  Token: "  << *(*it2) << endl;
      }         
    }
   if (error == 0 && pass == 1) map_print(labelMap);
}

int main(int argc, char* argv[]){
  // Nested vector representing lines of Tokens
  // Needs to be used here to cleanup in the case
  // of an exception
  vector< vector<Token*> > tokLines;
  map<string,int> labelMap;
  try{
    // Create a MIPS recognizer to tokenize
    // the input lines
    Lexer lexer;
    // Tokenize each line of the input
    string line;
    while(getline(cin,line)){
      tokLines.push_back(lexer.scan(line));
    }
     
    input_checker(tokLines, labelMap, 1);
    input_checker(tokLines, labelMap, 2);
  }
  catch(const string& msg){
    // If an exception occurs print the message and end the program
    cerr << msg << endl;
  }
  // Delete the Tokens that have been made
  vector<vector<Token*> >::iterator it;
  for(it = tokLines.begin(); it != tokLines.end(); ++it){
    vector<Token*>::iterator it2;
    for(it2 = it->begin(); it2 != it->end(); ++it2){
      delete *it2;
    }
  }

}
