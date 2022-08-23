# JAVM - Just Another Virtual Machine  

## Description
VM is a software program that run physical machine to act as a virtual computer system with its own set of instruction and other components.
<br>
They are used in many different applications:
- Simulator/emulator (VMWare, VirtualBox, etc.)
- Execute program on cross-platform (Java Virtual Machine, etc.)
## About the project
 JAVM is a stack-based machine (similar to JVM) that operates in single-threaded mechanism with JAStack (stackframe core).
 <br>
 This project implements 2 main component:
 - **Operand Stack**: Stack for pushing/poping _operand value, results, parameters of function_
 - **Local Variable Space**: space containing local variable being to executed.

 > Note: This project implements Local Variable Space using __AVL-Tree__ data structure.

## Table of Content
- [Document](#docs)
- [Setup](#setup)
- [Compile](#compile)

### Document
- [JAVM Requirement](JAVM___Assignment_2__Vietnamese_.pdf) for expected outcome
- [JAVM Specifications](JAVM___Specifications__Vietnamese_.pdf) for more detailed implementation.

### Setup
Step 1: Download and unzip `initial.zip`<br>
Step 2: The unzipped folder should includes: `main.cpp, errors.h, constants.h, StackFrame.h, StackFrame.cpp`<br>
Step 3: Modify the inital code in `StackFrame.h` and `StackFrame.cpp` as required in the specification.

> Sample test and expected results are in `testcase` and `solution` folder for testing and debugging.

### Compile 
Use C++11 standard to compile the program. For example, with g++ compiler:<br>
```$ g++ main.cpp StackFrame.cpp-o main-std=c++11```

After `main.exe` created, run ```$ .\testcase\test00x.txt``` x = 0..4 to and check for expected outcomes in `\solution\sol00x.txt`.

You can test with your own testcases as needed.


