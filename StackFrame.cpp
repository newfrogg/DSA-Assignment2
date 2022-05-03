#include "StackFrame.h"
#include <iostream>
#include <fstream>
#include "errors.h"
#include "constants.h"
using namespace std;

/*
Types of instruction
*/
enum instructionType
{
    Arithmetic,
    loadstore,
    typeConversion,
    operandStackManagement,
    localVariableManagement
};

/*
1. TypeMisMatch
2. DivideByZero
3. StackFull
4. StackEmpty
5. LocalSpaceFull
6. UndefinedVariable
@param numberOfexception Type of Exception
*/
void StackFrame::raiseException(int numberOfexception)
{
    IsExcep = true;
    switch (numberOfexception)
    {
    case 1:
    {
        throw TypeMisMatch(PC);
        break;
    }
    case 2:
    {
        throw DivideByZero(PC);
        break;
    }
    case 3:
    {
        throw StackFull(PC);
        break;
    }
    case 4:
    {
        throw StackEmpty(PC);
        break;
    }
    case 5:
    {
        throw LocalSpaceFull(PC);
        break;
    }
    case 6:
    {
        throw UndefinedVariable(PC);
        break;
    }
    }
}

StackFrame::StackFrame()
{
    opStackMaxSize = OPERAND_STACK_MAX_SIZE;
    localVarSpaceSize = LOCAL_VARIABLE_SPACE_SIZE;

    OperandStack = new Stack<operand>();
    LocalVariableSpace = new AVLTree<string, variable>();

    PC = 0;
    IsExcep = false;
};

StackFrame::~StackFrame()
{
    delete OperandStack;
    delete LocalVariableSpace;
}

void StackFrame::runloadstore(string instr, string var, operand val)
{
    if (instr == "iconst" || instr == "fconst")
    {
        if (OperandStack->isFull(opStackMaxSize))
        {
            raiseException(3);
        }
        else
        {
            OperandStack->push(val);
            operand i1;
            i1.data_bool = instr[0] == 'f';
            OperandStack->push(i1);
        }
    }
    else if (instr == "iload" || instr == "fload")
    {
        AVLNode<string, variable> *load = LocalVariableSpace->find(var);
        if (!load)
        {
            raiseException(6);
        }
        else
        {
            if ((load->isFloat == true && instr[0] == 'i') ||
                (load->isFloat == false && instr[0] == 'f'))
            {
                raiseException(1);
            }
            else
            {
                if (OperandStack->isFull(opStackMaxSize))
                {
                    raiseException(3);
                }
                else
                {
                    operand l1;
                    if (instr[0] == 'i')
                    {
                        l1.data_int = load->val.data_int;
                    }
                    else
                    {
                        l1.data_float = load->val.data_float;
                    }
                    OperandStack->push(l1);
                    operand l2;
                    l2.data_bool = load->isFloat;
                    OperandStack->push(l2);
                }
            }
        }
    }
    else if (instr == "istore" || instr == "fstore")
    {
        if (OperandStack->empty())
        {
            raiseException(4);
        }
        else
        {
            operand t1, t2;
            t1 = OperandStack->top();
            OperandStack->pop();
            t2 = OperandStack->top();
            OperandStack->pop();
            if ((t1.data_bool == true && instr[0] != 'f') ||
                (t1.data_bool == false && instr[0] != 'i'))
            {
                raiseException(1);
            }
            else
            {
                if (LocalVariableSpace->isFull(localVarSpaceSize))
                {
                    raiseException(5);
                }
                else
                {
                    variable t;
                    if (t1.data_bool)
                    {
                        t.data_float = t2.data_float;
                    }
                    else
                    {
                        t.data_int = t2.data_int;
                    }
                    LocalVariableSpace->insert(var, t1.data_bool, t);
                }
            }
        }
    }
}
void StackFrame::runLocalVariableManagement(string instr, string var)
{
    AVLNode<string, variable> *temp = LocalVariableSpace->find(var);
    if (!temp)
    {
        raiseException(6);
    }
    // val <var> instruction
    if (instr[0] == 'v')
    {
        if (temp->isFloat == true)
        {
            cout << temp->val.data_float << '\n';
        }
        else
        {
            cout << temp->val.data_int << '\n';
        }
    }
    // par <var> instruction
    else
    {
        AVLNode<string, variable> *parent = LocalVariableSpace->findParent(var);
        if (parent)
        {
            cout << parent->key << '\n';
        }
        else
        {
            cout << "null\n";
        }
    }
}
void StackFrame::runOperandStackManagement(string instr)
{
    // Only instruction top
    if (OperandStack->empty())
    {
        raiseException(4);
    }
    else
    {
        operand temp;
        temp = OperandStack->top();
        OperandStack->pop();
        if (temp.data_bool == false)
        {
            cout << OperandStack->top().data_int << '\n';
        }
        else
        {
            cout << OperandStack->top().data_float << '\n';
        }
        OperandStack->push(temp);
    }
}

