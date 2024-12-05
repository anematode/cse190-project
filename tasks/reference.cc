
// safe keyword marks a function as being "safe".
int func() safe {
    ...
}

// unsafe keyword can be used to still execute unsafe
// code in a safe environment.
unsafe {
    sprintf(buffer, "hello world");
    ...
}

// BORROW CHECKING

// '^' is used to represent a mutable borrow
Object^ object = ...

// 'const ^' represents a shared borrow
// (object cannot be modified)
const Object^ object = ...

// To call a function that requires a mutable borrow,
// use '^' before the function call
Object object;
object^->set_value(5);

// To borrow an object, put '^' before the object:
modify_object(^object);

// shared borrows do not require an explicit '^' before.

// Borrows act like pointers, so you can "dereference" them
Object^ object;
*object;

// To get a standard C++ reference to an object,
// the new syntax is to put '&' before the object.
Object& ref = &object;


