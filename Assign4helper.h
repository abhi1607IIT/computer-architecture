#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <string>
#include <map>
using namespace std;
vector <string> instructions;
map <string,int> latency;
bool elatency = false;
vector <pair<string,int>> labels;
int Register[13]={0},pc,lr,sp = 30,operand1,operand2,operand3,memory[100]={0},stack[31] = {0};  // created a memory of size 100 and stack of size 30
int totalInstructions = 0,cycles = 0;
bool valid(string s)                    // helper function in determining operand for register
{
    if(s.length()>8) return false;
    for(int i = 1;i<s.length();i++)
    {
        if(s[i]<'0'||s[i]>'9') return false;
    }
    return true;
}
int find(string s)
{
    for(int i=0;i<labels.size();i++)
    {                                        // helper function used to set pc in branch statements
        if(labels[i].first == s)
        {
            return labels[i].second;
        }
    }
    return -1;
}
int s2i(string s)                     // function that converts string to integer
{
    int ans = 0;
    for(int i=1;i<s.length();i++)
    {
        ans*=10;
        ans+=(s[i]-'0');
    }
    return ans;
}

int rvalue(string s)                 //to determine register value
{
    if(s.length()==2&&s[1]<='9')
    {
        operand1 = s[1] - '0';
    }
    else if (s.length()==3&&s[1]!='0'&&s[1]<='9')
    {
        operand1 = (s[1] - '0')*10 + (s[2] - '0');
    }
    else return -1;
}

