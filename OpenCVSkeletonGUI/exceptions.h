#include <exception>
using namespace std;

struct NumOfVertException : public exception
{
  const char * what () const throw ()
  {
    return " There are more than four outer corners! ";
  }
};
