#include<cstring>
#include<utility>
#include<new>

namespace implicits{

/// This is the storage for representations of inactive values,
/// which have been overriden and yet to be restored
class implicit_stack{
    /// use a friend class so nobody except us can directly manipulate
    /// the stack
    template<class T, auto tag>
    friend class val;

    char* stack_ptr;   // pointer to the top of the stack
    char stack[1<<20]; // pre-allocated storage for values
                       //
    //create an instance of the stack for each thread
    thread_local static implicit_stack s;

    implicit_stack() : stack_ptr(this->stack){}

    //push a value onto the stack
    template<class T>
    static void push(T const* v) noexcept{
        std::memcpy(s.stack_ptr, v, sizeof(T));
        s.stack_ptr += sizeof(T);
    }
    //pop a value from the stack
    template<class T>
    static void pop(T* v) noexcept{
        s.stack_ptr -= sizeof(T);
        std::memcpy(v, s.stack_ptr, sizeof(T));
    }
};
thread_local implicit_stack implicit_stack::s;


/// This is the user-facing thing, that will provide us with all the
/// tools we need to modify and access an implicit variable
template<class T, auto tag = []{}>
class val{
    thread_local static T active_value;
public:
    // creating an instance of this class will override the implicit value
    val(auto&& args){
        // here we just store the old representation on the stack, and
        // replace it with a new one.
        //
        // Yes, this implemenation means we store a garbage value as the
        // first one on the stack, but we won't ever use it, therefore
        // it does not matter
        implicit_stack::push(&active_value);
        new (&active_value)T (std::forward<decltype(args)>(args));
    }
    // and destroying it will reset that override
    ~val(){
        // and here we get rid of it and recover the old one
        active_value.~T();
        implicit_stack::pop(&active_value);
    }

    // and this is how we get a value to read and/or modify
    static T& get() noexcept{return active_value;}
};
template<class T, auto tag> thread_local T val<T, tag>::active_value;
}
