#ifndef STRING_H
#define STRING_H

#include "namespace.h"
#include <iostream>
#include <cstring>

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
		buf_[0] = '\0';
	}

	String(const String& rhs) :
		capacity_(rhs.capacity_),
		length_(rhs.length_),
		buf_(new char[rhs.capacity_ + 1])
	{
		memcpy(buf_, rhs.buf_, length_);
	}

	String(const char* rhs, uint pos = 0, uint length = npos) :
		capacity_(length == npos ? strlen(rhs) : length),
		length_(capacity_),
		buf_(new char[capacity_ + 1])
	{
		memcpy(buf_, rhs + pos, length_);
	}

	String(const std::string& rhs) :
		capacity_(rhs.capacity()),
		length_(rhs.length()),
		buf_(new char[capacity_ + 1])
	{
		memcpy(buf_, rhs.c_str(), length_);
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

// PICK UP HERE


	String operator+(const String& rhs) const
	{
		String str(*this);
		str.append(rhs);
		return str;
	}

	String operator+(const std::string& rhs) const
	{
		String str(*this);
		str += rhs.c_str();
		return str;
	}

	String operator+(const char* rhs) const
	{
		String str(*this);
		str += rhs;
		return str;
	}

	String operator+(char rhs) const
	{
		String str(*this);
		str += rhs;
		return str;
	}

	String& operator+=(const char* rhs)
	{
		append(rhs);
		return *this;
	}

	String& operator+=(char rhs)
	{
		append(rhs);
		return *this;
	}

	String& operator+=(const String& rhs)
	{
		append(rhs);
		return *this;
	}

	template <class T>
	void swap (T& a, T& b)
	{
		T c(a);
		a = b;
		b = c;
	}

	void swap(String& rhs)
	{
		swap(buf_, rhs.buf_);
		swap(capacity_, rhs.capacity_);
		swap(length_, rhs.length_);
		swap(c_str_, rhs.c_str_);
	}

	void append(const String& rhs)
	{
		uint len = rhs.length_;

		if (length_ + len >= capacity_)
		{
			do
			{
				capacity_ = (capacity_ ? capacity_ * 2 : 1);
			}
			while (length_ + len >= capacity_);

			char* buf = new char[capacity_];

			if (buf_ != NULL)
			{
				memcpy(buf, buf_, length_);
				delete[] buf_;
			}

			buf_ = buf;
		}

		memcpy(buf_ + length_, rhs.buf_, len);
		length_ += len;	
	}

	void append(const char* c_str)
	{
		uint len = strlen(c_str);

		if (length_ + len >= capacity_)
		{
			do
			{
				capacity_ = (capacity_ ? capacity_ * 2 : 1);
			}
			while (length_ + len >= capacity_);

			char* buf = new char[capacity_];

			if (buf_ != NULL)
			{
				memcpy(buf, buf_, length_);
				delete[] buf_;
			}

			buf_ = buf;
		}

		memcpy(buf_ + length_, c_str, len);
		length_ += len;
	}

	void append(char Char)
	{
		if (length_ >= capacity_)
		{
			do
			{
				capacity_ = (capacity_ ? capacity_ * 2 : 1);
			}
			while (length_ >= capacity_);

			char* buf = new char[capacity_];

			if (buf_ != NULL)
			{
				// std::cout << "problem?\n";
				memcpy(buf, buf_, length_);
				// std::cout << "no problem yet " << str_[0] << "\n";
				// printf("freeing: %x\n", str_);
				delete[] buf_;
				// std::cout << "no problem\n";
			}

			buf_ = buf;
			// printf("allocated: %x\n", str_);
		}

		buf_[length_++] = Char;
	}

	void reserve(uint capacity)
	{
		if (capacity <= capacity_)
		{
			throw std::string("String Error: Can not reserve less than string's current capacity.");
			return;
		}

		capacity_ = capacity;
		char* temp = buf_;
		buf_ = new char[capacity_];
		// printf("allocated: %x\n", buf_);
		
		if (temp != NULL)
		{
			memcpy(buf_, temp, length_);
			delete[] temp;
			// printf("freeing: %x\n", temp);
		}
	}

	void resize(int length)
	{
		String temp(buf_, length);
		swap(temp);
	}

	void minimize()
	{
		resize(length_);
	}

	// Note: c_str() can be modified by further calls to String
	const char* c_str() const
	{
		if (c_str_ != NULL)
		{
			delete[] c_str_;
		}

		c_str_ = new char[length_ + 1];
		memcpy(c_str_, buf_, length_);
		c_str_[length_] = '\0';
		return c_str_;
	}

	void clear()
	{
		// free(); // TODO: Fix this function
		length_ = 0;
		capacity_ = 0;
	}

	uint length() const
	{
		return length_;
	}

	uint capacity() const
	{
		return capacity_;
	}

public:
	uint capacity_;
	uint length_;
	char* buf_;
	mutable char* c_str_;
};

inline String operator+(const char* rhs, const String& lhs)
{
	return String(rhs) + lhs;
}

inline std::ostream& operator<<(std::ostream& out, const String& rhs)
{
	return out << rhs.c_str();
}

#endif
