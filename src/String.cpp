#include "String.h"

/* SECTION: Constructors ******************************************************/

String::String() :
	capacity_(1),
	length_(0),
	buf_(static_cast<char*>(malloc(2)))
{
	nullCap();
}

String::String(const String& str) :
	capacity_(str.capacity_),
	length_(str.length_),
	buf_(static_cast<char*>(malloc(str.capacity_ + 1)))
{
	memcpy(buf_, str.buf_, length_);
	nullCap();
}

String::String(const String& str, uint pos, uint len) :
	capacity_(len > str.length_ - pos ? str.length_ - pos : len),
	length_(capacity_),
	buf_(static_cast<char*>(malloc(capacity_ + 1)))
{
	if (pos > str.length_)
	{
		throw String("pos cannot be greater than str.length()");
	}

	memcpy(buf_, str.buf_ + pos, length_);
	nullCap();
}

String::String(const char* s) :
	capacity_(strlen(s)),
	length_(capacity_),
	buf_(static_cast<char*>(malloc(capacity_ + 1)))
{
	memcpy(buf_, s, length_);
	nullCap();
}

String::String(const char* s, uint n) :
	capacity_(n),
	length_(capacity_),
	buf_(static_cast<char*>(malloc(capacity_ + 1)))
{
	memcpy(buf_, s, n);
	nullCap();
}

String::String(uint n, char c) :
	capacity_(n),
	length_(n),
	buf_(static_cast<char*>(malloc(capacity_ + 1)))
{
	memset(buf_, c, n);
}

String::String(const std::string& rhs) :
	capacity_(rhs.capacity()),
	length_(rhs.length()),
	buf_(static_cast<char*>(malloc(capacity_ + 1)))
{
	memcpy(buf_, rhs.c_str(), length_);
	nullCap();
}

String::String(uint capacity) :
	capacity_(capacity),
	length_(0),
	buf_(static_cast<char*>(malloc(capacity_ + 1)))
{
	nullCap();
}

/* SECTION: destructor ********************************************************/

String::~String()
{
	free(buf_);
}

/* SECTION: operator= ********************************************************/

String& String::operator=(const String& str)
{
	return assign(str);
}

String& String::operator=(const char* s)
{
	return assign(s);
}

String& String::operator=(char c)
{
	return assign(&c, 0, 1);
}

/* SECTION: Iterators *********************************************************/

// TODO: implement

/* SECTION: Capacity **********************************************************/

uint String::size() const
{
	return length_;
}

uint String::length() const
{
	return length_;
}

uint String::max_size()
{
	return npos - 1;
}

void String::resize(uint length, char fill)
{
	if (length > length_)
	{
		reserve(length);
		memset(buf_ + length, fill, length - length_);
	}
	
	length_ = length;
	nullCap();
}

uint String::capacity() const
{
	return capacity_;
}

void String::reserve(uint capacity)
{
	if (capacity > capacity_)
	{
		if (capacity > max_size())
		{
			throw String("String Error: Cannot reserve greater than max_size.");
		}

		// Create bigger buffer
		char* temp = static_cast<char*>(realloc(buf_, capacity + 1));

		// Copy old buf
		// memcpy(temp, buf_, length_ + 1);

		swap(buf_, temp);
		// delete[] temp;

		capacity_ = capacity;
	}
}

void String::clear()
{
	length_ = 0;
	nullCap();
}

bool String::empty()
{
	return length_ == 0;
}

void String::shrink_to_fit()
{
	String temp(buf_, 0, length_);
	swap(temp);
}

/* SECTION: Element Access ****************************************************/

char& String::operator[](int index)
{
	return const_cast<char&>((*static_cast<const String*>(this))[index]);
}

const char& String::operator[](int index) const
{
	return at(index);
}

char& String::at(int index)
{
	return const_cast<char&>(static_cast<const String*>(this)->at(index));
}

const char& String::at(int index) const
{
	return buf_[index];
}

const char& String::back() const
{
	return buf_[length_];
}

const char& String::front() const
{
	return buf_[0];
}

/* SECTION: Modifiers *********************************************************/

String& String::operator+=(const String& rhs)
{
	return append(rhs);
}

String& String::operator+=(const char* rhs)
{
	return append(rhs);
}

String& String::operator+=(char rhs)
{
	return append(rhs);
}

String& String::append(const String& str, uint subpos, uint sublen)
{
	return append(str.buf_, subpos, sublen == npos ? str.length_ : sublen);
}

String& String::append(const char* str, uint subpos, uint sublen)
{
	if (sublen == npos)
	{
		sublen = strlen(str);
	}

	_reserve(length_ + sublen);

	memcpy(buf_ + length_, str + subpos, sublen);
	length_ += sublen;
	nullCap();

	return *this;		
}

String& String::append(char c)
{
	_reserve(length_ + 1);
	buf_[length_++] = c;
	nullCap();
	return *this;
}

String& String::append(char c, uint copies)
{
	_reserve(length_ + copies);

	while (copies--)
	{
		buf_[length_++] = c;
	}

	nullCap();
	return *this;
}

String& String::assign(const String& str)
{
	return assign(str.buf_, str.length_);
}

