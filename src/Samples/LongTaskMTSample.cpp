#include "stdafx.h"
#include "Samples/LongTaskMTSample.h"
#include "Log.h"

namespace Sample
{
LongTaskMTSample::LongTaskMTSample()
	: m_taskLooper(*this, m_mutex, m_cv)
	, m_backgroundThread(&LongTaskMTSample::_Run, this)
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
	m_taskLooper.Run([this]() { return m_shutdown; });
}

void LongTaskMTSample::RunAsync(Callback_t i_callback)
{
	std::lock_guard lk(m_mutex);
	m_taskLooper.Push({ &LongTaskMTSample::RunSync, i_callback });
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