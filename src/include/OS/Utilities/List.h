namespace HydroGen::OS::Utilities {
    template <typename T>
    class List
    {
    public:
        struct _Node;
    };

    template <typename T>
    struct List<T>::_Node
    {
        _Node *m_previous, *m_next;
        T      m_value;
    };
}