// R bridge header /////////////////////////////////
// 
// Based on the MT4R dll by Bernd Kreuss, distributed under GNU license
// in the Source folder. See RTest.c for example usage.

#ifndef r_h
#define r_h

#ifdef WIN32
#define F(x) (__stdcall *x)
#define API(f,l)
#define PROC(f) *(FARPROC*)&f = GetProcAddress(g->RLib,#f)
#else
#include <default.c>
#define F(x) __stdcall x
#endif

int F(RInit_)(const char* commandline, int debuglevel);
API(RInit_,mt4R)
// Start a new R session, by giving the path to the RTerm executable 
// with command line options. You must use --no-save but you 
// MUST NOT use --slave or otherwise turn off the echo or change 
// the default prompt or it will fail.
// Debug level can be 0, 1 or 2. During development you should set it 
// to 2 which will output every available message. A value of 1 will 
// only output warnings and notes and a value of 0 will only output 
// fatal errors, this is the recommended setting for production use.
// Debug output will go to the system debug monitor. You can use 
// the free DebugView.exe tool from Microsoft to log it in real time.

void F(RDeinit)(int rhandle);
API(RDeinit,mt4R)
// Teminate the R session. Call this in your deinit() function.
// After this the handle is no longer valid.
   
bool F(RIsRunning)(int rhandle);
API(RIsRunning,mt4R)
// Return true if the R session belonging to this handle is 
// still runing. R will terminate on any fatal error in the 
// code you send it. You should check this at the beginning
// of your start function and stop all actions. The last
// command prior to the crash will be found in the log.
// If R is not running anymore this library won't emit any
// more log messages and will silently ignore all commands.
   
bool F(RIsBusy)(int rhandle);
API(RIsBusy,mt4R)
// Return true if R is still executing a command (resulting 
// from a call to RExecuteAsync())
   
void F(RExecuteAsync)(int rhandle, const char* code);
API(RExecuteAsync,mt4R)
// Execute code and do not wait. Any subsequent call however
// will wait since there can only be one thread executing at
// any given time. Use RIsBusy() to check whether it is finished
   
void F(RExecute)(int rhandle, const char* code);
API(RExecute,mt4R)
// Execute code and wait until it is finished. This will not
// return anything. You can basically achieve the same with
// the RGet() functions, evaluating the expression is also
// just executig code, the only difference is that these
// RGet() functions will additionally try to parse and return 
// the output while RExecute() will just execute, wait and 
// ignore all output.
   
void F(RAssignBool)(int rhandle, const char* variable, bool value);
API(RAssignBool,mt4R)
// Assign a bool to the variable name. In R this type is called "logical"
   
void F(RAssignInteger)(int rhandle, const char* variable, int value);
API(RAssignInteger,mt4R)
// Assign an integer to the variable name.
   
void F(RAssignDouble)(int rhandle, const char* variable, double value);
API(RAssignDouble,mt4R)
// Assign a var (double) to the variable name.
   
void F(RAssignString)(int rhandle, const char* variable, const char* value);
API(RAssignString,mt4R)
// Assign a string to the variable name. In R this type is called "character"
   
void F(RAssignVector)(int rhandle, const char* variable, var* vector, int size);
API(RAssignVector,mt4R)
// Assign a var array (called a vector in R) to the variable name. If the size does not match
// your actual array size then bad things might happen.
   
void F(RAssignStringVector)(int rhandle, const char* variable, char* *s, int size);
API(RAssignStringVector,mt4R)
// Assign an array of 16-bit strings to the variable. If you need
// a factor then you should execute code to convert it after this command. In
// recent versions of R a vector of strings does not need any more memory than
// a factor and it is easier to append new elements to it.
   
