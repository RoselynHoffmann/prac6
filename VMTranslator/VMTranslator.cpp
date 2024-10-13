#include <string>
#include "VMTranslator.h"

using namespace std;

int VMTranslator::counter = 0;
/**
 * VMTranslator constructor
 */
VMTranslator::VMTranslator() {
    // Your code here
}

/**
 * VMTranslator destructor
 */
VMTranslator::~VMTranslator() {
    // Your code here
}


string VMTranslator::vmArg(string arg, int i) {
    //gives me the relevant name for all functions to use

    if (arg == "that")
		return "THAT";
    if (arg == "local")
		return "LCL";
    if (arg == "this")
		return "THIS";
    if (arg == "static")
		return to_string(16 + i);
    if (arg == "temp")
		return "R" + to_string(5 + i);
    return arg;
}

/** Generate Hack Assembly code for a VM push operation */
string VMTranslator::vm_push(string segment, int offset){

    string result = "// Push " + segment + " " + to_string(offset) + "\n";
    //gives relevant input to identify what to push
    string input = vmArg(segment, offset);
    

    //different cases for what to push listed 
    if(segment == "constant") {
        result += "@" + to_string(offset) + "\n"; //loads constatn value to A
        result += "D=A"; // D = A
    }
    //pointers case
    else if (segment == "local" || segment == "argument" || segment == "this"
             || segment == "that") 
    {
            result += "@" + input + "\n";
            result += "D=M\n";
            result += "@" + to_string(offset) + "\n";
            result += "A=D+A\n";
            result += "D=M\n";
    }
    else if(segment == "static" || segment == "temp" || segment == "pointer") {
        result += "@" + input + "\n";
        result += "D=M\n"; //load D=M
    }

    result += "@SP\n";
	result += "AM=M+1\n";
	result += "A=A-1\n";
	result += "M=D\n";
    return result;
}

/** Generate Hack Assembly code for a VM pop operation */
string VMTranslator::vm_pop(string segment, int offset){ 

    string result = "// Pop " + segment + " " + to_string(offset) + "\n";
    string input = vmArg(segment, offset);

    if (segment == "constant")
    {
        throw runtime_error("Invalid pop");
    }
    else if (segment == "local" || segment == "argument" || segment == "this" || segment == "that")
    {
		result += "@" + input + "\n";
		result += "D=M\n";
		result += "@" + to_string(offset) + "\n";
		result += "D=D+A\n";
		result += "@R13\n";
		result += "M=D\n";
		result += "@SP\n";
		result += "AM=M-1\n";
		result += "D=M\n";
		result += "@R13\n";
		result += "A=M\n";
		result += "M=D\n";
    }
    else if(segment == "static" || segment == "temp" || segment == "pointer")
    {
		result += "@" + input + "\n";
		result += "D=A\n";
		result += "@R13\n";
		result += "M=D\n";
		result += "@SP\n";
		result += "AM=M-1\n";
		result += "D=M\n";
		result += "@R13\n";
		result += "A=M\n";
		result += "M=D\n";
    }
    else
    {
        throw runtime_error("Invalid");
    }
    return result;
}

string VMTranslator::vm_add()
{
	string result = "// Add\n";
	result += "@SP\n";
	result += "AM=M-1\n";
	result += "D=M\n";
	result += "A=A-1\n";
	result += "M=M+D\n";
	return result;
}

/** Generate Hack Assembly code for a VM sub operation */
string VMTranslator::vm_sub()
{
	string result = "// Sub\n";
	result += "@SP\n";
	result += "AM=M-1\n";
	result += "D=M\n";
	result += "A=A-1\n";
	result += "M=M-D\n";
	return result;
}

/** Generate Hack Assembly code for a VM neg operation */
string VMTranslator::vm_neg()
{
	string result = "// Neg\n";
	result += "@SP\n";
	result += "A=M\n";
	result += "A=A-1\n";
	result += "M=-M\n";
	return result;
}

