// ConsoleApplication9.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include "DataSource.h"
#include "CustomPricing.h"
#include "Product.h"
#include "Checkout.h"

//uncomment this macro to test code
//#define DO_TESTS

#ifdef DO_TESTS
#define CATCH_CONFIG_MAIN
#include "catch2.hpp"

//Test names are self-explanatory

TEST_CASE("Check price sum")
{
	auto ds = DataSource();
	ds.LoadTestData();

	auto co = Checkout(ds);

	co.Scan("A1");
	co.Scan("A2");
	co.Scan("A3");
	co.Scan("A4");
	co.Scan("A5");
	co.Void("A2");

	REQUIRE(int(co.getTotal() * 100) == int(16.96 * 100));
}

TEST_CASE("Check exception on incorrect item")
{
	auto ds = DataSource();
	ds.LoadTestData();

	auto co = Checkout(ds);

	co.Scan("A1");

	REQUIRE_THROWS(co.Scan("B2"));
}

TEST_CASE("Check packs")
{
	auto ds = DataSource();
	ds.LoadTestData();

	auto co = Checkout(ds);

	co.Scan("A3");
	co.Scan("A3");
	co.Scan("A3");
	co.Scan("A3");
	co.Scan("A3");
	co.Scan("A4");
	co.Scan("A4");
	co.Scan("A4");
	co.Scan("A4");

	REQUIRE(int(co.getTotal() * 100) == int(28.95 * 100));
}

TEST_CASE("Check incorrect void")
{
	auto ds = DataSource();
	ds.LoadTestData();

	auto co = Checkout(ds);
	co.Scan("A3");
	co.Void("A3");

	REQUIRE_THROWS(co.Void("A3"));
}


#else //DO_TESTS

int main()
{
	DataSource ds;
	ds.LoadData();

	//Prints pricing tables
	for (auto i : ds.getProducts())
	{
		printf("%s %.2fGBP\n", i.first.c_str(), i.second.getPrice());
	}

	for (auto i : ds.getCustomPricings())
	{
		printf("%s packsize %i costs %.2fGBP\n", i.first.c_str(), i.second.getReqCount(), i.second.getPackPrice());
	}

	auto co = Checkout(ds);
	co.printAllItems();

	co.Scan("A1");
	co.Scan("A2");
	co.Scan("A1");

	printf("Subtotal: %.2fGBP\n", co.getTotal());

	co.Scan("A2");
	co.Scan("A2");
	co.Scan("A3");

	co.Scan("A4");
	co.Scan("A2");
	co.Scan("A4");

	co.Void("A2");

	co.printAllItems();
	printf("Total: %.2fGBP\n", co.getTotal());
}
#endif //DO_TESTS