void F(RAssignMatrix)(int rhandle, const char* variable, var* matrix, int rows, int cols);
API(RAssignMatrix,mt4R)
// Assign a matrix to the variable name. The matrix must have the row number as the
// first dimension (byrow=TRUE will be used on the raw data). This function is much 
// faster than building a huge matrix (hundreds of rows) from scratch by appending 
// new rows at the end with RRowBindVector() for every row. This function is optimized
// for huge throughput with a single function call through using file-IO with the
// raw binary data. For very small matrices and vectors with only a handful of elements 
// this might be too much overhead and the other functions will be faster. Once you 
// have the matrix with possibly thousands of rows transferred to R you should then
// only use RRowBindVector() to further grow it slowly on the arrival of single new 
// data vectors instead of always sending a new copy of the entire matrix.
   
void F(RAppendMatrixRow)(int rhandle, const char* variable, var* vector, int size);
API(RAppendMatrixRow,mt4R)
// Append a row to a matrix or dataframe. This will exexute 
// variable <- rbind(variable, vector)
// if the size does not match the actual array size bad things might happen.
   
bool F(RExists)(int rhandle, const char* variable);
API(RExists,mt4R)
// Return true if the variable exists, false otherwise.
   
bool F(RGetBool)(int rhandle, const char* expression);
API(RGetBool,mt4R)
// Evaluate expression and return a bool. Expression can be any R code 
// that will evaluate to logical. If it is a vector of logical then only
// the first element is returned.
   
int F(RGetInteger)(int rhandle, const char* expression);
API(RGetInteger,mt4R)
// Evaluate expression and return an integer. Expression can be any R code 
// that will evaluate to an integer. If it is a floating point it will be
// rounded, if it is a vector then only the first element will be returned.
   
double F(RGetDouble)(int rhandle, const char* expression);
API(RGetDouble,mt4R)
// Evaluate expression and return a double. Expression can be any R code 
// that will evaluate to a floating point number, if it is a vector then
// only the first element is returned.
   
int F(RGetVector)(int rhandle, const char* expression, var *vector, int size);
API(RGetVector,mt4R)
// Evaluate expression and return a vector of doubles. Expression can
// be anything that evaluates to a vector of floating point numbers.
// Return value is the number of elements that could be copied into the
// array. It will never be bigger than size but might be smaller.
// warnings are output on debuglevel 1 if the sizes don't match.
   
void F(RPrint)(int rhandle, const char* expression);
API(RPrint,mt4R)
// Do a print(expression) for debugging purposes. The output will be 
// sent to the debug monitor on debuglevel 0.

char* F(RLastOutput)(int rhandle);
API(RLastOutput,mt4R)
// last known raw output of the session (use this when logging R crash)

///////////////////////////////////////////////////////////////////
// shorthands for some of the above functions

inline char* slash(const char* name) { return strxc(name,'\\','/'); }

#define hR g->RHandle

int Rx(const char* code,int mode) 
{ 
	if(mode == 0)
		RExecute(hR,code); 
	else if(mode == 1)
		RExecuteAsync(hR,code);
	else if(mode >= 2) {
		if(!wait(0)) return 0; // Stop hit
		RExecuteAsync(hR,code);
		while(RIsRunning(hR) && RIsBusy(hR))
			if(!wait(20)) return 0;
		if(RIsRunning(hR)) {
			if(mode >= 3) {
				string s = RLastOutput(hR);
				if(s) if(*s) print(TO_ANY,"\n%s",s);
			}
			return 1;
		}
		print(TO_WINDOW,"R error - %s",RLastOutput(hR));
		quit("");
		hR = 0;
		return 0;
	} 	
	return 1;
}

inline int Rx(const char* code) { return Rx(code,0); }