/** Generate Hack Assembly code for a VM eq operation */
string VMTranslator::vm_eq(){

    //setup labels for the equalise operation
	string labelTrue = "TRUE" + to_string(counter);
	string labelLoop = "LOOP" + to_string(counter);
	string result = "// GT \n";

	result += "@SP\n";
	result += "AM=M-1\n";
	result += "D=M\n";
	result += "A=A-1\n";
	result += "D=M-D\n";
	result += "@" + labelTrue + "\n";
	result += "D;JEQ\n";
	result += "@SP\n";
	result += "A=M-1\n";
	result += "M=0\n";
	result += "@" + labelLoop + "\n";
	result += "0;JMP\n";
	result += "(" + labelTrue + ")\n";
	result += "@SP\n";
	result += "A=M-1\n";
	result += "M=-1\n";
	result += "(" + labelLoop + ")\n";
	counter++;
	return result;
}

/** Generate Hack Assembly code for a VM gt operation */
string VMTranslator::vm_gt(){

    //setup labels for the equalise operation
	string labelTrue = "TRUE" + to_string(counter);
	string labelLoop = "LOOP" + to_string(counter);
	string result = "// GT \n";

	result += "@SP\n";
	result += "AM=M-1\n";
	result += "D=M\n";
	result += "A=A-1\n";
	result += "D=M-D\n";
	result += "@" + labelTrue + "\n";
	result += "D;JGT\n";
	result += "@SP\n";
	result += "A=M-1\n";
	result += "M=0\n";
	result += "@" + labelLoop + "\n";
	result += "0;JMP\n";
	result += "(" + labelTrue + ")\n";
	result += "@SP\n";
	result += "A=M-1\n";
	result += "M=-1\n";
	result += "(" + labelLoop + ")\n";
    ++counter;
    return result;
}

/** Generate Hack Assembly code for a VM lt operation */
string VMTranslator::vm_lt(){
	string labelTrue = "TRUE" + to_string(counter);
	string labelLoop = "LOOP" + to_string(counter);
	string result = "// GT \n";

	result += "@SP\n";
	result += "AM=M-1\n";
	result += "D=M\n";
	result += "A=A-1\n";
	result += "D=M-D\n";
	result += "@" + labelTrue + "\n";
	result += "D;JLT\n";
	result += "@SP\n";
	result += "A=M-1\n";
	result += "M=0\n";
	result += "@" + labelLoop + "\n";
	result += "0;JMP\n";
	result += "(" + labelTrue + ")\n";
	result += "@SP\n";
	result += "A=M-1\n";
	result += "M=-1\n";
	result += "(" + labelLoop + ")\n";
	counter++;
	return result;
}

/** Generate Hack Assembly code for a VM and operation */
string VMTranslator::vm_and(){
	string result = "// AND\n";
	result += "@SP\n";
	result += "AM=M-1\n";
	result += "D=M\n";
	result += "A=A-1\n";
	result += "M=M&D\n";
	return result;
}

/** Generate Hack Assembly code for a VM or operation */
string VMTranslator::vm_or(){
	string result = "// OR\n";
	result += "@SP\n";
	result += "AM=M-1\n";
	result += "D=M\n";
	result += "A=A-1\n";
	result += "M=M|D\n";
	return result;
}

/** Generate Hack Assembly code for a VM not operation */
string VMTranslator::vm_not(){
	string result = "// Neg\n";
	result += "@SP\n";
	result += "A=M\n";
	result += "A=A-1\n";
	result += "M=!M\n";
	return result;
}

/** Generate Hack Assembly code for a VM label operation */
string VMTranslator::vm_label(string label){
    return "(" + label + ")\n";
}

/** Generate Hack Assembly code for a VM goto operation */
string VMTranslator::vm_goto(string label){
    return "@" + label + "\n0;JMP\n";
}