void readProgram()
{
    int i = 0;
    while(i!=instructions.size())
    {
        if(instructions[i]=="mov")              // mov instruction 
        {
            i++;
            bool counter = false;
            if(instructions[i][0]=='r')
            {
                operand1 = rvalue(instructions[i]);
                if(operand1<0||operand1>12)
                {
                    cout<<"Invalid operand1 in mov Instruction\n";
                    break;
                }
            }
            else if(instructions[i]=="pc"||instructions[i]=="PC")
            {
                counter = true;
            }
            else
            {
                cout<<"Invalid mov Instruction\n";
                break;
            }
            i++;
            if(instructions[i][0]=='r')
            {
                operand2 = rvalue(instructions[i]);

                if(operand2<0||operand2>12)
                {
                    cout<<"Invalid operand2 in mov Instruction\n";
                    break;
                }
                Register[operand1] = Register[operand2];
                cout<<"Moved the value from register"<<operand2<<" to register"<<operand1<<endl;
            }
            else if(instructions[i][0]=='#')
            {
                if(valid(instructions[i])) Register[operand1] = s2i(instructions[i]);
                else
                {
                    cout<<"Invalid constant operand2 in mov Instruction\n";
                    break;
                }
                cout<<"Moved the constant value "<<s2i(instructions[i])<<" to register"<<operand1<<endl;
            }
            else if(instructions[i]=="lr"||instructions[i]=="LR")
            {
                if(counter)
                {
                    i = lr-1;
                    cout<<"Moved value of Link register to the required register\n";
                }
                else
                {
                    cout<<"Invalid \"mov\" instruction";
                    break;
                }
            }
            else
            {
                cout<<"Invalid mov Instruction\n";
                break;
            }
            i++;
            totalInstructions += 1;
            cycles += latency["mov"];
        }
        else if(instructions[i]=="add")            //add instruction
        {
            bool flag2=false,flag3=false,stackpointer1=false,stackpointer2=false;
            i++;
            if(instructions[i][0]=='r')
            {
                operand1 = rvalue(instructions[i]);
                if(operand1<0||operand1>12)
                {
                    cout<<"Invalid operand1 in add Instruction\n";
                    break;
                }
            }
            else if(instructions[i][0]=='s'&&instructions[i][1]=='p'){
                stackpointer1=true;
            }
            else
            {
                cout<<"Invalid add Instruction\n";
                break;
            }
            i++;
            if(instructions[i][0]=='r')
            {
                operand2 = rvalue(instructions[i]);
                if(operand2<0||operand2>12)
                {
                    cout<<"Invalid operand2 in add Instruction\n";
                    break;
                }
            }
            else if(instructions[i][0]=='s'&&instructions[i][1]=='p'){
                stackpointer2=true;
            }
            else if(instructions[i][0]=='#')
            {
                if(valid(instructions[i]))
                {
                  operand2 = s2i(instructions[i]);
                  flag2=true;  
                } 
                else
                {
                    cout<<"Invalid constant operand2 in add Instruction\n";
                    break;
                }
            }
            else
            {
                cout<<"Invalid add Instruction\n";
                break;
            }
            i++;
            if(instructions[i][0]=='r')
            {
                operand3 = rvalue(instructions[i]);
                if(operand3<0||operand3>12)
                {
                    cout<<"Invalid operand3 in add Instruction\n";
                    break;
                }
            }
            else if(instructions[i][0]=='#')
            {
                if(valid(instructions[i]))
                {
                    operand3 = s2i(instructions[i]);
                    flag3=true;
                }
                else
                {
                    cout<<"Invalid constant operand3 in add Instruction\n";
                    break;
                }
            }
            else
            {
                cout<<"Invalid Instruction\n";
                break;
            }

            if(flag2==false&&flag3==false){
                Register[operand1]=Register[operand2]+Register[operand3];
                cout<<"Added sucessfully .The value of register "<<operand1<<" is "<<Register[operand1]<<endl;
            }
            else if(flag2==false&&flag3==true){
                if(stackpointer1&&stackpointer2){
                    sp=sp+operand3;
                    cout<<"stack pointed shifted succesfully .Final value of stack pointer is "<<sp<<endl;
                }
                else if(!stackpointer1&&stackpointer2){
                    cout<<"wrong add sp syntax"<<endl;
                }
                else if(stackpointer1&&!stackpointer2){
                    cout<<"wrong add sp syntax"<<endl;
                }
                else{
                    Register[operand1]=Register[operand2] + operand3;
                    cout<<"Added sucessfully .The value of register "<<operand1<<" is "<<Register[operand1]<<endl;

                }
            }
            else if(flag2==true&&flag3==true){
                cout<<"invalid format as both operands can't be constant"<<endl;
            }
            else{
                cout<<"Invalid format for add Instruction"<<endl;
            }
            i++;
            totalInstructions+=1;
            cycles+=latency["add"];

        }
        else if(instructions[i]=="sub")    // sub instruction
        {
            bool flag2=false,flag3=false,stackpointer1=false,stackpointer2=false;
            i++;
            if(instructions[i][0]=='r')
            {
                operand1 = rvalue(instructions[i]);
                if(operand1<0||operand1>12)
                {
                    cout<<"Invalid operand1 in sub Instruction\n";
                    break;
                }
            }
            else if(instructions[i][0]=='s'&&instructions[i][1]=='p'){
                stackpointer1=true;
            }
            else
            {
                cout<<"Invalid sub Instruction\n";
                break;
            }
            i++;
            if(instructions[i][0]=='r')
            {
                operand2 = rvalue(instructions[i]);
                if(operand2<0||operand2>12)
                {
                    cout<<"Invalid operand2 in sub Instruction\n";
                    break;
                }
            }
            else if(instructions[i][0]=='s'&&instructions[i][1]=='p'){
                stackpointer2=true;
            }
            else if(instructions[i][0]=='#')
            {
                if(valid(instructions[i]))
                {
                  operand2 = s2i(instructions[i]);
                  flag2=true;  
                } 
                else
                {
                    cout<<"Invalid constant operand2 in sub Instruction\n";
                    break;
                }
            }
            else
            {
                cout<<"Invalid sub Instruction\n";
                break;
            }
            i++;
            if(instructions[i][0]=='r')
            {
                operand3 = rvalue(instructions[i]);
                if(operand3<0||operand3>12)
                {
                    cout<<"Invalid operand3 in sub Instruction\n";
                    break;
                }
            }
            else if(instructions[i][0]=='#')
            {
                if(valid(instructions[i]))
                {
                    operand3 = s2i(instructions[i]);
                    flag3=true;
                }
                else
                {
                    cout<<"Invalid constant operand3 in sub Instruction\n";
                    break;
                }
            }
            else
            {
                cout<<"Invalid sub Instruction\n";
                break;
            }

            if(flag2==false&&flag3==false){
                Register[operand1]=Register[operand2]-Register[operand3];
                cout<<"Subtracted sucessfully .The value of register"<<operand1<<" is "<<Register[operand1]<<endl;
            }
            else if(flag2==false&&flag3==true){
                if(stackpointer1&&stackpointer2){
                    sp=sp- operand3;
                    cout<<"stack pointer shifted succesfully . Value of sp is "<<sp<<endl;
                }
                else if(!stackpointer1&&stackpointer2){
                    cout<<"wrong sub sp syntax"<<endl;
                }
                else if(stackpointer1&&!stackpointer2){
                    cout<<"wrong sub syntax"<<endl;
                }
                else{
                    Register[operand1]=Register[operand2] - operand3;
                    cout<<"Subtracted sucessfully .The value of register"<<operand1<<" is "<<Register[operand1]<<endl;
                }
                
            }
            else if(flag2==true&&flag3==true){
                cout<<"invalid format as both operands can't be constant"<<endl;
            }
            else{
                cout<<"invalid format for sub Instruction"<<endl;
            }
            i++;
            totalInstructions+=1;
            cycles+=latency["sub"];

        }
        else if(instructions[i]=="ldr") //ldr instruction
        {
            bool stackpointer1=false,rread = false,link = false;
            i++;
            if(instructions[i][0]=='r')
            {
                operand1 = rvalue(instructions[i]);
                if(operand1<0||operand1>12)
                {
                    cout<<"Invalid operand1 in ldr Instruction\n";
                    break;
                }
            }
            else if(instructions[i]=="lr"||instructions[i]=="LR")
            {
                link = true;
            }
            else
            {
                cout<<"Invalid Instruction\n";
                break;
            }
            i++;
            if(instructions[i] =="[") i++;
            else
            {
                cout<<"Invalid format of operand2 in ldr,'[' Missing\n";
                break;
            }
            if(instructions[i][0]=='r')
            {
                operand2 = rvalue(instructions[i]);
                rread = true;
                if(operand2<0||operand2>12)
                {
                    cout<<"Invalid operand2 in ldr Instruction\n";
                    break;
                }
            }
            else if(instructions[i][0]=='s'&&instructions[i][1]=='p')
            {
                stackpointer1=true;
            }
            else
            {
                cout<<"Invalid Instruction\n";
                break;
            }
            i++;
            if(instructions[i][0]=='#')
            {
                if(valid(instructions[i]))
                {
                    operand3 = s2i(instructions[i]);
                }
                else
                {
                    cout<<"Wrong ldr syntax, wrong offset"<<endl;
                    break;
                }
            }
            else{
                cout<<"Wrong ldr syntax, offset missing"<<endl;
            }
            if(stackpointer1)
            {
                if(sp+operand3<30)
                {
                    if(link)
                    {
                        lr = stack[sp + operand3];
                        cout<<"Loaded value in Link Register sucessfully from stack location "<<sp + operand3<<endl;
                    }
                    else
                    {
                        Register[operand1]=stack[sp + operand3];
                        cout<<"Loaded value in Register "<<operand1<<" sucessfully from stack location "<<sp + operand3<<endl;
                    }
                }
                else{
                    cout<<"stack memory can't be loaded"<<endl;
                }
            }
            else if(Register[operand2] + operand3>=100)
            {
                cout<<"Memory can't be accesed"<<endl;
            }
            else
            {
                if(link)
                {
                    lr = memory[Register[operand2]+operand3];
                    cout<<"Loaded value in Register "<<operand1<<" sucessfully from memory location "<<Register[operand2] + operand3<<endl;
                }
                else
                {
                    Register[operand1]=memory[Register[operand2]+operand3];
                    cout<<"Loaded value in Register "<<operand1<<" sucessfully from memory location "<<Register[operand2] + operand3<<endl;
                }
            }
            i++;
            if(instructions[i]=="]") i++;
            else
            {
                cout<<" ']' Missing \n";
                break;
            }
            totalInstructions+=1;
            cycles+=latency["ldr"];

        }
        else if(instructions[i]=="cmp")    // cmp function
        {
            bool greater = false,equal = false;
            i++;
            if(instructions[i][0]=='r')
            {
                operand1 = rvalue(instructions[i]);
                if(operand1<0||operand1>12)
                {
                    cout<<"Invalid operand1 in cmp Instruction\n";
                    break;
                }
            }
            else
            {
                cout<<"Invalid cmp Instruction\n";
                break;
            }
            i++;
            if(instructions[i][0]=='r')
            {
                operand2 = rvalue(instructions[i]);

                if(operand2<0||operand2>12)
                {
                    cout<<"Invalid operand2 in mov Instruction\n";
                    break;
                }
                if(Register[operand1]>Register[operand2]) greater = true;
                if(Register[operand1]==Register[operand2]) equal = true;
            }
            else if(instructions[i][0]=='#')
            {
                if(valid(instructions[i])) operand2 = s2i(instructions[i]);
                else
                {
                    cout<<"Invalid constant operand2 in cmp Instruction\n";
                    break;
                }
                if(Register[operand1]>operand2) greater = true;
                if(Register[operand1]==operand2) equal = true;
                
            }
            else
            {
                cout<<"Invalid cmp Instruction\n";
                break;
            }
            i++;
            if(instructions[i]=="bne")
            {
                i++;
                if(!equal)
                {
                    i = find (instructions[i]);
                    if(i>0) continue;
                    else cout<<"Invalid Label in bne\n";
                    break;
                }
            }
            if(instructions[i]=="bge")
            {
                i++;
                if(equal||greater)
                {
                    i = find(instructions[i]);
                    if(i>0) continue;
                    else cout<<"Invalid Label in bge\n";
                    break;
                }
            }
            i++;
            totalInstructions+=1;
            cycles+=latency["cmp"];
        }
        else if(instructions[i]=="str")
        {
            bool stackpointer1=false,rread = false,link = false;
            i++;
            if(instructions[i][0]=='r')
            {
                operand1 = rvalue(instructions[i]);
                if(operand1<0||operand1>12)
                {
                    cout<<"Invalid operand1 in str Instruction\n";
                    break;
                }
            }
            else if(instructions[i]=="lr"||instructions[i]=="LR")
            {
                link = true;    
            }
            else
            {
                cout<<"Invalid Instruction\n";
                break;
            }
            i++;
            if(instructions[i] =="[") i++;
            else
            {
                cout<<"Invalid format of operand2 in str,'[' Missing\n";
                break;
            }

            if(instructions[i][0]=='r')
            {
                operand2 = rvalue(instructions[i]);

                if(operand2<0||operand2>12)
                {
                    cout<<"Invalid operand2 in str Instruction\n";
                    break;
                }
            }
            else if(instructions[i][0]=='s'&&instructions[i][1]=='p'){
                stackpointer1=true;
            }
            else
            {
                cout<<"Invalid Instruction\n";
                break;
            }
            i++;
            if(instructions[i][0]=='#')
            {
                if(valid(instructions[i]))
                {
                    operand3 = s2i(instructions[i]);
                }
                else
                {
                    cout<<"Wrong str syntax, wrong offset"<<endl;
                    break;
                }
            }
            else
            {
                cout<<"Wrong str syntax, offset missing"<<endl;
            }
            if(stackpointer1)
            {
                if(sp+operand3<30)
                {
                    if(link)
                    {
                        stack[sp + operand3]=lr;
                        cout<<"Stored value in Link register sucessfully to stack location "<<sp + operand3<<endl;
                    }
                    else
                    {
                        stack[sp + operand3]=Register[operand1];
                        cout<<"Stored value in Register "<<operand1<<" sucessfully to stack location "<<sp + operand3<<endl;
                    }
                }
                else
                {
                    cout<<"stack memory can't be loaded"<<endl;
                }

            }
            else if(Register[operand2] + operand3>=100){
                cout<<"Memory can't be accesed"<<endl;
            }
            else
            {
                if(link)
                {
                    memory[Register[operand2]+operand3] =lr;
                    cout<<"Stored value in Link Register sucessfully to memory location "<<Register[operand2] + operand3<<endl;
                }
                else
                {
                    memory[Register[operand2] + operand3]=Register[operand1];
                    cout<<"Stored value in Register "<<operand1<<" sucessfully to memory location "<<Register[operand2] + operand3<<endl;
                }
            }
            i++;
            if(instructions[i]=="]") i++;
            else
            {
                cout<<" ']' Missing \n";
                break;
            }
            totalInstructions+=1;
            cycles+=latency["str"];
        }
        else if(instructions[i]=="mul")
        {
            i++;
            if(instructions[i][0]=='r')
            {
                operand1 = rvalue(instructions[i]);
                if(operand1<0||operand1>12)
                {
                    cout<<"Invalid operand1 in mov Instruction\n";
                    break;
                }
            }
            else
            {
                cout<<"Invalid mov Instruction\n";
                break;
            }
            i++;
            if(instructions[i][0]=='r')
            {
                operand2 = rvalue(instructions[i]);
                if(operand2<0||operand2>12)
                {
                    cout<<"Invalid operand2 in mov Instruction\n";
                    break;
                }
            }
            else
            {
                cout<<"Invalid mov Instruction\n";
                break;
            }
            i++;
            if(instructions[i][0]=='r')
            {
                operand3 = rvalue(instructions[i]);
                if(operand3<0||operand3>12)
                {
                    cout<<"Invalid operand3 in mov Instruction\n";
                    break;
                }
                Register[operand1] = Register[operand2] * Register[operand3];
            }
            else
            {
                cout<<"Invalid mov Instruction\n";
                break;
            }
            i++;
            totalInstructions+=1;
            cycles+=latency["mul"];
        }
        else if(instructions[i]=="b")
        {
            i++;
            i = find(instructions[i]);
            if(i>0)
            {
                totalInstructions+=1;
                cycles+=latency["b"];
                continue;

            }
            else cout<<"Invalid Label in b\n";
            break;
        }
        else if(instructions[i]=="bl")
        {
            i++;
            lr = i+1;
            i = find(instructions[i]);
            if(i>0)
            {
                totalInstructions+=1;
                cycles+=latency["sub"];
                continue;
            }
            else cout<<"Invalid Label in b\n";
            break;
        }
    }
}
