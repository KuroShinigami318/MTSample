#include "stdafx.h"
#include "Samples/LongTaskMTSample.h"
#include "Samples/DeadlockSample.h"
#include "Utils/DurationTracker.h"
#include "Log.h"

#define DurationTracker(Name) \
	utils::DurationTracker runAsyncTracker( \
	[](){INFO_LOG("Measure", "{} Begin Tracking elapsed time.", Name);}, \
	[](utils::DurationTracker::Duration_t i_duration){INFO_LOG("Measure", "{} elapsed time: {}s", Name, std::chrono::duration<double>(i_duration).count());} \
	);

int main(int argc, char** argv)
{
	Sample::LongTaskMTSample longTaskSample;
	{
		DurationTracker("RunAsync");
		longTaskSample.RunAsync([](Sample::LongTaskMTSample::RunResult i_result)
		{
			const utils::Log::TextFormat textFormat(utils::Log::TextStyle::Bold, utils::RGBColor(0, 255, 0));
			INFO_LOG_WITH_FORMAT(textFormat, "Callback", "Run successfully with result: {}", i_result);
		});
	}
	{
		DurationTracker("RunSync");
		longTaskSample.RunSync();
	}

	{
		DurationTracker("TestTransactions");
		Sample::TestTransactions();
	}

	return 0;
}