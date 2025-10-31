/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_main.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 15:15:32 by sliziard          #+#    #+#             */
/*   Updated: 2025/10/31 18:51:14 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fstream>
#include <cassert>
#include <string>

#include "../test.h"
#include "utils/Input.hpp"

/**
 * Create a small file on disk for file reading tests
 */
static void create_test_file()
{
	std::ofstream f("input_test_file.txt");
	f << "Hello File\nSecond Line\n";
	f.close();
}

/**
 * Test all Input features
 */
int main(void)
{
	create_test_file();

	// --------------------------------------------------------------------
	sep("1. Constructors");
	{
		Input empty = Input::fromText("");
		check(empty.eof(), "Empty input reports EOF");

		Input fromText = Input::fromText("Hello world");
		check(!fromText.eof(), "String constructor works");

		Input fromFile = Input::fromFile("input_test_file.txt");
		check(!fromFile.eof(), "File constructor works");
	}

	// --------------------------------------------------------------------
	sep("2. peek() / get() / eof()");
	{
		Input in = Input::fromText("abc");
		check(in.peek() == 'a', "peek() returns first character");
		check(in.get() == 'a', "get() returns and advances");
		check(in.peek() == 'b', "peek() after get() works");
		in.get();
		in.get();
		check(in.eof(), "EOF reached after consuming all characters");
		try {
			in.get();
			check(false, "get() throws on EOF");
		} catch (const PegException &e) {
			check(true, "get() throws PegException on EOF:");
			std::cout << e.what() << std::endl;
		}
	}

	// --------------------------------------------------------------------
	sep("3. match()");
	{
		Input in = Input::fromText("foobar");
		check(in.match("foo"), "match() matches literal");
		check(!in.match("foo"), "match() fails when already advanced");
		in.setPos(0);
		check(!in.match("bar"), "match() fails on wrong literal");
		in.setPos(3);
		check(in.match("bar"), "match() works at new position");
	}

	// --------------------------------------------------------------------
	sep("4. pos() / setPos() / remaining()");
	{
		Input in = Input::fromText("abcdef");
		check(in.pos() == 0, "Initial position is 0");
		in.get();
		check(in.pos() == 1, "Position increments after get()");
		in.setPos(3);
		check(in.pos() == 3, "setPos() sets position");
		check(in.remaining() == 3, "remaining() correct");
	}

	// --------------------------------------------------------------------
	sep("5. line() / column()");
	{
		Input in = Input::fromText("abc\ndef\nghi");
		in.setPos(0); // a
		check(in.line() == 1 && in.column() == 1, "Start at line 1, col 1");
		in.setPos(3); // \n
		check(in.line() == 1 && in.column() == 4, "Column after first line");
		in.setPos(4); // d
		check(in.line() == 2 && in.column() == 1, "Start of second line");
		in.setPos(5); // e
		check(in.line() == 2 && in.column() == 2, "Mid-second line");
		in.setPos(10);
		check(in.line() == 3 && in.column() == 3, "End of third line");
	}

	// --------------------------------------------------------------------
	sep("6. substr() / context()");
	{
		Input in = Input::fromText("0123456789abcdefghij");
		check(in.substr(0, 5) == "01234", "substr() basic range");
		std::string ctx = in.context(10, 5);
		std::cout << "Context(10,5): \"" << ctx << "\"" << std::endl;
		check(ctx.find('a') != std::string::npos, "context() includes center char");
	}

	// --------------------------------------------------------------------
	sep("7. skipWhitespace()");
	{
		Input in = Input::fromText(" \t\r\n  abc");
		in.skipWhitespace();
		check(in.peek() == 'a', "skipWhitespace() skips ASCII spaces");
	}

	// --------------------------------------------------------------------
	sep("8. operator* / ++");
	{
		Input in = Input::fromText("xyz");
		check(*in == 'x', "operator* dereferences current char");
		++in;
		check(*in == 'y', "operator++ moves forward");
	}

	// --------------------------------------------------------------------
	sep("9. file reading and context()");
	{
		Input fileIn = Input::fromFile("input_test_file.txt");
		check(fileIn.peek() == 'H', "File read starts with correct char");
		fileIn.setPos(11);
		std::cout << "File context(11, 10):\n" << fileIn.context(11, 10) << std::endl;
	}

	std::cout << "\n🎉 All Input tests passed successfully!\n" << std::endl;
	return 0;
}

