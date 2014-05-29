#ifndef DARA_STRING_H
#define DARA_STRING_H

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>

typedef unsigned int uint;

class String
{
public:
	/* SECTION: Constructors **************************************************/

	// TODO: Add complexity and exceptions to constructors

	/// empty string constructor (default constructor)
	/**
	 * Constructs an empty string with a length of zero characters and a
	 * capacity of one.
	 */
	String();

	/// copy constructor
	/**
	 * Constructs a copy of *str*.
	 */
	String(const String& str);

	/// substring constructor
	/**
	 * Copies the portion of *str* that begins at the character position *pos*
	 * and spans *len* characters (or until the end of *str*, if either *str* is
	 * too short or if *len* is string::npos).
	 */
	String(const String& str, uint pos, uint len = npos);

	/// from c-string
	/**
	 * Copies the null-terminated character sequence (C-string) pointed by s.
	 */
	String(const char* s);

	/// from buffer
	/**
	 * Copies the first n characters from the array of characters pointed by s.
	 */
	String(const char* s, uint n);

	/// fill constructor
	/**
	 * Fills the string with n consecutive copies of character c.
	 */
	String(uint n, char c);

	/// from std::string
	/**
	 * Copies the std::string (C++ STL).
	 */
	String(const std::string& rhs);

	/// capacity constructor
	/**
	 * Initializes an empty string with the given capacity.
	 */
	String(uint capacity);

	/* SECTION: destructor ****************************************************/

	/// Destroys the string object.
	/**
	 * This deallocates all the storage capacity allocated by the string.
	 */
	~String();

	/* SECTION: operator= ****************************************************/

	/**
	 * @name operator=()
	 * 
	 * #### String assignment ####
	 * Assigns a new value to the string, replacing its current contents.
	 * (See member function assign for additional assignment options).
	 *
	 * #### Complexity ####
	 * Linear in respect to the length of what is being assigned.
	 */
	 //@{

	/// assign string
	/**
	 * Copies *str* as a new value for this string
	 */
	inline String& operator=(const String& str);

	/// assign C-string
	/**
	 * @param s
	 * Pointer to a null-terminated sequence of characters.
	 * The sequence is copied as the new value for the string.
	 */
	inline String& operator=(const char* s);

	/// assign char
	/**
	 * A character. The string value is set to a single copy of this character
	 * (the string length becomes 1).
	 */
	inline String& operator=(char c);
	//@}

	/* SECTION: Iterators *****************************************************/

	// TODO: implement

	/* SECTION: Capacity ******************************************************/

	inline uint size() const;

	inline uint length() const;

	inline uint max_size();

	/// Resize string and fill empty space with "fill"
	inline void resize(uint length, char fill = '\0');

	inline uint capacity() const;

	inline void reserve(uint capacity);

	inline void clear();

	inline bool empty();

	inline void shrink_to_fit();

	/* SECTION: Element Access ************************************************/

	char& operator[](int index);

	const char& operator[](int index) const;

	char& at(int index);

	const char& at(int index) const;

	const char& back() const;

	const char& front() const;

	/* SECTION: Modifiers *****************************************************/

	inline String& operator+=(const String& rhs);

	inline String& operator+=(const char* rhs);

	inline String& operator+=(char rhs);

	inline String& append(const String& str, uint subpos = 0, uint sublen = npos);

	inline String& append(const char* str, uint subpos = 0, uint sublen = npos);

	inline String& append(char c);

	inline String& append(char c, uint copies);

	/**
	 * @name assign()
	 * 
	 * #### Assign content to string ####
	 * Assigns a new value to the string, replacing its current contents.
	 *
	 * #### Complexity ####
	 * Linear in respect to the length of what is being copied.
	 */
	 //@{

	/// string
	/**
	 * Copies str.
	 */
	inline String& assign(const String& str);

	/// substring
	/**
	 * Copies the portion of *str* that begins at the character position
	 * *subpos* and spans *sublen* characters (or until the end of *str*, if
	 * either *str* is too short or if *sublen* is string::npos).
	 */
	inline String& assign(const String& str, uint subpos, uint sublen);

	/// c-string
	/**
	 * Copies the first *n* characters from the array of characters pointed to
	 * by *s*.
	 */
	inline String& assign(const char* s);

	/// buffer
	/**
	 * Copies the first *n* characters from the array of characters pointed to
	 * by *s*.
	 */
	inline String& assign(const char* s, uint n);

	/// fill
	/**
	 * Replaces the current value by *n* consecutive copies of character *c*.
	 */
	inline String& assign(uint n, char c);
	//@}

	inline void swap(String& rhs);

	/* SECTION: String operations *********************************************/

	/// Note: c_str() can be modified by further calls to String
	inline const char* c_str() const;

	inline uint find(const String& str, uint pos = 0) const;

	inline uint find(const char* s, uint pos = 0) const;

	inline uint find(const char* s, uint pos, uint n) const;

	inline uint find(char c, uint pos = 0) const;

	String substr(uint pos = 0, uint len = npos) const;

	inline int compare(const String& rhs) const;

	inline int compare(const char* rhs) const;

	inline int compare(uint pos, uint len, const String& rhs, uint subpos = 0, uint sublen = npos) const;

	inline int compare(uint pos, uint len, const char* rhs, uint subpos = 0, uint sublen = npos) const;

	/* SECTION: Member constants **********************************************/

	static const uint npos = -1;

	/* SECTION: Miscellaneous *************************************************/

	/// Print debug information
	/*
	 * Prints the current capacity, length, and contents of the internal buffer.
	 */
	void debug(std::ostream& out = std::cout) const;

private:
	uint capacity_;
	uint length_;
	char* buf_;

	template <class T>
	inline void swap (T& a, T& b);

	inline void nullCap();

	inline void _reserve(uint capacity);
};

/* SECTION: Non-member function overloads *************************************/

inline String operator+(const String& lhs, const String& rhs);

inline String operator+(const char* lhs, const String& rhs);

inline String operator+(const String& lhs, const char* rhs);

inline String operator+(char lhs, const String& rhs);

inline String operator+(const String& lhs, char rhs);

inline bool operator==(const String& lhs, const String& rhs);

inline bool operator==(const String& lhs, const char* rhs);

inline bool operator==(const char* lhs, const String& rhs);

inline bool operator!=(const String& lhs, const String& rhs);

inline bool operator!=(const String& lhs, const char* rhs);

inline bool operator!=(const char* lhs, const String& rhs);

inline std::ostream& operator<<(std::ostream& out, const String& rhs);

#endif
