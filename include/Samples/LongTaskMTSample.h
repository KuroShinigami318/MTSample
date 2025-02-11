#pragma once
#include "Utils/TaskLooper.h"
#include <condition_variable>
#include <mutex>
#include <thread>

namespace Sample
{
class LongTaskMTSample
{
public:
	LongTaskMTSample();
	~LongTaskMTSample();
	using RunResult = int;
	using Callback_t = std::function<void(const RunResult& i_result)>;
	using TaskLooper_t = utils::TaskLooper<RunResult(LongTaskMTSample::*)(), Callback_t>;
public:
	void RunAsync(Callback_t i_callback); // Should run very quickly as this 
	RunResult RunSync();;
private:
	void _Run();
	bool m_shutdown;
	std::mutex m_mutex;
	std::condition_variable m_cv;
	TaskLooper_t m_taskLooper;
	std::thread m_backgroundThread;
};
} // namespace Sample