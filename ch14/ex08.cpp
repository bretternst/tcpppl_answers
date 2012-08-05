#include <iostream>
#include <string>

namespace ch14
{
    // This exception class is patterned after the .NET System.Exception class.
    // In C#, every exception must derive from this. It is missing some of the
    // properties, such as the stack trace, and target method which require
    // runtime support in .NET. However, the basic idea is the same.
    //
    // In this scenario, exceptions should derive from this class. Generic
    // exceptions could also simply construct this class and throw it since it
    // is not abstract. Generally, deriving classes should take similar
    // constructor parameters to supply to the base, as well as any additional
    // information specific to the exception.
    //
    // This pattern is considered advantageous in C# because it provides a base
    // set of facilities for every error. For example, a suitable diagnostic
    // error message, as well as the source of the error are provided for all
    // exception types. This enables better debugging and logging scenarios.
    // The use of the inner exception allows abstractions to preserve the
    // exception stack and is useful for diagnosing the root cause of a failure
    // without multiplying the number of exceptions a consumer must handle.
    //
    // One disadvantage is that there might be more overhead than throwing a
    // simple empty exception because the fields need to be copied. However,
    // since exceptions are exceptional, performance should not be a primary
    // concern. Another disadvantage is that slicing (throwing a specific
    // exception but catching Exception) without using a pointer or reference
    // would cause the incorrect write method to be used. For this reason, it
    // is better to catch by reference using this pattern.
    class Exception
    {
        std::string m_message;
        Exception* m_inner;
        std::string m_source;

    public:
        Exception() : m_inner(0) {}
        Exception(const std::string& message) : m_message(message), m_inner(0) {}
        Exception(const std::string& message, Exception* inner)
            : m_message(message), m_inner(inner) {}

        virtual void write(std::ostream& output)
        {
            output << m_message << std::endl;
            if(m_inner)
            {
                m_inner->write(output);
            }
        }

        const std::string& message() const { return m_message; }
        const std::string& source() const { return m_source; }
        Exception * const inner() const { return m_inner; }
    };

    std::ostream& operator<<(std::ostream& output, Exception& ex)
    {
        ex.write(output);
        return output;
    }

    class SpecificException : public Exception
    {
    public:
        SpecificException() : Exception() {}
        SpecificException(const std::string& message) : Exception(message) {}
        SpecificException(const std::string& message, Exception* inner) : Exception(message,inner) {}

        void write(std::ostream& output)
        {
            std::cout << "SpecificException is being thrown:" << std::endl;
            Exception::write(output);
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