String& String::assign(const String& str, uint subpos, uint sublen)
{
	uint len = str.length_ - subpos;
	return assign(str.buf_ + subpos, sublen > len ? len : sublen);
}

String& String::assign(const char* s)
{
	return assign(s, strlen(s));
}

String& String::assign(const char* s, uint n)
{
	if (buf_ != s)
	{
		String temp(s, n);
		swap(temp);
	}

	return *this;
}

String& String::assign(uint n, char c)
{
	if (n > capacity_)
	{
		resize(n);
	}

	memset(buf_, c, n);
	
	return *this;
}

void String::swap(String& rhs)
{
	swap(buf_, rhs.buf_);
	swap(capacity_, rhs.capacity_);
	swap(length_, rhs.length_);
}

/* SECTION: String operations *********************************************/

/// Note: c_str() can be modified by further calls to String
const char* String::c_str() const
{
	return buf_;
}

uint String::find(const String& str, uint pos) const
{
	return find(str.c_str(), pos, str.length());
}

uint String::find(const char* s, uint pos) const
{
	return find(s, pos, strlen(s));
}

uint String::find(const char* s, uint pos, uint n) const
{
	while (pos < length_)
	{
		if (compare(pos, n, s, 0, n) == 0)
		{
			return pos;
		}

		pos++;
	}

	return npos;
}

uint String::find(char c, uint pos) const
{
	while (pos < length_)
	{
		if (buf_[pos] == c)
		{
			return pos;
		}
		
		pos++;
	}

	return npos;
}

String String::substr(uint pos, uint len) const
{
	return String(buf_, pos, len == npos ? length_ - pos : len);
}

int String::compare(const String& rhs) const
{
	return compare(0, length_, rhs.buf_, 0, rhs.length_);
}

int String::compare(const char* rhs) const
{
	return compare(0, length_, rhs, 0, strlen(rhs));
}

int String::compare(uint pos, uint len, const String& rhs, uint subpos, uint sublen) const
{
	return compare(pos, len, rhs.buf_, subpos, (sublen == npos ? rhs.length_ : sublen));
}

int String::compare(uint pos, uint len, const char* rhs, uint subpos, uint sublen) const
{
	if (sublen == npos)
	{
		sublen = strlen(rhs + subpos);
	}

	if (len != sublen)
	{
		// Returns -1 if "this" string is shorter and 1 if it is longer.
		// Boolean arithmetic avoids a costly branch.
		return (len > sublen) - (len < sublen);
	}

	const char* buf_char = buf_ + pos;
	const char* rhs_char = rhs + subpos;
	uint i = 0;

	// while (buf_char - buf_ < len) could also be used but I'm not sure if the arithmetic would be
	// properly optimized out. The only variable changing in that equation is buf_char so its more
	// efficient to just use a counter.
	while (i < len)
	{
		if (*buf_char != *rhs_char)
		{
			return (*buf_char > *rhs_char) - (*buf_char < *rhs_char);
		}
		buf_char++;
		rhs_char++;
		i++;
	}

	return 0;
}

/* SECTION: Member constants **************************************************/

static const uint npos = -1;

/* SECTION: Miscellaneous *****************************************************/

void String::debug(std::ostream& out) const
{
	out << "\n"
	    << "buf: " << buf_ << "\n"
	    << "capacity: " << capacity_ << "\n"
	    << "length: " << length_ << "\n"
	    << "\n";
}

template <class T>
void String::swap (T& a, T& b)
{
	T c(a);
	a = b;
	b = c;
}

void String::nullCap()
{
	buf_[length_] = '\0';
}

void String::_reserve(uint capacity)
{
	if (capacity > capacity_)
	{
		uint newCapacity = capacity_ ? capacity_ : 1;

		do
		{
			newCapacity *= 2;
		}
		while (capacity > newCapacity);

		reserve(newCapacity);	
	}
}

/* SECTION: Non-member function overloads *************************************/

String operator+(const String& lhs, const String& rhs)
{
	return String(lhs).append(rhs);
}

String operator+(const char* lhs, const String& rhs)
{
	return String(lhs).append(rhs);
}

String operator+(const String& lhs, const char* rhs)
{
	return String(lhs).append(rhs);
}

String operator+(char lhs, const String& rhs)
{
	return String(&lhs, 0, 1).append(rhs);
}

String operator+(const String& lhs, char rhs)
{
	return String(lhs).append(rhs);
}

bool operator==(const String& lhs, const String& rhs)
{
	return lhs.compare(rhs) == 0;
}

bool operator==(const String& lhs, const char* rhs)
{
	return lhs.compare(rhs) == 0;
}

bool operator==(const char* lhs, const String& rhs)
{
	return rhs.compare(lhs) == 0;
}

bool operator!=(const String& lhs, const String& rhs)
{
	return !operator==(lhs, rhs);
}

bool operator!=(const String& lhs, const char* rhs)
{
	return !operator==(lhs, rhs);
}

bool operator!=(const char* lhs, const String& rhs)
{
	return !operator==(lhs, rhs);
}

std::ostream& operator<<(std::ostream& out, const String& rhs)
{
	return out << rhs.c_str();
}