void StackFrame::runTypeConversion(string instr)
{
    if (OperandStack->empty())
    {
        raiseException(4);
    }
    else
    {
        operand temp;
        temp = OperandStack->top();
        OperandStack->pop();
        if ((temp.data_bool == false && instr[0] != 'i') ||
            (temp.data_bool == true && instr[0] != 'f'))
        {
            raiseException(1);
        }
        else
        {
            operand data;
            data = OperandStack->top();
            OperandStack->pop();
            if (instr[2] == 'f')
            {
                data.data_float = float(data.data_int);
            }
            else
            {
                data.data_int = int(data.data_float);
            }
            OperandStack->push(data);
            temp.data_bool = (instr[2] == 'f');
            OperandStack->push(temp);
        }
    }
}

void StackFrame::runArithmetic(string instr)
{
    if (instr != "ibnot" && instr != "ineg" && instr != "fneg")
    {
        if (OperandStack->size() <= 2)
        {
            raiseException(4);
        }
        else
        {
            operand t1, t2;
            t1 = OperandStack->top();
            OperandStack->pop();
            t2 = OperandStack->top();
            OperandStack->pop();

            operand v1, v2;
            v1 = OperandStack->top();
            OperandStack->pop();
            v2 = OperandStack->top();
            OperandStack->pop();

            if (instr[0] == 'i')
            {
                if (t1.data_bool || v1.data_bool)
                {
                    raiseException(1);
                }
                else
                {
                    operand w1, w2;
                    w2.data_bool = false;
                    if (instr == "iadd")
                    {
                        w1.data_int = v2.data_int + t2.data_int;
                    }
                    else if (instr == "isub")
                    {
                        w1.data_int = v2.data_int - t2.data_int;
                    }
                    else if (instr == "imul")
                    {
                        w1.data_int = v2.data_int * t2.data_int;
                    }
                    else if (instr == "idiv")
                    {
                        if (t2.data_int == 0)
                        {
                            raiseException(2);
                        }
                        else
                        {
                            w1.data_int = v2.data_int / t2.data_int;
                        }
                    }
                    else if (instr == "irem")
                    {
                        if (t2.data_int == 0)
                        {
                            raiseException(2);
                        }
                        else
                        {
                            w1.data_int = v2.data_int - (v2.data_int / t2.data_int) * t2.data_int;
                        }
                    }
                    else if (instr == "iand")
                    {
                        w1.data_int = v2.data_int & t2.data_int;
                    }
                    else if (instr == "ior")
                    {
                        w1.data_int = v2.data_int | t2.data_int;
                    }
                    else if (instr == "ieq")
                    {
                        w1.data_int = v2.data_int == t2.data_int;
                    }
                    else if (instr == "ineq")
                    {
                        w1.data_int = v2.data_int != t2.data_int;
                    }
                    else if (instr == "ilt")
                    {
                        w1.data_int = v2.data_int < t2.data_int;
                    }
                    else if (instr == "igt")
                    {
                        w1.data_int = v2.data_int > t2.data_int;
                    }

                    OperandStack->push(w1);
                    OperandStack->push(w2);
                }
            }
            else if (instr[0] == 'f')
            {
                if (!t1.data_bool && !v1.data_bool)
                {
                    t2.data_float = float(t2.data_int);
                    v2.data_float = float(v2.data_int);
                }
                else if (!t1.data_bool && v1.data_bool)
                {
                    t2.data_float = float(t2.data_int);
                }
                else if (t1.data_bool && !v1.data_bool)
                {
                    v2.data_float = float(v2.data_int);
                }
                operand w1, w2;
                if (instr == "fadd")
                {
                    w1.data_float = v2.data_float + t2.data_float;
                    w2.data_bool = true;
                }
                else if (instr == "fsub")
                {
                    w1.data_float = v2.data_float - t2.data_float;
                    w2.data_bool = true;
                }
                else if (instr == "fmul")
                {
                    w1.data_float = v2.data_float * t2.data_float;
                    w2.data_bool = true;
                }
                else if (instr == "fdiv")
                {
                    if (t2.data_float == 0.0)
                    {
                        raiseException(2);
                    }
                    else
                    {
                        w1.data_float = v2.data_float / t2.data_float;
                        w2.data_bool = true;
                    }
                }
                else if (instr == "feq")
                {
                    w1.data_int = (v2.data_float == t2.data_float) ? 1 : 0;
                    w2.data_bool = false;
                }
                else if (instr == "fneq")
                {
                    w1.data_int = (v2.data_float != t2.data_float) ? 1 : 0;
                    w2.data_bool = false;
                }
                else if (instr == "flt")
                {
                    w1.data_int = (v2.data_float < t2.data_float) ? 1 : 0;
                    w2.data_bool = false;
                }
                else if (instr == "fgt")
                {
                    w1.data_int = (v2.data_float > t2.data_float) ? 1 : 0;
                    w2.data_bool = false;
                }

                OperandStack->push(w1);
                OperandStack->push(w2);
            }
        }
    }
    else if (instr == "ineg" || instr == "fneg")
    {
        if (OperandStack->empty())
        {
            raiseException(4);
        }
        else
        {
            operand t1, t2;
            t1 = OperandStack->top();
            OperandStack->pop();
            t2 = OperandStack->top();
            OperandStack->pop();

            if (t1.data_bool == true && instr[0] == 'i')
            {
                raiseException(1);
            }
            else if (t1.data_bool == false && instr[0] == 'f')
            {
                t2.data_float = -float(t2.data_int);
                t1.data_bool = true;
            }
            else
            {
                if (instr[0] == 'i')
                {
                    t2.data_int *= -1;
                    t1.data_bool = false;
                }
                else
                {
                    t2.data_float *= -1.0;
                    t1.data_bool = true;
                }
            }
            OperandStack->push(t2);
            OperandStack->push(t1);
        }
    }
    else if (instr == "ibnot")
    {
        if (OperandStack->empty())
        {
            raiseException(4);
        }
        else
        {
            operand t1, t2;
            t1 = OperandStack->top();
            OperandStack->pop();
            t2 = OperandStack->top();
            OperandStack->pop();
            if (t1.data_bool == true)
            {
                raiseException(1);
            }
            else
            {
                t2.data_int = (t2.data_int == 0) ? 1 : 0;
                OperandStack->push(t2);

                t1.data_bool = false;
                OperandStack->push(t1);
            }
        }
    }
}

