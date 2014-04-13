#ifndef STRING_H
#define STRING_H

#include "namespace.h"
#include <iostream>
#include <cstring>

class String
{
public:
	String() :
		capacity_(0),
		length_(0),
		buf_(NULL),
		c_str_(NULL)
	{ }

	String(const String& rhs) :
		capacity_(rhs.capacity_),
		length_(rhs.length_),
		buf_(new char[rhs.capacity_]),
		c_str_(NULL)
	{
		memcpy(buf_, rhs.buf_, length_);
	}

	String(const char* rhs) :
		capacity_(strlen(rhs)),
		length_(capacity_),
		buf_(new char[capacity_]),
		c_str_(NULL)
	{
		memcpy(buf_, rhs, length_);
	}

	String(const char* rhs, uint length) :
		capacity_(length + 1),
		length_(length),
		buf_(new char[capacity_]),
		c_str_(NULL)
	{
		memcpy(buf_, rhs, length_);
	}

	String(const std::string& rhs) :
		capacity_(rhs.capacity()),
		length_(rhs.length()),
		buf_(new char[capacity_]),
		c_str_(NULL)
	{
		memcpy(buf_, rhs.c_str(), length_);
	}

	~String()
	{
		free();
	}

	void free()
	{
		// printf("freeing: %x\n", str_);
		if (buf_ != NULL)
		{
			delete[] buf_;
			buf_ = NULL;
		}

		if (c_str_ != NULL)
		{
			delete[] c_str_;
			c_str_ = NULL;
		}
	}

	bool operator==(const String& rhs) const
	{
		if (length_ != rhs.length_)
		{
			return false;
		}

		for (uint i = 0; i < length_; i++)
		{
			if (buf_[i] != rhs.buf_[i])
			{
				return false;
			}	
		}

		return true;
	}

	bool operator!=(const String& rhs) const
	{
		return !(*this == rhs);
	}

	String& operator=(const String& rhs)
	{
		if (this != &rhs)
		{
			free();
			capacity_ = rhs.capacity_;
			length_ = rhs.length_;
			buf_ = new char[capacity_];
			c_str_ = rhs.c_str_;
			memcpy(buf_, rhs.buf_, length_);
		}

		return *this;
	}

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
		free();
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
