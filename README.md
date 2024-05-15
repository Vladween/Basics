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
Container is a template class that gives any derived class ability to receive, contain and process a certain Containable class.
This is used to create updaters, initializers and other classes that contain pointers to their containables and that process them in their own unique way.
For example: Menu class in the OfficeBasics module receives any updatable that is created inside it and updates it only when the menu is open.
Here is an example of how you can use it in your program:
'''c++:
#include "AppBasics/Containable.h"
using namespace basics;

class TestUpdatable : public Containable<TestUpdatable, int>
{
};
int main()
{

}
'''