void decodeforProcessing(string line, string &instr, operand &val, string &var, instructionType &typeOfinstr)
{
    int i = 0, size = line.size();
    while (i < size && line[i] != ' ')
    {
        instr += line[i];
        i++;
    }
    if (!instr.empty() && instr[instr.size() - 1] == '\r')
        instr.erase(instr.size() - 1);

    if (i < size - 1)
    {
        ++i;
        string temp = "";
        while (i < size)
        {
            temp += line[i];
            ++i;
        }
        if (instr == "val" || instr == "par")
        {
            typeOfinstr = localVariableManagement;
            if (!temp.empty() && temp[temp.size() - 1] == '\r')
                temp.erase(temp.size() - 1);
            var = temp;
        }
        else
        {
            typeOfinstr = loadstore;
            if (instr == "fconst")
            {
                val.data_float = stof(temp);
            }
            else if (instr == "iconst")
            {
                val.data_int = stoi(temp);
            }
            else
            {
                if (!temp.empty() && temp[temp.size() - 1] == '\r')
                    temp.erase(temp.size() - 1);
                var = temp;
            }
        }
    }
    else
    {
        if (instr == "top")
        {
            typeOfinstr = operandStackManagement;
        }
        else if (instr == "i2f" || instr == "f2i")
        {
            typeOfinstr = typeConversion;
        }
        else
        {
            typeOfinstr = Arithmetic;
        }
    }
}

void StackFrame::run(string filename)
{
    // Parameters of instruction
    string instr;

    operand val;
    string var;

    ifstream file;
    instructionType typeOfinstr;
    // Open file
    file.open(filename);

    while (!file.eof())
    {
        instr.clear();
        string line;
        getline(file, line);

        // Pre-processing
        decodeforProcessing(line, instr, val, var, typeOfinstr);

        // Processing
        ++PC;
        // cout << PC << ' ' << instr;
        // if (typeOfinstr == loadstore || typeOfinstr == localVariableManagement)
        // {
        //     cout << ' ';
        //     if (instr[1] == 'c')
        //     {
        //         cout << ((instr[0] == 'f') ? val.data_float : val.data_int);
        //     }
        //     else
        //     {
        //         cout << var;
        //     }
        // }
        // cout << '\n';
        switch (typeOfinstr)
        {
        case loadstore:
            runloadstore(instr, var, val);
            break;
        case localVariableManagement:
            runLocalVariableManagement(instr, var);
            break;
        case operandStackManagement:
            runOperandStackManagement(instr);
            break;
        case typeConversion:
            runTypeConversion(instr);
            break;
        case Arithmetic:
            runArithmetic(instr);
            break;
        }
    }
    // Close file
    file.close();
}