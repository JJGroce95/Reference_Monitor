#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <iomanip> 
#include <string.h>
#include "ref.h"


using namespace std;

int main(int argc, char *argv[])    //get command line text
{
    //check # of command line arguments
    if(argc>2 || argc < 2)
    {
        cout << "usage: secureblp instructionList\n";
        return 0;
    }
    strcat(argv[1], ".txt");
    ifstream file(argv[1]);
    if(file.is_open())
    {  
       RefMon Obj;
       while(!file.eof())
       {
           
           string str;
           while(getline(file,str))
           {
               //remove the \r 
               str.erase(remove(str.begin(), str.end(), '\r'), str.end());
               Obj.parser(str);
               
               
           }
       }
       //1 means EOF and to print final state.
       Obj.printState(1);
    }
    else cout << "Unable to open " << argv[1] << " file. Terminating...\n";
   file.close();
    
    return 0;
}