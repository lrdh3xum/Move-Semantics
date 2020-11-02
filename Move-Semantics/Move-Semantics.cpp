/*

	C++11 introduced rvalues, a requirement for move semantics.
	Move semantics do not copy one object from one location to another.
	When the new location is the only location where the object is accessible,
	move semantics are useful to avoid this copying.

	For returns, you have return value optimization. For functions, however, the
	passing of a new object requires the creation of a throw-a-way object.
	The compiler copies the new object into the stack frame; then, the function
	receives a copy. This copying is essential to consider when dealing
	with non-trivial objects.

	An object consisting of two primitives, for example, is not a major concern.
	Though, if the object is heap allocating memory, or if it is a string, it would
	require a copy and a completely new heap allocation. Meaning the object is a 
	heavy copy. Move semantics allows the object to be moved instead of copied.

*/

#include <iostream>
#include <string>

class String
{
	
public:
	String() = default;

	// A constructor. Takes a string, allocates memory, and copies everything
	// into the memory buffer.
	String(const char* string)
	{
		printf("Created!\n");
		m_Size = strlen(string);
		m_Data = new char[m_Size];
		memcpy(m_Data, string, m_Size);
	}

	// Copy constructor
	// Deep copy
	String(const String& other)
	{
		printf("Copied!\n");
		m_Size = other.m_Size;
		m_Data = new char[m_Size];
		memcpy(m_Data, other.m_Data, m_Size);
	}

	// Move constructor, takes a temporary (rvalue) object.
	String(String&& other) noexcept
	{
		printf("Moved!\n");
		m_Size = other.m_Size;
		m_Data = other.m_Data;

		// Do not need, we just point to the same block of
		// data as the old string.
		// memcpy(m_Data, other.m_Data, m_Size);

		// Take care of string instance stole
		other.m_Data = nullptr;
		other.m_Size = 0;
	}

	~String()
	{
		std::cout << "Destroyed\n";
		delete m_Data;
	}

	void Print()
	{
		for (uint32_t i = 0; i < m_Size; i++)
		{
			printf("%c", m_Data[i]);
		}

		printf("\n");
	}

private:
	char* m_Data;
	uint32_t m_Size;
};

// A class consuming the String class.
class Entity
{

public:
	// Entity constructor, copies name into m_Name.
	Entity(const String& name) : m_Name(name)
	{

	}

	// Entity constructor that takes a temporary to use
	// with move semantics. Notice the name parameter
	// is called using std::move; this forces the move
	// constructor to be used over the copy constructor.
	Entity(String&& name) : m_Name(std::move(name))
	{

	}

	void PrintName()
	{
		m_Name.Print();
	}

private:
	String m_Name;
};

int main()
{
	// Create object inside main function.
	// "Foo" is a temporary object.
	Entity entity(String("Foo"));
	entity.PrintName();

	std::cin.get();
}
