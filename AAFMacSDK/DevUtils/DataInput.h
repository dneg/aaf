#ifndef _DATAINPUT_H
#define _DATAINPUT_H

#ifdef __cplusplus
extern "C" {
#endif


/* Redefine the normal "main" entrypoint to "Main" so that we can
   re-implement main to process a command line and redirect stdout
   before calling the renamed "Main" with the correct values for 
   argc and argv */
#define main Main


/* The prototype for the newly defined (renamed) global function. */
int Main(int argc, char* argv[]);


/* The following routine are obsolete. */
void getInputData(int *pargc, char **argv, char *inputFile);
void cleanUpInputData(int argc, char **argv);


#ifdef __cplusplus
}
#endif

#endif
