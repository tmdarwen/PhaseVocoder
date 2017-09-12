#include <gtest/gtest.h>
#include <Signal/Windowing.h>
#include <Signal/SignalConversion.h>
#include <vector>

TEST(WindowingTests, BlackmanWindowTestFullWindow)
{
	std::vector<double> signal(1024, 1.0);
	Signal::BlackmanWindow(signal);	
	Signal::ReverseBlackmanWindow(signal);	

	for(auto sample : signal)
	{
		EXPECT_NEAR(1.0, sample, 0.0001);
	}
}

TEST(WindowingTests, BlackmanWindowTest0To25)
{
	std::vector<double> signal(2048, 1.0);
	Signal::BlackmanWindow(signal, 0.0, 25.0);	
	Signal::ReverseBlackmanWindow(signal, 0.0, 25.0);	

	auto signal16{Signal::ConvertFloat64ToSigned16(signal)};

	for(auto sample : signal)
	{
		EXPECT_NEAR(1.0, sample, 0.0001);
	}
}

TEST(WindowingTests, BlackmanWindowTest25To75)
{
	std::vector<double> signal(256, 1.0);
	Signal::BlackmanWindow(signal, 25.0, 75.0);	
	Signal::ReverseBlackmanWindow(signal, 25.0, 75.0);	

	for(auto sample : signal)
	{
		EXPECT_NEAR(1.0, sample, 0.0001);
	}
}

TEST(WindowingTests, BlackmanWindowTest75To100)
{
	std::vector<double> signal(256, 1.0);
	Signal::BlackmanWindow(signal, 75.0, 100.0);	
	Signal::ReverseBlackmanWindow(signal, 75.0, 100.0);	

	for(auto sample : signal)
	{
		EXPECT_NEAR(1.0, sample, 0.0001);
	}
}

TEST(WindowingTests, LinearFadeInOutEvenTest)
{
	std::vector<double> signal(1024, 1.0);
	Signal::LinearFadeInOut(signal);	

	EXPECT_EQ(1024, signal.size());

	EXPECT_NEAR(0.0, signal[0], 0.01);
	EXPECT_NEAR(0.25, signal[127], 0.01);
	EXPECT_NEAR(0.5, signal[255], 0.01);
	EXPECT_NEAR(0.75, signal[383], 0.01);
	EXPECT_NEAR(1.0, signal[511], 0.01);
	EXPECT_NEAR(1.0, signal[512], 0.01);
	EXPECT_NEAR(0.75, signal[639], 0.01);
	EXPECT_NEAR(0.5, signal[767], 0.01);
	EXPECT_NEAR(0.25, signal[895], 0.01);
	EXPECT_NEAR(0.0, signal[1023], 0.01);
}

TEST(WindowingTests, LinearFadeInOutOddTest)
{
	std::vector<double> signal(11, 1.0);
	Signal::LinearFadeInOut(signal);	

	EXPECT_EQ(11, signal.size());

	EXPECT_NEAR(0.00, signal[0], 0.00001);
	EXPECT_NEAR(0.20, signal[1], 0.00001);
	EXPECT_NEAR(0.40, signal[2], 0.00001);
	EXPECT_NEAR(0.60, signal[3], 0.00001);
	EXPECT_NEAR(0.80, signal[4], 0.00001);
	EXPECT_NEAR(1.00, signal[5], 0.00001);
	EXPECT_NEAR(0.80, signal[6], 0.00001);
	EXPECT_NEAR(0.60, signal[7], 0.00001);
	EXPECT_NEAR(0.40, signal[8], 0.00001);
	EXPECT_NEAR(0.20, signal[9], 0.00001);
	EXPECT_NEAR(0.00, signal[10], 0.00001);
}

TEST(WindowingTests, LinearFadeInOutAttenuatedSignal)
{
	std::vector<double> signal(11, 0.75);
	Signal::LinearFadeInOut(signal);	

	EXPECT_EQ(11, signal.size());

	EXPECT_NEAR(0.00, signal[0], 0.00001);
	EXPECT_NEAR(0.15, signal[1], 0.00001);
	EXPECT_NEAR(0.30, signal[2], 0.00001);
	EXPECT_NEAR(0.45, signal[3], 0.00001);
	EXPECT_NEAR(0.60, signal[4], 0.00001);
	EXPECT_NEAR(0.75, signal[5], 0.00001);
	EXPECT_NEAR(0.60, signal[6], 0.00001);
	EXPECT_NEAR(0.45, signal[7], 0.00001);
	EXPECT_NEAR(0.30, signal[8], 0.00001);
	EXPECT_NEAR(0.15, signal[9], 0.00001);
	EXPECT_NEAR(0.00, signal[10], 0.00001);
}