int Rstart(const char* source,int debug) 
{ 
	if(hR) return hR; // R session already running
#ifdef WIN32
	g->RLib = LoadLibrary("mt4R.dll");
	PROC(RInit_);
	PROC(RIsRunning);
	PROC(RIsBusy);
	PROC(RExecute);
	PROC(RExecuteAsync);
	PROC(RAssignString);
	PROC(RAssignInteger);
	PROC(RAssignDouble);
	PROC(RAssignVector);
	PROC(RAssignMatrix);
	PROC(RGetInteger);
	PROC(RGetDouble);
	PROC(RGetVector);
	PROC(RPrint);
	PROC(RLastOutput);
	PROC(RDeinit);
#endif
	hR = RInit_(strf("%s  --no-save",g->sRTermPath), debug);
	if(!hR) {
		print(TO_WINDOW,"\nCan't open %s\n ",g->sRTermPath);
		return 0;
	}
	if(!RIsRunning(hR)) {
		print(TO_WINDOW,"\nCan't run R!\n ");
		return 0;
	}
	Rx("rm(list = ls());",0); // clear the workspace
	if(source) if(*source)
		Rx(strf("source('%sStrategy/%s')",slash(ZorroFolder),source),2);
	return hR; 
}

inline int Rstart() { return Rstart(0,0); }

int Rrun() 
{ 
	if(!RIsRunning(hR))
		return 0;
	else if(RIsBusy(hR))
		return 2;
	else
		return 1;
}

void Rstop() { 
	if(!hR) return;
	RDeinit(hR); 
#ifdef WIN32
	if(g->RLib) FreeLibrary(g->RLib);
#endif
	hR = 0; 
}

inline void Rset(const char* name, const char* s) { RAssignString(hR, name, s); }

inline void Rset(const char* name, int i) { RAssignInteger(hR, name, i); }

inline void Rset(const char* name, var d) { RAssignDouble(hR, name, d); }

inline void Rset(const char* name, var *a, int size) { RAssignVector(hR, name, a, size); }

inline void Rset(const char* name, var *m, int rows, int cols) { RAssignMatrix(hR, name, m, rows, cols); }

inline int Ri(const char* name){ return(RGetInteger(hR, name)); }

inline var Rd(const char* name) { return(RGetDouble(hR, name)); }

inline void Rv(const char* name, var *a, int size) { RGetVector(hR, name, a, size); }

inline void Rp(const char* expression) { RPrint(hR, expression); }

inline char* Rs(const char* expression) { Rp(expression); return RLastOutput(hR); }

///////////////////////////////////////////////////////////////////////////////
// R machine learning framework

DLLFUNC var neural(int mode, int model, int numSignals, void* Data)
{
	if(!wait(-200)) return 0;
// open an R script with the same name as the strategy script	
	if(mode == NEURAL_INIT) {
		if(!Rstart(strf("%s.r",Script),2)) return 0;
		Rx("neural.init()");
		return 1;
	}
// export batch training samples to a file to be read by R	
	if(mode == NEURAL_TRAIN) {
		string name = strf("Data\\signals%i.csv",Core);
		file_write(name,(char*)Data,0);
		Rx(strf("XY <- read.csv('%s%s',header = F)",slash(ZorroFolder),slash(name)));
		Rset("AlgoVar",AlgoVar,8);
		if(!Rx(strf("neural.train(%i,XY)",model+1),2)) 
			return 0;
		return 1;
	}
// predict the target	
	if(mode == NEURAL_PREDICT) {
		Rset("AlgoVar",AlgoVar,8);
		Rset("X",(double*)Data,numSignals);
		Rx(strf("Y <- neural.predict(%i,X)",model+1));
		return Rd("Y[1]");
	}
// save all trained models	
	if(mode == NEURAL_SAVE) {
		print(TO_ANY,"\nStore %s",strrchr((char*)Data,'\\')+1);
		return Rx(strf("neural.save('%s')",slash((char*)Data)),2);
	}
// load all trained models	
	if(mode == NEURAL_LOAD) {
		print(TO_WINDOW,"\nLoad %s",strrchr((char*)Data,'\\')+1);
		int Exists = Ri("exists('neural.load')");
		if(Exists) return Rx(strf("neural.load('%s')",slash((char*)Data)),2);
 		else return Rx(strf("load('%s')",slash((char*)Data)),2);
 	}
	return 1;
}

///////////////////////////////////////////////////////////////////////////////
#endif // r_h