/** Generate Hack Assembly code for a VM if-goto operation */
string VMTranslator::vm_if(string label){
	string result = "// IF-GOTO\n";
	result += "@SP\n";
	result += "AM=M-1\n";
	result += "D=M\n";
	result += "@" + label + "\nD;JNE\n";
	return result;
}

/** Generate Hack Assembly code for a VM function operation */
string VMTranslator::vm_function(string function_name, int n_vars){
	string result = "// FUNCTION \n";
	result += "(" + function_name + ")\n";
	for (int i = 0; i < n_vars; i++)
	{
		result += "@SP\n";
		result += "AM=M+1\n";
		result += "A=A-1\n";
		result += "M=0\n";
	}
	return result;
}

/** Generate Hack Assembly code for a VM call operation */
string VMTranslator::vm_call(string function_name, int n_args){
	string result = "// CALL \n";
	//push return address
	result += "@retAddr\n";
	result += "D=A\n";
	result += "@SP\n";
	result += "AM=M+1\n";
	result += "A=A-1\n";
	result += "M=D\n";

	result += "@LCL\n";
	result += "D=M\n";
	result += "@SP\n";
	result += "AM=M+1\n";
	result += "A=A-1\n";
	result += "M=D\n";

	result += "@ARG\n";
	result += "D=M\n";
	result += "@SP\n";
	result += "AM=M+1\n";
	result += "A=A-1\n";
	result += "M=D\n";

	result += "@THIS\n";
	result += "D=M\n";
	result += "@SP\n";
	result += "AM=M+1\n";
	result += "A=A-1\n";
	result += "M=D\n";

	result += "@THAT\n";
	result += "D=M\n";
	result += "@SP\n";
	result += "AM=M+1\n";
	result += "A=A-1\n";
	result += "M=D\n";

	result += "@SP\n";
	result += "D=M\n";
	result += "@5\n";
	result += "D=D-A\n";
	result += "@" + to_string(n_args) + "\n";
	result += "D=D-A\n";
	result += "@ARG\n";
	result += "M=D\n";

	result += "@SP\n";
	result += "D=M\n";
	result += "@LCL\n";
	result += "M=D\n";

	result += "@" + function_name + "\n";
	result += "0;JMP\n";
	result += "(retAddr)\n";

	return result;
}

/** Generate Hack Assembly code for a VM return operation */
string VMTranslator::vm_return() {
	string result = "// RETURN\n";
	result += "@LCL\n";
	result += "D=M\n";
	result += "@R13\n"; 
	result += "M=D\n";

	result += "D=M\n";
	result += "@5\n";
	result += "A=D-A\n";
	result += "D=M\n";
	result += "@R14\n";
	result += "M=D\n";

	result += "@SP\n";
	result += "M=M-1\n";
    result += "A = M\n";
	result += "D=M\n";
	result += "@ARG\n";
	result += "A=M\n";
	result += "M=D\n";

	result += "@ARG\n";
	result += "D=M+1\n";
	result += "@SP\n";
	result += "M=D\n";

	result += "@R13\n";
	result += "D=M\n";
	result += "@1\n";
	result += "A=D-A\n";
	result += "D=M\n";
	result += "@THAT\n";
	result += "M=D\n";

	result += "@R13\n";
	result += "D=M\n";
	result += "@2\n";
	result += "A=D-A\n";
	result += "D=M\n";
	result += "@THIS\n";
	result += "M=D\n";

	result += "@R13\n";
	result += "D=M\n";
	result += "@3\n";
	result += "A=D-A\n";
	result += "D=M\n";
	result += "@ARG\n";
	result += "M=D\n";

	result += "@R13\n";
	result += "D=M\n";
	result += "@4\n";
	result += "A=D-A\n";
	result += "D=M\n";
	result += "@LCL\n";
	result += "M=D\n";

	result += "@R14\n";
	result += "A=M\n";
	result += "0;JMP\n"; 
    return result;}