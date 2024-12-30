#pragma once
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
public:
	void RunAsync(Callback_t i_callback); // Should run very quickly as this 
	RunResult RunSync();
private:
	struct Task
	{
		using Task_t = RunResult(LongTaskMTSample::*)();
		Task_t runnable;
		Callback_t callback;
	};
private:
	void _Run();
	bool m_shutdown;
	std::mutex m_mutex;
	std::condition_variable m_cv;
	std::queue<Task> m_taskQueue;
	std::thread m_backgroundThread;
};
} // namespace Sample