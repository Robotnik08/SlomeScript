#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <float.h>
#include <cmath>
#include <algorithm>

using namespace std;

//define error codes
#define SYNTAX_ERROR 1
#define TOO_MANY_ARGS 2
#define EMPTY_LINE 3
#define NOT_AN_INT 4
#define INT32_OVERFLOW 5
#define INVALID_VARIABLE_NAME 6
#define VAR_NOT_FOUND 7
#define INVALID_DOUBLE 8
#define INVALID_BOOL 9
#define SKIP_LOCATION_NOT_FOUND 10
#define TYPE_MISMATCH 11
#define BOOL_OPERATIONS_ERROR 12
#define ONLY_VAR_NAMES_ACCEPTED 13
#define VAR_MUST_BE_NUMBER 14
#define FUNC_BEING_SEARCHED 15
#define FUNC_END_FOUND_WITHOUT_PARENT 16
#define TOO_FEW_ARGS 17
#define INVALID_FUNC_NAME 18
#define FUNC_NOT_FOUND 19
const string ERROR_MESSAGES[] = {"SyntaxError at line: ",
                                 "Too many arguments at line: ",
                                 "Empty at line: ",
                                 "Argument is not an interger at line: ",
                                 "Int overflow at line: ",
                                 "Variable names can only include letters, at line: ",
                                 "Variable not found at: ",
                                 "Double not valid (Too big, or invalid input) at line: ",
                                 "Value must be either \"true\" or \"false\" at line: ",
                                 "Skip Location was not found at line: ",
                                 "Arguments must be of the same type at line: ",
                                 "Incorrect comparison operator for boolean at line: ",
                                 "Only variables are allowed or variable name not found in bool parsing at line: ",
                                 "Variable must be a number at line: ",
                                 "A function is declared before the last function has found its end at line: ",
                                 "An ENDFUNC has been found but no starting point at line: ",
                                 "Too few arguments at line: ",
                                 "Function names can only include letters, at line: ",
                                 "Function not found at: "
                                };


//Variable ID's
#define VAL_INTERGER 0
#define VAL_DOUBLE 1
#define VAL_BOOL 2
#define VAL_STRING 3

#define VAL_VOID -1

//logical operators
const string COMPARISON_OPERATORS[] = {"==","!=",">","<",">=","<="};
#define EQUAL_TOO 0
#define NOT_EQUAL_TOO 1
#define GREATER_THAN 2
#define LESS_THAN 3
#define GREATER_THAN_EQUAL_TOO 4
#define LESS_THAN_EQUAL_TOO 5

//declare full scope
class token;
class function;
bool parseLine (string l, int location, bool isFunc);
bool checkIfInt(string str);
string trimSpace (string c);
bool checkIfdouble(string str);
bool strOverflow (string str);
bool strOverflowDouble (string str);
bool checkIfAlphaBetic (string str);
bool checkIfSurroundedBy (string str, char character);
string trimString(string str);
string prepareLog(string out);
void throwError (int code, int line);
vector<string> splitString(string content, string delimiter);
token* lookupVar (string name);
function* lookupFunction (string name);
int searchSkipLocation (string name, int line);
bool parseToBoolean (string str);
bool getBooleanFromString (string str);
vector<string> splitStringMultiple (string content, string del1, string del2);
double returnMath (string str);
vector<token*> parseToRawArgs (string str);

//global variables
vector<string> mainScript;
int reader = 0;
vector<int> intergers;
vector<double> doubles;
vector<string> strings;
vector<int8_t> booleans;
vector<token*> vars;
vector<function*> funcs;

