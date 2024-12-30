#include "stdafx.h"
#include "Utils/DurationTracker.h"
#include "Log.h"

namespace utils
{
void DefaultStartTrackCb()
{
	INFO_LOG("Measure", "Begin Tracking elapsed time.");
}

void DefaultEndDurationCb(DurationTracker::Duration_t i_duration)
{
	INFO_LOG("Measure", "Elapsed time: {}s", std::chrono::duration<double>(i_duration).count());
}

DurationTracker::DurationTracker()
	: DurationTracker(DefaultStartTrackCb, DefaultEndDurationCb)
{
}

DurationTracker::DurationTracker(StartTrackCb i_startTrackCb)
	: DurationTracker(i_startTrackCb, DefaultEndDurationCb)
{
}

DurationTracker::DurationTracker(StartTrackCb i_startTrackCb, EndDurationCb i_endDurationCb)
	: m_startTime(std::chrono::system_clock::now())
	, m_endDurationCb(i_endDurationCb)
{
	i_startTrackCb();
}

DurationTracker::~DurationTracker()
{
	auto elapsed = std::chrono::system_clock::now() - m_startTime;
	m_endDurationCb(elapsed);
}
}