#include <iostream>
#include <string>
#include "String.h"

uint test_num;
uint pass = 0;
uint fail = 0;

void assert(bool condition)
{
	std::cout << test_num++ << " - ";

	if (condition)
	{
		std::cout << "PASS\n";
		pass++;
	}
	else
	{
		std::cout << "***FAIL***\n";
		fail++;
	}
}

template <typename T>
void assert(T expected, T received)
{
	std::cout << test_num++ << " - ";

	if (expected == received)
	{
		std::cout << "PASS\n";
		pass++;
	}
	else
	{
		std::cout << " - ***FAIL*** expected: \"" << expected << "\" received \"" << received << "\"\n";
		fail++;
	}
}

void new_test_series(const char* method)
{
	std::cout << "\n" << method << "\n";
	test_num = 1;
}

int main()
{
	{
		new_test_series("String()");
		// 1-3
		const String empty;
		assert("" == empty);
		assert<uint>(0, empty.length());
		assert<uint>(1, empty.capacity());
	}

	{
		new_test_series("String(const String& str)");
		// 1-3
		const String empty;
		assert<String>("", String(empty));
		assert<uint>(0, String(empty).length());
		assert<uint>(1, String(empty).capacity());

		// 4-6
		const String one_char("a");
		assert<String>("a", String(one_char));
		assert<uint>(1, String(one_char).length());
		assert<uint>(1, String(one_char).capacity());

		// 7-9
		const String test("this is a test");
		assert<String>("this is a test", String(test));
		assert<uint>(14, String(test).length());
		assert<uint>(14, String(test).capacity());
	}

	{
		new_test_series("String(const String& str, uint pos, uint len = npos)");
		// 1-3
		const String a("a");
		const String empty(a, 0, 0);
		assert<String>("", empty);
		assert<uint>(0, empty.length());
		assert<uint>(0, empty.capacity());

		// 4-6
		const String one_char(a, 0, 1);
		assert<String>("a", one_char);
		assert<uint>(1, one_char.length());
		assert<uint>(1, one_char.capacity());

		const String test("this is a test");

		// 7-9
		const String all_of_it(test, 0);
		assert<String>("this is a test", all_of_it);
		assert<uint>(14, all_of_it.length());
		assert<uint>(14, all_of_it.capacity());

		// 10-12
		const String past_end(test, 0, 100);
		assert<String>("this is a test", past_end);
		assert<uint>(14, past_end.length());
		assert<uint>(14, past_end.capacity());

		// 13-15
		const String partial(test, 3, 10);
		assert<String>("s is a tes", partial);
		assert<uint>(10, partial.length());
		assert<uint>(10, partial.capacity());

		// 16
		String error;

		try
		{
			const String bad_pos(test, 20);
		}
		catch (const String& e)
		{
			error = e;
		}

		assert<String>("pos cannot be greater than str.length()", error);
	}

	{
		new_test_series("String(const char* s)");
		// 1-3
		const String empty("");
		assert<String>("", empty);
		assert<uint>(0, empty.length());
		assert<uint>(0, empty.capacity());

		// 4-6
		const String one_char("a");
		assert<String>("a", one_char);
		assert<uint>(1, one_char.length());
		assert<uint>(1, one_char.capacity());

		// 7-9
		const String test("this is a test");
		assert<String>("this is a test", test);
		assert<uint>(14, test.length());
		assert<uint>(14, test.capacity());
	}

	{
		new_test_series("String(const char* s, uint n)");

		// 1-3
		String zero_test("a", 0);
		assert<String>("", zero_test);
		assert<uint>(0, zero_test.length());
		assert<uint>(0, zero_test.capacity());

		// 4-6
		String one_test("a", 1);
		assert<String>("a", one_test);
		assert<uint>(1, one_test.length());
		assert<uint>(1, one_test.capacity());

		// 7-9
		String long_test("this is a test", 4);
		assert<String>("this", long_test);
		assert<uint>(4, long_test.length());
		assert<uint>(4, long_test.capacity());
	}

	{
		new_test_series("String(uint n, char c)");

		// 1-3
		String zero_test(0, 'a');
		assert<String>("", zero_test);
		assert<uint>(0, zero_test.length());
		assert<uint>(0, zero_test.capacity());

		// 4-6
		String one_test(1, 'a');
		assert<String>("a", one_test);
		assert<uint>(1, one_test.length());
		assert<uint>(1, one_test.capacity());

		// 7-9
		String long_test(4, 't');
		assert<String>("tttt", long_test);
		assert<uint>(4, long_test.length());
		assert<uint>(4, long_test.capacity());
	}

	{
		new_test_series("String(const std::string& rhs)");

		// 1-3
		std::string empty("");
		String empty_test(empty);
		assert<String>("", empty_test);
		assert<uint>(0, empty_test.length());
		assert<uint>(0, empty_test.capacity());

		// 4-6
		std::string one_char("a");
		String one_test(one_char);
		assert<String>("a", one_test);
		assert<uint>(1, one_test.length());
		assert<uint>(1, one_test.capacity());

		// 7-9
		std::string test("this is a test");
		String long_test(test);
		assert<String>("this is a test", long_test);
		assert<uint>(14, long_test.length());
		assert<uint>(14, long_test.capacity());
	}

	{
		new_test_series("String(uint capacity)");

		// 1-3
		String empty(0u);
		assert<String>("", empty);
		assert<uint>(0, empty.length());
		assert<uint>(0, empty.capacity());

		// 4-6
		String one(1u);
		assert<String>("", one);
		assert<uint>(0, one.length());
		assert<uint>(1, one.capacity());

		// 7-9
		String ten(10u);
		assert<String>("", ten);
		assert<uint>(0, ten.length());
		assert<uint>(10, ten.capacity());
	}


	// Print results
	std::cout << "\n===Test Results===\n";
	std::cout << "Pass: " << pass << "\n";
	std::cout << "Fail: " << fail << "\n";

	return fail;	
}