//declare classes and functions
class token {
    public:
        int type;
        int* Vint;
        double* Vdouble;
        string* Vstring;
        int8_t* Vbool;
        string name;
        token(int _type, string _name, int valueInt = 0, double valuedouble = 0, string valueString = "", int8_t valueBool = 0) {
            type = _type;
            name = _name;
            if (!(_type ^ VAL_INTERGER)) {
                intergers.push_back(valueInt);
                Vint = &intergers[intergers.size()-1];
            } else if (!(_type ^ VAL_DOUBLE)) {
                doubles.push_back(valuedouble);
                Vdouble = &doubles[doubles.size()-1];
            } else if (!(_type ^ VAL_STRING)) {
                strings.push_back(valueString);
                Vstring = &strings[strings.size()-1];
            } else if (!(_type ^ VAL_BOOL)) {
                booleans.push_back(valueBool);
                Vbool = &booleans[booleans.size()-1];
            }
        }
        int returnInt() {
            return *Vint;
        }
        double returnDouble() {
            return *Vdouble;
        }
        string returnString() {
            return *Vstring;
        }
        bool returnBool() {
            return *Vbool != 0;
        }
        void freeItSelf() {
            free(this);
        }
};
class function {
    public:
        string name;
        vector<string> code;
        int returnType;
        string para;
        vector<token*> parameters;
        function (int _returnType, string _name, string _para, vector<string> _code) {
            returnType = _returnType;
            name = _name;
            para = _para;
            code = _code;
        }
        void runcodeVoid (int location, vector<token*> parameters) {
            int mini_reader;
            for (mini_reader = 0; mini_reader < code.size(); mini_reader++) {
                if (!parseLine(code[mini_reader], location, true)) {
                    break;
                }
            }
        }
        int runcodeInt (int location, vector<token*> parameters) {
            int mini_reader;
            for (mini_reader = 0; mini_reader < code.size(); mini_reader++) {
                if (!parseLine(code[mini_reader], location, true)) {
                    break;
                }
            }
            string returnValue = splitString(code[mini_reader], " ")[2];
            token* val = lookupVar(returnValue);
            if (!val) {
                throwError(VAR_NOT_FOUND, location);
            }
            if (!(val->type ^ VAL_INTERGER)) {
                return val->returnInt();
            } else {
                throwError(TYPE_MISMATCH, location);
            }
        }
        double runcodeDouble (int location, vector<token*> parameters) {
            int mini_reader;
            for (mini_reader = 0; mini_reader < code.size(); mini_reader++) {
                if (!parseLine(code[mini_reader], location, true)) {
                    break;
                }
            }
            string returnValue = splitString(code[mini_reader], " ")[2];
            token* val = lookupVar(returnValue);
            if (!val) {
                throwError(VAR_NOT_FOUND, location);
            }
            if (!(val->type ^ VAL_DOUBLE)) {
                return val->returnDouble();
            } else {
                throwError(TYPE_MISMATCH, location);
            }
        }
        string runcodeString (int location, vector<token*> parameters) {
            int mini_reader;
            for (mini_reader = 0; mini_reader < code.size(); mini_reader++) {
                if (!parseLine(code[mini_reader], location, true)) {
                    break;
                }
            }
            string returnValue = splitString(code[mini_reader], " ")[2];
            token* val = lookupVar(returnValue);
            if (!val) {
                throwError(VAR_NOT_FOUND, location);
            }
            if (!(val->type ^ VAL_STRING)) {
                return val->returnString();
            } else {
                throwError(TYPE_MISMATCH, location);
            }
        }
        bool runcodeBool (int location, vector<token*> parameters) {
            int mini_reader;
            for (mini_reader = 0; mini_reader < code.size(); mini_reader++) {
                if (!parseLine(code[mini_reader], location, true)) {
                    break;
                }
            }
            string returnValue = splitString(code[mini_reader], " ")[2];
            token* val = lookupVar(returnValue);
            if (!val) {
                throwError(VAR_NOT_FOUND, location);
            }
            if (!(val->type ^ VAL_BOOL)) {
                return val->returnBool();
            } else {
                throwError(TYPE_MISMATCH, location);
            }
        }
};
bool checkIfInt(string str) {
    if (!isdigit(str[0]) && str[0] != '-') {
        return false;
    }
    for (int i = 1; i < str.length(); i++) {
        if (!isdigit(str[i])) {
            return false;
        }
    }
    return true;
}
string trimSpace (string c) {
    int count = 0;
    while (c[count] == ' ') {
        count++;
    }
    c.erase(0,count);
    return c;
}
bool checkIfdouble(string str) {
    if (!isdigit(str[0]) && str[0] != '-') {
        return false;
    }
    for (int i = 1; i < str.length(); i++) {
        if (!isdigit(str[i]) && str[i] != '.') {
            return false;
        }
    }
    return true;
}
bool strOverflow (string str) {
    string max = to_string(INT32_MAX);
    string min = to_string(INT32_MIN);
    if (str[0] == '-') {
        if (str.size() > min.size()) {
            return true;
        } else if (!(str.size() ^ min.size())) {
            for (int i = 1; i < str.size(); i++) {
                string s(1, str[i]);
                string m(1, min[i]);
                if (stoi(s) > stoi(m)) {
                    return true;
                } else if (stoi(s) < stoi(m)) {
                    return false;
                }
            }
        }
    } else {
        if (str.size() > max.size()) {
            return true;
        } else if (!(str.size() ^ max.size())) {
            for (int i = 0; i < str.size(); i++) {
                string s(1, str[i]);
                string m(1, max[i]);
                if (stoi(s) > stoi(m)) {
                    return true;
                } else if (stoi(s) < stoi(m)) {
                    return false;
                }
            }
        }
    }
    return false;
}
bool strOverflowDouble (string str) {
    string max = to_string(INT64_MAX);
    string min = to_string(INT64_MIN);
    if (str[0] == '-') {
        if (str.size() > min.size()) {
            return true;
        } else if (!(str.size() ^ min.size())) {
            for (int i = 1; i < str.size(); i++) {
                string s(1, str[i]);
                string m(1, min[i]);
                if (stoi(s) > stoi(m)) {
                    return true;
                } else if (stoi(s) < stoi(m)) {
                    return false;
                }
            }
        }
    } else {
        if (str.size() > max.size()) {
            return true;
        } else if (!(str.size() ^ max.size())) {
            for (int i = 0; i < str.size(); i++) {
                string s(1, str[i]);
                string m(1, max[i]);
                if (stoi(s) > stoi(m)) {
                    return true;
                } else if (stoi(s) < stoi(m)) {
                    return false;
                }
            }
        }
    }
    return false;
}
bool checkIfAlphaBetic (string str) {
    for (int i = 0; i < str.size(); i++) {
        if (!isalpha(str[i])) {
            return false;
        }
    }
    return true;
}
bool checkIfSurroundedBy (string str, char character) {
    return str[0] == character && str[str.size()-1] == character;
}
string trimString(string str) {
    str.erase(0,1);
    str.erase(str.size()-1);
    return str;
}
string prepareLog(string out) {
    while ((out.find("\\s")) != string::npos) {
        out = out.replace(out.find("\\s"), sizeof("\\s") - 1, " ");
    }
    while ((out.find("\\n")) != string::npos) {
        out = out.replace(out.find("\\n"), sizeof("\\n") - 1, "\n");
    }
    return out;
}
void throwError (int code, int line) {
    cout << ERROR_MESSAGES[code-1] << line + 1;
    exit(code);
}
vector<string> splitString(string content, string delimiter) {
    vector<string> res; 

    size_t pos = 0;
    string t;
    while ((pos = content.find(delimiter)) != string::npos) {
        t = content.substr(0, pos);
        res.push_back(t);
        content.erase(0, pos + delimiter.length());
    }
    res.push_back(content);
    return res;
}
token* lookupVar (string name) {
    for (int i = 0; i < vars.size(); i++) {
        if (vars[i]->name == name) {
            return vars[i];
        }
    }
    return NULL;
}
function* lookupFunction (string name) {
    for (int i = 0; i < funcs.size(); i++) {
        if (funcs[i]->name == name) {
            return funcs[i];
        }
    }
    return NULL;
}
int searchSkipLocation (string name, int line) {
    for (int i = line; i < mainScript.size(); i++) {
        vector<string> script = splitString(mainScript[i], " ");
        if (script[0] == "SKIP") {
            if (script[2] == name) {
                return i;
            }
        }
    }
    for (int i = 0; i < line; i++) {
        vector<string> script = splitString(mainScript[i], " ");
        if (script[0] == "SKIP") {
            if (script[2] == name) {
                return i;
            }
        }
    }
    throwError(SKIP_LOCATION_NOT_FOUND, line);
}
bool parseToBoolean (string str) {
    for (int i = 0; i < 6; i++) {
        if (str.find(COMPARISON_OPERATORS[i]) != string::npos) {
            vector<string> vals = splitString(str, COMPARISON_OPERATORS[i]);
            token* val1 = lookupVar(vals[0]);
            token* val2 = lookupVar(vals[1]);
            if (!val1 || !val2) {
                throwError(ONLY_VAR_NAMES_ACCEPTED, reader);
            }
            if (val1->type != val2->type) {
                throwError(TYPE_MISMATCH, reader);
            }
            if (!(i ^ EQUAL_TOO)) {
                if (!(val1->type ^ VAL_INTERGER)) {
                    return !(val1->returnInt() ^ val2->returnInt());
                } else if (!(val1->type ^ VAL_DOUBLE)) {
                    return val1->returnDouble() == val2->returnDouble();
                } else if (!(val1->type ^ VAL_STRING)) {
                    return val1->returnString() == val2->returnString();
                } else if (!(val1->type ^ VAL_BOOL)) {
                    return val1->returnBool() == val1->returnBool();
                }
            }
            if (!(i ^ NOT_EQUAL_TOO)) {
                if (!(val1->type ^ VAL_INTERGER)) {
                    return val1->returnInt() ^ val2->returnInt();
                } else if (!(val1->type ^ VAL_DOUBLE)) {
                    return val1->returnDouble() != val2->returnDouble();
                } else if (!(val1->type ^ VAL_STRING)) {
                    return val1->returnString() != val2->returnString();
                } else if (!(val1->type ^ VAL_BOOL)) {
                    return val1->returnBool() != val1->returnBool();
                }
            }
            if (!(i ^ GREATER_THAN)) {
                if (!(val1->type ^ VAL_INTERGER)) {
                    return val1->returnInt() > val2->returnInt();
                } else if (!(val1->type ^ VAL_DOUBLE)) {
                    return val1->returnDouble() > val2->returnDouble();
                } else if (!(val1->type ^ VAL_STRING)) {
                    return val1->returnString().size() > val2->returnString().size();
                } else if (!(val1->type ^ VAL_BOOL)) {
                    throwError(BOOL_OPERATIONS_ERROR, reader);
                }
            }
            if (!(i ^ LESS_THAN)) {
                if (!(val1->type ^ VAL_INTERGER)) {
                    return val1->returnInt() < val2->returnInt();
                } else if (!(val1->type ^ VAL_DOUBLE)) {
                    return val1->returnDouble() < val2->returnDouble();
                } else if (!(val1->type ^ VAL_STRING)) {
                    return val1->returnString().size() < val2->returnString().size();
                } else if (!(val1->type ^ VAL_BOOL)) {
                    throwError(BOOL_OPERATIONS_ERROR, reader);
                }
            }
            if (!(i ^ GREATER_THAN_EQUAL_TOO)) {
                if (!(val1->type ^ VAL_INTERGER)) {
                    return val1->returnInt() >= val2->returnInt();
                } else if (!(val1->type ^ VAL_DOUBLE)) {
                    return val1->returnDouble() >= val2->returnDouble();
                } else if (!(val1->type ^ VAL_STRING)) {
                    return val1->returnString().size() >= val2->returnString().size();
                } else if (!(val1->type ^ VAL_BOOL)) {
                    throwError(BOOL_OPERATIONS_ERROR, reader);
                }
            }
            if (!(i ^ LESS_THAN_EQUAL_TOO)) {
                if (!(val1->type ^ VAL_INTERGER)) {
                    return val1->returnInt() <= val2->returnInt();
                } else if (!(val1->type ^ VAL_DOUBLE)) {
                    return val1->returnDouble() <= val2->returnDouble();
                } else if (!(val1->type ^ VAL_STRING)) {
                    return val1->returnString().size() <= val2->returnString().size();
                } else if (!(val1->type ^ VAL_BOOL)) {
                    throwError(BOOL_OPERATIONS_ERROR, reader);
                }
            }
        }
    }
}
bool getBooleanFromString (string str) {
    vector<string> ands = splitString(str, "&&");
    for (int i = 0; i < ands.size(); i++) {
        vector<string> ors = splitString(ands[i], "||");
        for (int j = 0; j < ors.size(); j++) {
            if (ors[j][0] == '!') {
                ors[j].erase(0,1);
                if (!parseToBoolean(ors[j])) {
                    j = ands.size();
                    break;
                }
            } else if (parseToBoolean(ors[j])) {
                j = ands.size();
                break;
            }
            return false;
        }
    }
    return true;
}
vector<string> splitStringMultiple (string content, string del1, string del2) {
    vector<string> res; 

    size_t pos = 0;
    string t;
    while (content.find(del1) != string::npos || content.find(del2) != string::npos) {
        pos = min(content.find(del1) != string::npos ? content.find(del1) : INT32_MAX, content.find(del2) != string::npos ? content.find(del2) : INT32_MAX);
        t = content.substr(0, pos);
        res.push_back(t);
        content.erase(0, pos + 1);
    }
    res.push_back(content);
    return res;

}
double returnMath (string str) {
    str = trimString(str);
    vector<string> finalOperations = splitStringMultiple(str, "-", "+");
    double finalResult = 0;
    int operatorindexMain = 0;
    for (int i = 0; i < finalOperations.size(); i++) {
        double result;
        int operatorindex = 0;
        vector<string> multi = splitStringMultiple(finalOperations[i], "*", "/");
        for (int j = 0; j < multi.size(); j++) {
            if (checkIfAlphaBetic(multi[j])) {
                token* val = lookupVar(multi[j]);
                if (!val) {
                    throwError(VAR_NOT_FOUND, reader);  
                } else {
                    if (!j) {
                        if (!((val->type ^ VAL_INTERGER))) {
                            result = val->returnInt();
                        } else if (!(val->type ^ VAL_DOUBLE)) {
                            result = val->returnDouble();
                        } else {
                            throwError(VAR_MUST_BE_NUMBER, reader);
                        }
                    } else {
                        if (!((val->type ^ VAL_INTERGER))) {
                            result = finalOperations[i][operatorindex] == '*' ? result * val->returnInt() : result / val->returnInt();
                        } else if (!(val->type ^ VAL_DOUBLE)) {
                            result = finalOperations[i][operatorindex] == '*' ? result * val->returnDouble() : result / val->returnDouble();
                        } else {
                            throwError(TYPE_MISMATCH, reader);
                        }
                    }
                }
            } else if (checkIfdouble(multi[j])) {
                if (!j) {
                    result = stod(multi[j]);
                } else {
                    result = finalOperations[i][operatorindex] == '*' ? result * stod(multi[j]) : result / stod(multi[j]);
                }
            } else {
                throwError(VAR_NOT_FOUND, reader);
            }
            operatorindex += multi[j].size();
        }
        if (!i) {
            finalResult = result;
        } else {
            finalResult = str[operatorindexMain] == '+' ? finalResult + result : finalResult - result;
        }
        operatorindexMain += finalOperations[i].size();
    }
    return finalResult;
}
vector<token*> parseToRawArgs (string str) {
    vector<string> vals = splitString(trimString(str), ",");
    vector<token*> res;
    if (trimString(str) == "") {
        return res;
    }
    for (int i = 0; i < vals.size(); i++) {
        
        token* val = lookupVar(vals[i]);
        if (!val) {
            throwError(VAR_NOT_FOUND, reader);
        }
        res.push_back(val);
    }
    return res;
}
bool parseLine (string l, int location, bool isFunc = false) {
    vector<string> script = splitString(l, " ");
    if (!script.size()) {
        throwError(EMPTY_LINE, location);
    }
    if (script[0] == "DO") {
        if (checkIfSurroundedBy(script[1], '"')) {
            if (checkIfAlphaBetic(trimString(script[1]))) {
                if (!checkIfSurroundedBy(script[2], ':')) {
                    throwError(SYNTAX_ERROR, location);
                }
                function* func = lookupFunction(trimString(script[1]));
                if (!func) {
                    throwError(FUNC_NOT_FOUND, location);
                }
                if (func->returnType ^ VAL_VOID) {
                    token* val = lookupVar(script[4]);
                    if (!checkIfAlphaBetic(script[4])) {
                        throwError(INVALID_VARIABLE_NAME, location);
                    }
                    if (!val) {
                        throwError(VAR_NOT_FOUND, location);
                    }
                    if (script[3] != "TO") {
                        throwError(SYNTAX_ERROR, location);
                    }
                    if (script.size() < 5) {
                        throwError(TOO_FEW_ARGS, location);
                    } else if (script.size() > 5) {
                        throwError(TOO_MANY_ARGS, location);
                    }
                    if (!(func->returnType ^ VAL_INTERGER)) {
                        func->runcodeInt(location, parseToRawArgs(script[2]));
                    } else if (!(func->returnType ^ VAL_DOUBLE)) {
                        func->runcodeDouble(location, parseToRawArgs(script[2]));
                    } else if (!(func->returnType ^ VAL_STRING)) {
                        func->runcodeString(location, parseToRawArgs(script[2]));
                    } else if (!(func->returnType ^ VAL_BOOL)) {
                        func->runcodeBool(location, parseToRawArgs(script[2]));
                    }
                } else {
                    if (script.size() < 3) {
                        throwError(TOO_FEW_ARGS, location);
                    } else if (script.size() > 3) {
                        throwError(TOO_MANY_ARGS, location);
                    }
                    func->runcodeVoid(location, parseToRawArgs(script[2]));
                }
            } else {
                throwError(INVALID_FUNC_NAME, location);
            }
        } else if (script[1] == "SAY") {
            if (script.size() > 3) {
                throwError(TOO_MANY_ARGS, location);
            }
            if (checkIfSurroundedBy(script[2], '"')) {
                cout << prepareLog(trimString(script[2]));
            } else if (checkIfSurroundedBy(script[2], '@')) {
                cout << (getBooleanFromString(trimString(script[2])) ? "true" : "false");
            } else {
                token* val = lookupVar(script[2]);
                if (!val) {
                    throwError(VAR_NOT_FOUND, location);
                } else if (!(val->type ^ VAL_INTERGER)) {
                    cout << val->returnInt();
                } else if (!(val->type ^ VAL_DOUBLE)) {
                    cout << val->returnDouble();
                } else if (!(val->type ^ VAL_STRING)) {
                    cout << prepareLog(val->returnString());
                } else if (!(val->type ^ VAL_BOOL)) {
                    cout << (val->returnBool() ? "true" : "false");
                }
            }
        } else if (script[1] == "PAUSE") {
            if (script.size() > 2) {
                throwError(TOO_MANY_ARGS, location);
            }
            system("PAUSE");
        } else if (script[1] == "SET") {
            if (script.size() > 4) {
                throwError(TOO_MANY_ARGS, location);
            }
            if (checkIfAlphaBetic(script[2])) {
                token* val = lookupVar(script[2]);
                if (!val) {
                    throwError(VAR_NOT_FOUND, location);
                } else if (!(val->type ^ VAL_INTERGER)) {
                    if (checkIfSurroundedBy(script[3], ':')) {
                        if (checkIfInt(script[3])) {
                            if (strOverflow(script[3])) {
                                throwError(INT32_OVERFLOW, location);
                            }
                        } else {
                            *val->Vint = returnMath(script[3]);
                        }
                    } else if (checkIfAlphaBetic(script[3])) {
                        token* val1 = lookupVar(script[3]);
                        if (!val1) {
                            throwError(VAR_NOT_FOUND, location);
                        }
                        *val->Vint = val1->returnInt();
                    } else {
                        throwError(VAR_NOT_FOUND, location);
                    }
                } else if (!(val->type ^ VAL_DOUBLE)) {
                    if (checkIfSurroundedBy(script[3], ':')) {
                        if (checkIfInt(script[3])) {
                            if (strOverflow(script[3])) {
                                throwError(INT32_OVERFLOW, location);
                            }
                        } else {
                            *val->Vdouble = returnMath(script[3]);
                        }
                    } else if (checkIfAlphaBetic(script[3])) {
                        token* val1 = lookupVar(script[3]);
                        if (!val1) {
                            throwError(VAR_NOT_FOUND, location);
                        }
                        *val->Vdouble = val1->returnDouble();
                    } else {
                        throwError(VAR_NOT_FOUND, location);
                    }
                } else if (!(val->type ^ VAL_STRING)) {
                    if (checkIfAlphaBetic(script[3])) {
                        token* val1 = lookupVar(script[3]);
                        if (!val1) {
                            throwError(VAR_NOT_FOUND, location);
                        }
                        *val->Vstring = val1->returnString();
                    } else if (checkIfSurroundedBy(script[3], '"')) {
                        *val->Vstring = trimString(script[3]);
                    } else {
                        throwError(SYNTAX_ERROR, location);
                    }
                } else if (!(val->type ^ VAL_BOOL)) {
                    if (script[3] == "true" || script[3] == "false") {
                        *val->Vbool = script[3][0] == 't' ? true : false;
                    } else if (checkIfAlphaBetic(script[3])) {
                        token* val1 = lookupVar(script[3]);
                        if (!val1) {
                            throwError(VAR_NOT_FOUND, location);
                        }
                        *val->Vbool = val1->returnBool();
                    } else if (checkIfSurroundedBy(script[3], '@')) {
                        *val->Vbool = getBooleanFromString(trimString(script[3]));
                    } else {
                        throwError(SYNTAX_ERROR, location);
                    }  
                }
            } else {
                throwError(VAR_NOT_FOUND, location);
            }
        } else if (script[1] == "RETURN" && isFunc) {
            return false;
        } else {
            throwError(SYNTAX_ERROR, location);
        }
    } else if (script[0] == "MAKE") {
        if (script[1] == "INT") {
            if (script.size() > 4) {
                throwError(TOO_MANY_ARGS, location);
            }
            if (checkIfAlphaBetic(script[2])) {
                if (checkIfInt(script[3])) {
                    if (!strOverflow(script[3])) {
                        vars.push_back(new token(VAL_INTERGER, script[2], stoi(script[3])));
                    }
                    else {
                        throwError(INT32_OVERFLOW, location);
                    }
                } else {
                    throwError(NOT_AN_INT, location);
                }
            } else {
                throwError(INVALID_VARIABLE_NAME, location);
            }
        } else if (script[1] == "DOUBLE" || script[1] == "NUMBER") {
            if (script.size() > 4) {
                throwError(TOO_MANY_ARGS, location);
            }
            if (checkIfAlphaBetic(script[2])) {
                if (checkIfdouble(script[3])) {
                    vars.push_back(new token(VAL_DOUBLE, script[2], 0, stod(script[3])));
                }
            } else {
                throwError(INVALID_VARIABLE_NAME, location);
            }
        } else if (script[1] == "STRING") {
            if (script.size() > 4) {
                throwError(TOO_MANY_ARGS, location);
            }
            if (checkIfAlphaBetic(script[2])) {
                if (checkIfSurroundedBy(script[3], '"')) {
                    vars.push_back(new token(VAL_STRING, script[2], 0, 0, trimString(script[3])));
                } else {
                    throwError(SYNTAX_ERROR, location);  
                }
            } else {
                throwError(INVALID_VARIABLE_NAME, location);
            }
        } else if (script[1] == "BOOL") {
            if (script.size() > 4) {
                throwError(TOO_MANY_ARGS, location);
            }
            if (checkIfAlphaBetic(script[2])) {
                if (script[3] == "true" || script[3] == "false") {
                    vars.push_back(new token(VAL_BOOL, script[2], 0, 0, "", script[3][0] == 't' ? 1 : 0));
                } else {
                    throwError(INVALID_BOOL, location);
                }
            } else {
                throwError(INVALID_VARIABLE_NAME, location);
            }
        } else {
            throwError(SYNTAX_ERROR, location);
        }
    } else if (script[0] == "SKIP") {
        if (script[1] == "TO") {
            if (checkIfSurroundedBy(script[2], '"')) {
                if (checkIfAlphaBetic(trimString(script[2]))) {
                    if (script.size() > 3) {
                        if (script.size() > 5) {
                            throwError(TOO_MANY_ARGS, location);  
                        }
                        if (script[3] == "WHEN") {
                            if (checkIfSurroundedBy(script[4], '@')) {
                                if (getBooleanFromString(trimString(script[4]))) {
                                    reader = searchSkipLocation(trimString(script[2]), location);
                                }
                                return true;
                            } else {
                                throwError(SYNTAX_ERROR, location);
                            }
                        } else if (script[3] == "UNLESS") {
                            if (checkIfSurroundedBy(script[4], '@')) {
                                if (!getBooleanFromString(trimString(script[4]))) {
                                    reader = searchSkipLocation(trimString(script[2]), location);
                                }
                                return true;
                            } else {
                                throwError(SYNTAX_ERROR, location);
                            }
                        } else if (checkIfSurroundedBy(script[3], ':')) {
                            if (!getBooleanFromString(trimString(script[4]))) {
                                reader = searchSkipLocation(trimString(script[2]), location);
                            }
                            return true;
                        } else {
                            throwError(SYNTAX_ERROR, location);
                        }
                    }
                    else {
                        reader = searchSkipLocation(trimString(script[2]), location);
                    }

                } else {
                    throwError(SYNTAX_ERROR, location);
                }
            } else {
                throwError(SYNTAX_ERROR, location);
            }
        } else if (script[1] == "LOCATION") {

        } else {
            throwError(SYNTAX_ERROR, location);
        }
    } else if (script[0] == ">>") {
        //isComment
    } else if (script[0] == "END") {
        if (checkIfInt(script[1])) {
            if (strOverflow(script[1])) {
                throwError(INT32_OVERFLOW, location);
            }
            exit(stoi(script[1]));
        } else {
            throwError(NOT_AN_INT, location);
        }
    } else {
        throwError(SYNTAX_ERROR, location);
    }
    return true;
}
//main
int main() {
    string content;
    ifstream file("test.sls");
    string line;

    while (getline (file, line)) {
        content += line;
    }
    file.close();
    mainScript = splitString(content, ";");
    parseLine ("MAKE NUMBER NULL 0", -2);
    parseLine ("MAKE NUMBER NULL 0", -2);
    int latestReturnType = VAL_VOID;
    int foundTarget = -1;
    string latestName = "";
    string latestPara = "";
    for (int i = 0; i < mainScript.size() -1; i++) {
        mainScript[i] = trimSpace(mainScript[i]);
        vector<string> spl = splitString(mainScript[i], " ");
        if (spl[0] == "DEFINE") {
            if (foundTarget >= 0) {
                throwError(FUNC_BEING_SEARCHED, i);
            }
            if (spl[1] == "FUNC") {
                if (spl[2] == "INT") {
                    latestReturnType = VAL_INTERGER;
                } else if (spl[2] == "DOUBLE" || spl[2] == "NUMBER") {
                    latestReturnType = VAL_DOUBLE;
                } else if (spl[2] == "STRING") {
                    latestReturnType = VAL_STRING;
                } else if (spl[2] == "BOOL") {
                    latestReturnType = VAL_BOOL;
                } else if (spl[2] == "VOID") {
                    latestReturnType = VAL_VOID;
                } else {
                    throwError(SYNTAX_ERROR, i);
                }
                if (spl.size() < 5) {
                    throwError(TOO_FEW_ARGS, i);
                }
                latestName = spl[3];
                latestPara = spl[4];
                foundTarget = i;
            } else {
                throwError(SYNTAX_ERROR, i);
            }
        } else if (spl[0] == "ENDFUNC") {
            if (foundTarget < 0) {
                throwError(FUNC_END_FOUND_WITHOUT_PARENT, i);
            }
            vector<string> function_code;
            for (int j = foundTarget + 1; j < i; j++) {
                function_code.push_back(mainScript[j]);
            }
            funcs.push_back(new function(latestReturnType, latestName, latestPara, function_code));
            foundTarget = -1;
        }
    }
    for (reader = 0; reader < mainScript.size() -1; reader++) {
        mainScript[reader] = trimSpace(mainScript[reader]);
        parseLine(mainScript[reader], reader);
    }
}