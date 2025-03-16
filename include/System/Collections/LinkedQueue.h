#include <System/Collections/IQueue.h>
#include <System/Collections/LinkedList.h>

namespace System::Collections {
    template <typename T>
    class LinkedQueue : protected LinkedList<T>
    {
    public:
        LinkedQueue()                                    = default;
        LinkedQueue(LinkedQueue const& other)            = delete;
        LinkedQueue(LinkedQueue&& other)                 = delete;
        LinkedQueue& operator=(LinkedQueue const& other) = delete;
        LinkedQueue& operator=(LinkedQueue&& other)      = delete;
        ~LinkedQueue()                                   = default;

        void operator<<(T const& val) {}

        bool operator>>(T& val) { return false; }

    private:
    };
}