#ifndef STRING_H
#define STRING_H

#include <iostream>
#include <cstring>

typedef unsigned int uint;

class String
{
public:
	static const uint npos = -1;

	void debug() const
	{
		std::cout << "buf: " << buf_ << "\n";
		std::cout << "capacity: " << capacity_ << "\n";
		std::cout << "length: " << length_ << "\n\n";
	}

	String() :
		capacity_(1),
		length_(0),
		buf_(new char[2])
	{
		nullCap();
	}

	String(const String& rhs) :
		capacity_(rhs.capacity_),
		length_(rhs.length_),
		buf_(new char[rhs.capacity_ + 1])
	{
		memcpy(buf_, rhs.buf_, length_);
		nullCap();
	}

	String(const char* rhs, uint pos = 0, uint length = npos) :
		capacity_(length == npos ? strlen(rhs) : length),
		length_(capacity_),
		buf_(new char[capacity_ + 1])
	{
		memcpy(buf_, rhs + pos, length_);
		nullCap();
	}

	String(const std::string& rhs) :
		capacity_(rhs.capacity()),
		length_(rhs.length()),
		buf_(new char[capacity_ + 1])
	{
		memcpy(buf_, rhs.c_str(), length_);
		nullCap();
	}

	~String()
	{
		delete[] buf_;
	}

	int compare(const String& rhs) const
	{
		return compare(0, length_, rhs.buf_, 0, rhs.length_);
	}

	int compare(const char* rhs) const
	{
		return compare(0, length_, rhs, 0, strlen(rhs));
	}

	int compare(uint pos, uint len, const String& rhs, uint subpos = 0, uint sublen = npos) const
	{
		return compare(pos, len, rhs.buf_, subpos, (sublen == npos ? rhs.length_ : sublen));
	}

	int compare(uint pos, uint len, const char* rhs, uint subpos = 0, uint sublen = npos) const
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

	bool operator==(const String& rhs) const
	{
		return compare(rhs) == 0;
	}

	bool operator==(const char* rhs) const
	{
		return compare(rhs) == 0;
	}

	bool operator==(char rhs) const
	{
		return length_ == 1 && buf_[0] == rhs;
	}

	bool operator!=(const String& rhs) const
	{
		return !operator==(rhs);
	}

	bool operator!=(const char* rhs) const
	{
		return !operator==(rhs);
	}

	bool operator!=(char rhs) const
	{
		return !operator==(rhs);
	}

	String& assign(const String& rhs, uint subpos = 0, uint sublen = npos)
	{
		return assign(rhs.buf_, subpos, sublen == npos ? rhs.length_ : sublen);
	}

	String& assign(const char* rhs, uint subpos = 0, uint sublen = npos)
	{
		if (buf_ != rhs)
		{
			String temp(rhs, subpos, sublen);
			swap(temp);
		}

		return *this;
	}

	String& operator=(const String& rhs)
	{
		return assign(rhs);
	}

	String& operator=(const char* rhs)
	{
		return assign(rhs);
	}

	String& operator=(char rhs)
	{
		return assign(&rhs, 0, 1);
	}

	String& append(const String& str, uint subpos = 0, uint sublen = npos)
	{
		return append(str.buf_, subpos, sublen == npos ? str.length_ : sublen);
	}

	String& append(const char* str, uint subpos = 0, uint sublen = npos)
	{
		if (sublen == npos)
		{
			sublen = strlen(str);
		}

		_reserve(length_ + sublen);
		memcpy(buf_ + length_, str + subpos, sublen);
		nullCap();

		return *this;		
	}

	String& append(char c, uint copies = 1)
	{
		_reserve(length_ + copies);

		while (copies--)
		{
			buf_[length_++] = c;
		}

		nullCap();
		return *this;
	}

	uint max_size()
	{
		return npos - 1;
	}

	String& operator+=(const String& rhs)
	{
		return append(rhs);
	}

	String& operator+=(const char* rhs)
	{
		return append(rhs);
	}

	String& operator+=(char rhs)
	{
		return append(rhs);
	}

	void swap(String& rhs)
	{
		swap(buf_, rhs.buf_);
		swap(capacity_, rhs.capacity_);
		swap(length_, rhs.length_);
	}

	void reserve(uint capacity)
	{
		if (capacity > max_size())
		{
			throw std::string("String Error: Can not reserve greater than max_size.");
		}
		else if (capacity > capacity_)
		{
			// Create bigger buffer
			char* temp = new char[capacity + 1];

			// Copy old buf
			memcpy(temp, buf_, length_ + 1);

			swap(buf_, temp);
			delete[] temp;

			capacity_ = capacity;
		}
	}

	/// Resize string and fill empty space with "fill"
	void resize(uint length, char fill = '\0')
	{
		if (length > length_)
		{
			reserve(length);
			memset(buf_ + length, fill, length - length_);
		}
		
		length_ = length;
		nullCap();
	}

	void shrink_to_fit()
	{
		String temp(buf_, 0, length_);
		swap(temp);
	}

	// Note: c_str() can be modified by further calls to String
	const char* c_str() const
	{
		return buf_;
	}

	void clear()
	{
		length_ = 0;
		nullCap();
	}

	uint size() const
	{
		return length_;
	}

	uint length() const
	{
		return length_;
	}

	uint capacity() const
	{
		return capacity_;
	}

private:
	uint capacity_;
	uint length_;
	char* buf_;

	template <class T>
	void swap (T& a, T& b)
	{
		T c(a);
		a = b;
		b = c;
	}

	void nullCap()
	{
		buf_[length_] = '\0';
	}

	void _reserve(uint capacity)
	{
		uint newCapacity = capacity_;
		while (capacity > newCapacity)
		{
			newCapacity *= 2;
		}

		reserve(newCapacity);
	}
};

inline String operator+(const String& lhs, const String& rhs)
{
	String temp(lhs);
	temp.append(rhs);
	return temp;
}

inline String operator+(const char* lhs, const String& rhs)
{
	String temp(lhs);
	temp.append(rhs);
	return temp;
}

inline String operator+(const String& lhs, const char* rhs)
{
	String temp(lhs);
	temp.append(rhs);
	return temp;
}

inline String operator+(char lhs, const String& rhs)
{
	String temp(&lhs, 0, 1);
	temp.append(rhs);
	return temp;
}

inline String operator+(const String& lhs, char rhs)
{
	String temp(lhs);
	temp.append(rhs);
	return temp;
}

inline std::ostream& operator<<(std::ostream& out, const String& rhs)
{
	return out << rhs.c_str();
}

#endif
