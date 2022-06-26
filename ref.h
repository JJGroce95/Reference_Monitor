#ifndef HEADER_H
#define HEADER_H
using namespace std;
class Subjects
{
    friend class RefMon;
    public:
        string name[50][1];       //subject name ie. jjg0159
        int value[50][1] = {0};   //subject value, initialized to zero
    
    void addName(vector <string> id, int totalNames)
    {
        name[totalNames][0] = id[1];
    }
    bool userCheck(int i, vector <string> user)
    {
        if(name[i][0] == user[1])
        {
            return true;
        }
        return false;
    }
    string getName(int i)
    {
        return name[i][0];
    }
    void setValue(int i, int num)
    {
        value[i][0] = num; 
    }
    int getValue(int i)
    {
        return value[i][0];
    }
};
class Objects
{
    friend class RefMon;
    public:
        string name[50][1];
        int value[50][1] = {0};
    
    void addName(vector <string> file, int totalFiles)
    {
        name[totalFiles][0] = file[1];
    }
    bool fileCheck(int j, vector <string> file)
    {   
        if(name[j][0] == file[2])
        {
            return true;
        }
        return false;
    }
    string getFile(int j)
    {
        return name[j][0];
    }
    int getValue(int j)
    {
        return value[j][0];
    }
    void setValue(int j, int val)
    {
        value[j][0] = val;
    }
};
class RefMon
{
    public:
        friend class Subjects;
        Subjects s;
        Objects o;
        vector <string> tokens;  //parsed tokens to be processed
        
        //for iterating
        int numUsers   = 0;       //number of users
        int numFiles   = 0;       //number of Files
        
        int user_secFlag[50];  //user security flag (low-medium-high)
        int file_secFlag[50];  //file security flag (low-medium-high)
        
        //parse the line into tokens and send them to destinations to be processed
        void parser(string command)
        {
            Subjects subj;
            stringstream check(command);
            string inter;
            int i = 0;      //countshow many tokens are in the line for bounds check
            
            //push tokens onto stack using whitespace as delimeter
            while(getline(check,inter, ' '))
            {
                tokens.push_back(inter);
                i++;
            }
            //Process tokens and send to respective classes
            if(tokens[0] == "addsub")
            {
                //check # of tokens for overflow or underflow
                if(i > 3 || i < 3)
                {
                    badInstruction();
                }
                else
                {
                    addSubject();
                }
            }
            else if(tokens[0] == "addobj")
            {
                //check # of tokens for overflow or underflow
                if(i > 3 || i < 3)
                {
                    badInstruction();
                }
                else
                {
                    addObject();
                }
            }
            else if(tokens[0] == "read")
            {
                //check # of tokens for overflow or underflow
                if(i > 3 || i < 3)
                {
                    badInstruction();
                }
                else
                {
                    executeRead();
                }
            }
            else if(tokens[0] == "write")
            {
                //check # of tokens for overflow or underflow
                if(i > 4 || i < 4)
                {
                    badInstruction();
                }
                else if(isNumber(tokens)== false)
                {
                    badInstruction();
                }
                else
                {
                    executeWrite();
                }
            }
            else if(tokens[0] == "status")
            {
                //check # of tokens for overflow or underflow
                if(i > 1)
                {
                    badInstruction();
                }
                else
                {
                    printState(0);
                }
            }
            else
            {
               badInstruction();
               tokens.clear();
            }
        }
        //add a username and security level
        void addSubject()
        {
            cout << "Subject added\t: addsub " << tokens[1] << " " << tokens[2] << endl;
            s.addName(tokens, numUsers);
            
            if(tokens[2] == "LOW")
            {
                user_secFlag[numUsers] = 1;
            }
            else if(tokens[2] == "MEDIUM")
            {
                user_secFlag[numUsers] = 2;
            }
            else if(tokens[2] == "HIGH")
            {
                user_secFlag[numUsers] = 3;
            }
            numUsers++;
            tokens.clear();
        }
        
