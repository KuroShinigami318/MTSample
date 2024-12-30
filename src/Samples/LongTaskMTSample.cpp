#include "stdafx.h"
#include "Samples/LongTaskMTSample.h"
#include "Log.h"

namespace Sample
{
LongTaskMTSample::LongTaskMTSample()
	: m_backgroundThread(&LongTaskMTSample::_Run, this)
	, m_shutdown(false)
{
}

LongTaskMTSample::~LongTaskMTSample()
{
	{
		std::lock_guard lk(m_mutex);
		m_shutdown = true;
		m_cv.notify_one();
	}
	m_backgroundThread.join();
}

void LongTaskMTSample::_Run()
{
	while (true)
	{
		std::unique_lock lk(m_mutex);
		m_cv.wait(lk, [this]() { return !m_taskQueue.empty() || m_shutdown; });
		if (m_shutdown && m_taskQueue.empty()) return;

		Task task = m_taskQueue.front();
		m_taskQueue.pop();
		lk.unlock();
		task.callback((this->*task.runnable)());
	}
}

void LongTaskMTSample::RunAsync(Callback_t i_callback)
{
	std::unique_lock lk(m_mutex);
	m_taskQueue.push({ &LongTaskMTSample::RunSync, i_callback });
	m_cv.notify_one();
}

LongTaskMTSample::RunResult LongTaskMTSample::RunSync()
{
	WARNING_LOG("LongTaskMTSample::RunSync", "Running long task");
	// fake long running task
	std::this_thread::sleep_for(std::chrono::seconds(1));
	return 1;
}
}