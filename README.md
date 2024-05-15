# Basics Library
This SFML based library will allow some basic game engine features, like animations, objects, servers, buttons and so on. 
It consists of several modules: AppBasics, GraphicsBasics, NetworkBasics, OfficeBasics abd GameBasics.
Each of these modules provide their own unique functions and classes for easier game development.
Lets look at each module individually and what functionality it brings.
## AppBasics
AppBasics is a module for creating, rendering and drawing on your own window and managing scenes in your application.
But not just that, it also has a unique system of containables and its containers that is used in the entire project.
### Containables and Containers
Lets first look at the Containables and Containers and why almost all of the project is based on these two classes. 
#### Container
Container is a template class that gives any derived class an ability to receive, contain and process a certain Containable class.
This is used to create updaters, initializers and other classes that contain pointers to their containables and that process them in their own unique way.
For example: Menu class in the OfficeBasics module receives any updatable that is created inside it and updates it only when the menu is open.
#### Containable
Containable is also a template class that gives any derived class an ability to be pushed to a certain container and to be processed there.
This is used to create updatables, initalizables and other classes that need to be pushed to a container and processed 
Here is an example of how you can use this structure in your programs:
```c++:
// Include Containable and Container classes
#include "AppBasics/Containable.h"
// Use basics namespace
using namespace basics;


// Define Fooable class and derive it from Containable<Fooable, int>
// to set Fooable as Containable and to process it with int argument
class Fooable : public Containable<Fooable, int>
{
public:
    // Constructor that sets process function to virtual foo function below
    Fooable()
        : Containable<Fooable, int>([&](int arg) { foo(arg); })
    {}
protected:
    // Virtual function 'foo' can be overriden in other derived classes
    virtual void foo(int arg) = 0;
};

// Define Fooer class that adds any created Fooable pointer to its array so it can foo the fooable later
class Fooer : public Container<Fooable, int>
{
public:
    // Constructor that sets container's process function to process all of it's fooables. 
    Fooer()
        : Container<Fooable, int>([&](int arg) 
            { 
                std::cout << "Fooer " << this << " is processing his fooables:\n";
                for (auto fooable : containables())
                {
                    std::cout << "- ";
                    processContainable(fooable, arg);
                }
            })
    {}

    // Public function that calls protected processContainables function
    void foo_all(int arg)
    {
        processContainables(arg);
    }
};

// Derived from Fooable TestFooable class that has more functionality
class TestFooable : public Fooable
{
public:
    // It has a bar variable
    int bar = 5;

    // Overrides a virtual foo function
    void foo(int arg) override
    {
        // Checks if the bar is not 5. If it is, say to the user that bar should be five
        if (bar != 5)
        {
            std::cout << "Fooable " << this << " failed to foo: bar must have value '5'!\n";
            return;
        }

        // Otherwise everything is okay
        std::cout << "Fooable " << this << " was fooed successfully with arg " << arg << "\n";
    }
};

// Public TestFooer derived from Fooer
class TestFooer : public Fooer
{
public:
    // It allows to use protected functions
    using Fooer::endInit;
    using Fooer::startInit;
};

// However, this class doesn't. It is final and has it's own TestFooables. This can be your own Scene, for example
class TestFinalFooer : public Fooer
{
public:
    // In constructor we end initialization of the container so it doesnt push unnessesary Fooables
    TestFinalFooer()
    {
        endInit();
    }

    // Define our own fooables
    TestFooable fooable1, fooable2;
};

int main()
{
    // Example of 'private' fooer
    TestFinalFooer fooer1;

    // Change the bar value of one of the fooables
    fooer1.fooable1.bar = 6;

    // Process all fooables
    fooer1.foo_all(7);


    std::cout << "\n";

    // Example of 'public' fooer
    TestFooer fooer2;

    // Define new fooables that will be pushed to the fooer2
    TestFooable fooable1, fooable2;

    // End initialization of fooer2
    fooer2.endInit();

    // This variable will not be added to fooer2
    TestFooable unpushed_fooable;

    // Process all fooables
    fooer2.foo_all(9);
    

    std::cout << "\nHowever, fooable " << &unpushed_fooable << " was not fooed because there are no fooers to foo it.";
    std::cin.get();

    return 0;
}
```

