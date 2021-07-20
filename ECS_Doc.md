# ECS Usage Documentation

## How to use it for gameplay programming

ECS/Entity-Component-System architecture is an alternative way for gameplay programming to the original OOP paradigm.
More documentation can be found online, which I'll not go in depth here.  

Our intended way to use the system is as follows:  
- to write a new type of component, create a new c++ class/struct and inherit RuntimeComponent.  
- to write a new system, create a new c++ class/struct and inherit RuntimeSystem.  
- each component type (built-in and custom) are labeled by their factory class, so to reference the type of component A, you should use ComponentPool<A>::GetInstance(), which returns the pointer to the factory instance; also it is a singleton and is only initialized on the first call to GetInstance().
- to create an archtype, suppose your member types are A, B, and C, use the statement "Archtype a(); a.Add(ComponentPool<A>::GetInstance()).Add(ComponentPool<B>::GetInstance()).Add(ComponentPool<C>::GetIntance())".  // TODO: this syntax is too long, make a macro for it

## Things yet to settle

- entity filter
- make macros for component type referencing
