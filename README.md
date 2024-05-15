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

class Fooable : public Containable<Fooable, int>
{
public:
    Fooable()
        : Containable<Fooalbe>([&](int arg) { foo(arg); })
    {}
protected:
    virtual void foo(int arg) = 0;
};

class Fooer : public Container<Fooable, int>
{
public:
    Fooer()
        : Container<Fooable, int>([&](int arg) { foo_all(arg)})
    {}

    void foo_all(int arg)
    {
        std::cout << "Fooer " << this << " is processing his fooables:\n";
        for(auto fooalbe : containables())
        {
            std::cout << "- ";
            processContainable(fooable);
        }
    }
};

class TestFooable : public Fooable
{
public:
    int bar = 5;
    void foo(int arg) override
    {
        std::cout << "Fooable " << this << " was fooed successfully with arg " << arg << " (current bar is " <<  bar << ")\n";
    }
};

class TestFinalFooer : public Fooer
{
public:
    TestFooer()
    {
        endInit();
    }
    TestFooable fooable1, fooable2;
};

int main()
{
    
}
```
