#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <string>
#include <map>
#include "Assign4helper.h"
using namespace std;

int main (int argc,char *argv[]) 
{
    ifstream lfile(argv[2]);
    /*else
    {
        cout<<"No input file passed";
        return 0;
    }*/
    string line,num,eq;    
    pair<string,int> label1;
    if(argc>2)
    {
        if(lfile.is_open())
        {
            while ( lfile>>line )
            {
                if(line[0]=='#')
                {
                    string leaveline;
                    getline(lfile,leaveline);
                    continue;
                }
                for(int i=0;i<line.length();i++)
                {
                    if(line[i]>='A'&&line[i]<='Z')
                    {
                        line[i] = line[i] - 'A' + 'a';
                    }
                }
                lfile>>eq;
                if(eq=="=")
                {
                    lfile>>num;
                    if(num[num.length()-1]==';') num = num.substr(0,num.length()-1);
                    if(!valid(num))
                    {
                        cout<<"Latency of "<<line<<"not Valid in latency file\n";
                        return 0;
                    }
                }
                else
                {
                    cout<<"Error in latency File\n";
                    return 0;
                }
                latency[line] = stoi(num);
            }  
        }
    }
    ifstream myfile(argv[1]);
    if (myfile.is_open())
    {
        cout<<"Initial value of the registers before execution of the Program:"<<endl;
        for(int i=0;i<13;i++)
        {
            cout<<" Register "<<i<<" = "<<Register[i]<<endl;
        }
        cout<<" Stack pointer : "<<sp<<"\n\n";
        while ( myfile>>line )
        {
            if(line==",") continue;
            if(line[line.length()-1]==',') line = line.substr(0,line.length()-1);
            for(int i=0;i<line.length();i++)
            {
                if(line[i]>='A'&&line[i]<='Z')
                {
                    line[i] = line[i] - 'A' + 'a';
                }
            }
            if(line[line.length() - 1]==':')
            {
                label1.second = instructions.size();
                label1.first = line.substr(0,line.length()-1);
                labels.push_back(label1);
                continue;
            }
            if(line[0]=='@'){
                string leaveline;
                getline(myfile,leaveline);
                continue;
            }
            instructions.push_back(line);
        }

        myfile.close();
        readProgram();
        cout<<fixed;
        cout<<"\n\nProgram Execution Completed\n\nTotal Number of Instructions Executed : "<<totalInstructions<<"\nTotal Number of Clock Cycles taken : "<<cycles<<"\nAverage Instructions per Cycle : "<<setprecision(2)<<(double(totalInstructions)/cycles)<<"\n\nFinal value of the registers after execution of the Program :"<<endl;
        for(int i=0;i<13;i++)
        {
            cout<<" Register "<<i<<" = "<<Register[i]<<endl;
        }
        cout<<"\nFinal value of stack pointer: "<<sp<<endl;
        cout<<"\nThe following are the changed memory locations after the execution of the program:"<<endl;
        for(int i=0;i<100;i++)
        {
            if(memory[i]!=0)
            {
                cout<<"Memory "<<i<<" = "<<memory[i]<<endl;
            }
        }
    }
    else cout << "Unable to open file";
    return 0;
}