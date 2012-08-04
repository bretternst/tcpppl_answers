#include <iostream>
#include <string>

namespace ch14
{
    // This exception class is patterned after the .NET System.Exception class. In C#, every
    // exception must derive from this. It is missing some of the properties, such as the stack
    // trace, and target method which require runtime support in .NET. However, the basic idea
    // is the same.
    //
    // In this scenario, exceptions should derive from this class. Generic exceptions could also
    // simply construct this class and throw it since it is not abstract. Generally, deriving
    // classes should take similar constructor parameters to supply to the base, as well as any
    // additional information specific to the exception.
    //
    // This pattern is considered advantageous in C# because it provides a base set of facilities for
    // every error. For example, a suitable diagnostic error message, as well as the source of the
    // error are provided for all exception types. This enables better debugging and logging scenarios.
    //
    // One disadvantage is that there might be more overhead than throwing a simple empty exception
    // because the fields need to be copied. However, since exceptions are exceptional, performance
    // should not be a primary concern. Another disadvantage is that slicing (throwing a specific exception
    // but catching Exception) without using a pointer or reference would cause the incorrect Write
    // method to be used. For this reason, it is better to catch by reference using this pattern.
    class Exception
    {
        std::string message;
        Exception* innerException;
        std::string source;

    public:
        Exception() : innerException(0) {}
        Exception(const std::string& message) : message(message), innerException(0) {}
        Exception(const std::string& message, Exception* innerException) : message(message), innerException(innerException) {}

        virtual void Write(std::ostream& output)
        {
            output << message << std::endl;
            if(innerException)
            {
                innerException->Write(output);
            }
        }

        const std::string& Message() const { return message; }
        const std::string& Source() const { return source; }
        Exception * const InnerException() const { return innerException; }
    };

    std::ostream& operator<<(std::ostream& output, Exception& ex)
    {
        ex.Write(output);
        return output;
    }

    class SpecificException : public Exception
    {
    public:
        SpecificException() : Exception() {}
        SpecificException(const std::string& message) : Exception(message) {}
        SpecificException(const std::string& message, Exception* innerException) : Exception(message,innerException) {}

        void Write(std::ostream& output)
        {
            std::cout << "SpecificException is being thrown:" << std::endl;
            Exception::Write(output);
        }
    };
}

void f()
{
    throw ch14::SpecificException("a specific error occurred");
}

int main()
{
    using namespace std;
    using namespace ch14;

    try
    {
        f();
    }
    catch(Exception& ex)
    {
        cout << ex;
    }

    return 0;
}