        //add object and security level
        void addObject()
        {
            cout << "Object added\t: addobj " << tokens[1] << " " << tokens[2] << endl;
            o.addName(tokens, numFiles);
            if(tokens[2] == "LOW")
            {
                file_secFlag[numFiles] = 1;
            }
            else if(tokens[2] == "MEDIUM")
            {
                file_secFlag[numFiles] = 2;
            }
            else if(tokens[2] == "HIGH")
            {
                file_secFlag[numFiles] = 3;
            }
            numFiles++;
            tokens.clear();
        }
        
        //Check if user can read file
        void executeRead()
        {
            //check if user exists
            for(int i = 0; i<numUsers; i++)
            {
                if(s.userCheck(i, tokens) == true)
                {
                    
                    //check if file exists
                    for(int j = 0; j<numFiles; j++)
                    {
                        if(o.fileCheck(j, tokens) == true)
                        {
                            //check permissions
                            if(user_secFlag[i] >= file_secFlag[j])
                            {
                                cout << "Access Granted\t: " << s.getName(i) << " reads " << o.getFile(j) << endl;
                                s.setValue(i, o.getValue(j));
                                tokens.clear();
                                return;
                            }
                            else
                            {
                                cout << "Access Denied\t: read " << s.getName(i) << " " << o.getFile(j) << endl;
                                tokens.clear();
                                return;                                
                            }
                        }
                    }
                }
            }
            badInstruction();
        }
        
        //check if user can write to file
        void executeWrite()
        {
            //check if user exists
            for(int i = 0; i<numUsers; i++)
            {
                if(s.userCheck(i, tokens) == true)
                {
                    //check if file exists
                    for(int j = 0; j<numFiles; j++)
                    {
                        if(o.fileCheck(j, tokens) == true)
                        {
                            //star property rule
                            if(user_secFlag[i] <= file_secFlag[j])
                            {
                                cout << "Access Granted\t: " << s.getName(i) << " writes value " << tokens[3] << " to " << o.getFile(j) << endl;
                                
                                //convert tokens[3] into an int value and save it
                                stringstream temp(tokens[3]);
                                int number;
                                temp >> number;
                                o.setValue(j, number);
                                tokens.clear();
                                return;
                            }
                            else
                            {
                                cout << "Access Denied\t: read " << s.getName(i) << " " << o.getFile(j) << endl;
                                tokens.clear();
                                return;                                
                            }
                        }
                    }
                }
            }
            badInstruction();
        }
        
        //print current state of data
        void printState(int final)
        {
            if(final == 0)
            {
                cout << "+--------current state--------+\n";
                
            }
            else if(final == 1)
            {
                cout << "+---------final state---------+\n";

            }
            cout << "|-subject-|--level--|--value--|\n";
                
                for(int i = 0; i<numUsers; i++)
                {
                    cout << "| " << s.getName(i) << " |" << setw(5) << printSec(user_secFlag[i]) << setw(5) << "\t " << setw(5) << s.getValue(i) << "|" << endl;
                    if(i == numUsers-1)
                    {
                        cout << "|--object-|--level--|--value--|\n";
                        for(int j = 0; j<numFiles; j++)
                        {
                            cout << "| " << o.getFile(j) << " |" << setw(5) << printSec(file_secFlag[j]) << "\t " << setw(5) << o.getValue(j) << "|" << endl;
                        }
                    }
                }
                cout << "+-----------------------------+\n";
                tokens.clear();
        }
        
        //display error message for bad instruction
        void badInstruction()
        {
            
             cout << "Bad Instruction\t: ";
            for (auto i = tokens.begin(); i != tokens.end(); ++i)
            cout << *i << ' ';
            cout << endl;
            
            tokens.clear();
        }
            
        //print security level from number
        string printSec(int num)
        {
            string sec;
            if(num == 1)
            {
                sec = "LOW";
                return sec;
            }
            else if(num == 2)
            {
                sec = "MEDIUM";
                return sec;
            }
            else if(num == 3)
            {
                sec = "HIGH";
                return sec;
            }
            return 0;
        }
        
        //ensures a non-number isn't written to a file ie. x
        bool isNumber(vector<string> str)
        {
            for (char const &c : str[3]) 
            {
                if (std::isdigit(c) == 0) return false;
            }
            return true;
        }
        
};

#endif