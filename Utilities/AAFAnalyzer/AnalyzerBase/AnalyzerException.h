#ifndef __ANALYZEREXCEPTION_H_
#define __ANALYZEREXCEPTION_H_

//STL files
#include <exception>
#include <string>

namespace aafanalyzer {

using namespace std;

class AnalyzerException : public std::exception {

public:

    AnalyzerException();
    AnalyzerException( const wchar_t* what );
    AnalyzerException( const wstring& what );
    virtual ~AnalyzerException() throw();
    virtual const wchar_t* widewhat() const throw();
    virtual const char* what() const throw();

private:

    string InitWhat( const wstring& s );

    wstring _widewhat;
    string _what;
    
    // prohibited
    AnalyzerException& operator=( const AnalyzerException& );
};

} // end of namespace diskstream

#endif /*ANALYZEREXCEPTION_H_*/
