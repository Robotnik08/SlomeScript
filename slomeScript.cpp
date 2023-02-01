#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <float.h>
#include <cmath>
#include <algorithm>

using namespace std;

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
const string ERROR_MESSAGES[] = {"SyntaxError at line: ",
                                 "Too many arguments at line: ",
                                 "Empty at line: ",
                                 "Argument is not an interger at line: ",
                                 "Int overflow at line: ",
                                 "Variables can only include letters, at line: ",
                                 "Variable not found at: ",
                                 "Double not valid (Too big, or invalid input) at line: ",
                                 "Skip Location was not found at line: ",
                                 "Value must be either \"true\" or \"false\" at line: ",
                                 "Arguments must be of the same type at line: ",
                                 "Incorrect comparison operator for boolean at line: ",
                                 "Only variables are allowed or variable name not found in bool parsing at line: "
                                };

#define VAL_INTERGER 0
#define VAL_DOUBLE 1
#define VAL_BOOL 2
#define VAL_STRING 3

const string COMPARISON_OPERATORS[] = {"==","!=",">","<",">=","<="};
#define EQUAL_TOO 0
#define NOT_EQUAL_TOO 1
#define GREATER_THAN 2
#define LESS_THAN 3
#define GREATER_THAN_EQUAL_TOO 4
#define LESS_THAN_EQUAL_TOO 5

vector<string> mainScript;
int reader = 0;
vector<int> intergers;
vector<double> doubles;
vector<string> strings;
vector<int8_t> booleans;
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
vector<token*> vars;

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
vector<string> splitString(string content, string delimiter)
{
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
void parseLine (string l, int location) {
    vector<string> script = splitString(l, " ");
    if (!script.size()) {
        throwError(EMPTY_LINE, location);
    }
    if (script[0] == "DO") {
        if (script[1] == "SAY") {
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
        } else if (script[1] == "DOUBLE") {
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
                                return;
                            } else {
                                throwError(SYNTAX_ERROR, location);
                            }
                            return;
                        } else if (script[3] == "UNLESS") {
                            if (checkIfSurroundedBy(script[4], '@')) {
                                if (!getBooleanFromString(trimString(script[4]))) {
                                    reader = searchSkipLocation(trimString(script[2]), location);
                                }
                                return;
                            } else {
                                throwError(SYNTAX_ERROR, location);
                            }
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
    } else {
        throwError(SYNTAX_ERROR, location);
    }
}
int main() {
    string content;
    ifstream file("main.sls");
    string line;

    while (getline (file, line)) {
        content += line;
    }
    file.close();
    mainScript = splitString(content, ";");
    for (reader = 0; reader < mainScript.size() -1; reader++) {
        mainScript[reader] = trimSpace(mainScript[reader]);
        parseLine(mainScript[reader], reader);
    }
    return 0;
}