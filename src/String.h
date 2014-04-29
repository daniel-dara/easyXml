#ifndef STRING_H
#define STRING_H

#include <iostream>
#include <cstring>
#include <cstdlib>

typedef unsigned int uint;

class String
{
public:
	static const uint npos = -1;

	void debug() const
	{
		std::cout << "\n";
		std::cout << "buf: " << buf_ << "\n";
		std::cout << "capacity: " << capacity_ << "\n";
		std::cout << "length: " << length_ << "\n";
		std::cout << "\n";
	}

	String() :
		capacity_(1),
		length_(0),
		buf_(static_cast<char*>(malloc(2)))
	{
		nullCap();
	}

	String(const String& rhs) :
		capacity_(rhs.capacity_),
		length_(rhs.length_),
		buf_(static_cast<char*>(malloc(rhs.capacity_ + 1)))
	{
		memcpy(buf_, rhs.buf_, length_);
		nullCap();
	}

	String(const char* rhs, uint pos = 0, uint length = npos) :
		capacity_(length == npos ? strlen(rhs) : length),
		length_(capacity_),
		buf_(static_cast<char*>(malloc(capacity_ + 1)))
	{
		memcpy(buf_, rhs + pos, length_);
		nullCap();
	}

	String(const std::string& rhs) :
		capacity_(rhs.capacity()),
		length_(rhs.length()),
		buf_(static_cast<char*>(malloc(capacity_ + 1)))
	{
		memcpy(buf_, rhs.c_str(), length_);
		nullCap();
	}

	String(uint capacity) :
		capacity_(capacity),
		length_(0),
		buf_(static_cast<char*>(malloc(capacity_ + 1)))
	{
		nullCap();
	}

	~String()
	{
		free(buf_);
	}

	inline int compare(const String& rhs) const
	{
		return compare(0, length_, rhs.buf_, 0, rhs.length_);
	}

	inline int compare(const char* rhs) const
	{
		return compare(0, length_, rhs, 0, strlen(rhs));
	}

	inline int compare(uint pos, uint len, const String& rhs, uint subpos = 0, uint sublen = npos) const
	{
		return compare(pos, len, rhs.buf_, subpos, (sublen == npos ? rhs.length_ : sublen));
	}

	inline int compare(uint pos, uint len, const char* rhs, uint subpos = 0, uint sublen = npos) const
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
			i++;
		}

		return 0;
	}

	inline bool operator==(const String& rhs) const
	{
		return compare(rhs) == 0;
	}

	inline bool operator==(const char* rhs) const
	{
		return compare(rhs) == 0;
	}

	inline bool operator==(char rhs) const
	{
		return length_ == 1 && buf_[0] == rhs;
	}

	inline bool operator!=(const String& rhs) const
	{
		return !operator==(rhs);
	}

	inline bool operator!=(const char* rhs) const
	{
		return !operator==(rhs);
	}

	inline bool operator!=(char rhs) const
	{
		return !operator==(rhs);
	}

	inline String& assign(const String& rhs, uint subpos = 0, uint sublen = npos)
	{
		return assign(rhs.buf_, subpos, sublen == npos ? rhs.length_ : sublen);
	}

	inline String& assign(const char* rhs, uint subpos = 0, uint sublen = npos)
	{
		if (buf_ != rhs)
		{
			String temp(rhs, subpos, sublen);
			swap(temp);
		}

		return *this;
	}

	inline String& operator=(const String& rhs)
	{
		return assign(rhs);
	}

	inline String& operator=(const char* rhs)
	{
		return assign(rhs);
	}

	inline String& operator=(char rhs)
	{
		return assign(&rhs, 0, 1);
	}

	inline String& append(const String& str, uint subpos = 0, uint sublen = npos)
	{
		return append(str.buf_, subpos, sublen == npos ? str.length_ : sublen);
	}

	inline String& append(const char* str, uint subpos = 0, uint sublen = npos)
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

	inline String& append(char c)
	{
		_reserve(length_ + 1);
		buf_[length_++] = c;
		nullCap();
		return *this;
	}

	inline String& append(char c, uint copies)
	{
		_reserve(length_ + copies);

		while (copies--)
		{
			buf_[length_++] = c;
		}

		nullCap();
		return *this;
	}

	inline uint max_size()
	{
		return npos - 1;
	}

	inline String& operator+=(const String& rhs)
	{
		return append(rhs);
	}

	inline String& operator+=(const char* rhs)
	{
		return append(rhs);
	}

	inline String& operator+=(char rhs)
	{
		return append(rhs);
	}

	inline char& operator[](uint index)
	{
		return buf_[index];
	}

	inline const char& operator[](uint index) const
	{
		return buf_[index];
	}

	inline void swap(String& rhs)
	{
		swap(buf_, rhs.buf_);
		swap(capacity_, rhs.capacity_);
		swap(length_, rhs.length_);
	}

	inline void reserve(uint capacity)
	{
		if (capacity > capacity_)
		{
			if (capacity > max_size())
			{
				throw std::string("String Error: Cannot reserve greater than max_size.");
			}

			// Create bigger buffer
			char* temp = static_cast<char*>(realloc(buf_, capacity + 1));

			// Copy old buf
			memcpy(temp, buf_, length_ + 1);

			swap(buf_, temp);
			// delete[] temp;

			capacity_ = capacity;
		}
	}

	/// Resize string and fill empty space with "fill"
	inline void resize(uint length, char fill = '\0')
	{
		if (length > length_)
		{
			reserve(length);
			memset(buf_ + length, fill, length - length_);
		}
		
		length_ = length;
		nullCap();
	}

	inline void shrink_to_fit()
	{
		String temp(buf_, 0, length_);
		swap(temp);
	}

	// Note: c_str() can be modified by further calls to String
	inline const char* c_str() const
	{
		return buf_;
	}

	inline void clear()
	{
		length_ = 0;
		nullCap();
	}

	inline uint size() const
	{
		return length_;
	}

	inline uint length() const
	{
		return length_;
	}

	inline uint capacity() const
	{
		return capacity_;
	}

private:
	uint capacity_;
	uint length_;
	char* buf_;

	template <class T>
	inline void swap (T& a, T& b)
	{
		T c(a);
		a = b;
		b = c;
	}

	inline void nullCap()
	{
		buf_[length_] = '\0';
	}

	inline void _reserve(uint capacity)
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
};

inline String operator+(const String& lhs, const String& rhs)
{
	return String(lhs).append(rhs);
}

inline String operator+(const char* lhs, const String& rhs)
{
	return String(lhs).append(rhs);
}

inline String operator+(const String& lhs, const char* rhs)
{
	return String(lhs).append(rhs);
}

inline String operator+(char lhs, const String& rhs)
{
	return String(&lhs, 0, 1).append(rhs);
}

inline String operator+(const String& lhs, char rhs)
{
	return String(lhs).append(rhs);
}

inline std::ostream& operator<<(std::ostream& out, const String& rhs)
{
	return out << rhs.c_str();
}

#endif
