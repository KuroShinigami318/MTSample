#pragma once

namespace utils
{
class DurationTracker
{
public:
	using Duration_t = std::chrono::system_clock::duration;
	using EndDurationCb = std::function<void(Duration_t)>;
	using StartTrackCb = std::function<void()>;
public:
	DurationTracker();
	DurationTracker(StartTrackCb i_startTrackCb);
	DurationTracker(StartTrackCb i_startTrackCb, EndDurationCb i_endDurationCb);
	~DurationTracker();

private:
	std::chrono::system_clock::time_point m_startTime;
	EndDurationCb m_endDurationCb;
};
}