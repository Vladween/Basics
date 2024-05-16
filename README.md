# Basics Library
This SFML based library will allow some basic game engine features, like animations, objects, servers, buttons and so on. 
It consists of several modules: AppBasics, GraphicsBasics, NetworkBasics, OfficeBasics and GameBasics.
Each of these modules provide their own unique functions and classes for easier game development.
## How to include
If you want to include this library in your Visual Studio project, do these steps:
- Download Basics.rar file
- Unzip it into your local folder
- In your project go to 'Project'->'Properties'->'C/C++'->'General'->'Additional Include Directories'->'Edit' and choose path to the ...\Basics\include folder.
- Then go to 'Linker'->'General'->'Additional Library Directories'->'Edit' and choose path to the ...\Basics\lib\Debug or ...\Basics\lib\Release (depends on your solution configuration. By default it is set to Debug).
- Then go to 'Linker'->'Input'->'Additional Dependencies'->'Edit' and write 'Basics.lib' there.
- Then go to 'Linker'->'System'->'SubSystem' and select 'Windows' there.
- Then go to 'Linker'->'Advanced'->'Entry Point' and write 'mainCRTStartup' there.
- Press 'OK'
- Go to your project folder and paste all files from ...\Basics\bin\Debug or ...\Basics\bin\Release folder (again, depends on your solution configuration)
- In your main.cpp file paste current test code:
```c++:
#include "AppBasics.h"
using namespace basics;

class Test : public Application
{
public:
    void init() override
    {
        createWindow({200, 200}, "Basics works!");
        shape.setRadius(100);
        shape.setFillColor(sf::Color::Green);
    }
    void draw() override
    {
        window.draw(shape);
    }
private:
    sf::CircleShape shape;
};
MAIN(Test)
```
And thats about it! Now lets look at each module individually and see what they allow.
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
### Application
Application is a class that makes any derived class a valid application.
It contains several virtual functions (they are not purely virtual, so you dont need to override them all):
- init - called when all containers and initializables are initialized
- whenWindowCreated - called when window is created
- event_update - called in every iteration of the event loop
- update - called in every iteration of the main loop
- draw - called in every iteration of the main loop right after the window was cleared.

Here is an example of how you can create a standard SFML test using Application class:
```c++:
// Include Application class
#include "AppBasics/Application.h"
// Use basics namespace
using namespace basics;

// Create your custom Application
class SFMLTest : public Application
{
protected:
	// This function is called first, right after all containers are initialized
	void init() override
	{
		createWindow({ 200, 200 }, "SFML with Basics works!");

		shape.setRadius(100);
		shape.setFillColor(sf::Color::Green);
	}
	// This function is called when window is cleared.
	// Right after this function window.display() is called and loop starts all over again
	void draw() override
	{
		window.draw(shape);
	}
private:
	// Create a CircleShape for testing
	sf::CircleShape shape;
};

// This macro expands to:
// int main()
// {
//     SFMLTest app;
//     app.run();
//	   return 0;
// }
MAIN(SFMLTest)
```
### AppInfo
AppInfo is a class that provides AppUpdatables with useful informaition about the app. Application class is derived from AppInfo.
AppInfo argument is provided as a reference, so it is possible to change some of application data inside of the process functions.
### AppUpdatables
AppUpdatables.h file contains some classes that are updated or initialized by the application. They are all processed with the AppInfo& argument.
There are several default app updatables and updaters:
- Updatable - has purely virtual 'update' function. Function is called every frame in the main loop - Its containers is Updater.
- EventUpdatable - has purely virtual 'event_update' function. Function is called every frame in the event loop - Its container is EventUpdater.
- Initializable - has purely virtual 'init' function. Function is called once before Application::init is called - Its container is Initalizer.

Here is another example of how you can use AppUpdatables in your programs:
```c++:
// Include Application and AppUpdatables
#include "AppBasics/Application.h"
// Use basics namespace
using namespace basics;

// Create your own class containing virtual init, event_update and update functions
class TestUpdatable : public Updatable, public EventUpdatable, public Initializable
{
public:
	// Constructor with a std::string name argument
	TestUpdatable(std::string name)
		: m_name(name)
	{
	}

	// Override init function
	void init(AppInfo& app) override
	{
		std::cout << "TestUpdatable " << m_name << " was successfully initialized!\n";
	}

	// Override event_update function
	void event_update(AppInfo& app) override
	{
		std::cout << "TestUpdatable " << m_name << " was successfully updated in the event loop\n";
	}

	// Override update function
	void update(AppInfo& app) override
	{
		std::cout << "TestUpdatable " << m_name << " was successfully updated in the main loop\n";
	}
protected:
	// Name that will be printed to console
	std::string m_name;
};

// Our application class that will update our updatables
class AppUpdatablesTest : public Application
{
protected:
	// Initialize application
	void init() override
	{
		// Create window
		createWindow({ 200, 200 }, "AppUpdatables Test");
	}
private:
	// Create foo and bar updatables
	TestUpdatable updatable1 = { "foo" }, updatable2 = { "bar" };
};
// Main macro
MAIN(AppUpdatablesTest)
```
