#include "Samples/DeadlockSample.h"
#include "Log.h"

namespace Sample
{
BankAccount::BankAccount(std::string i_name, int i_currentBalance)
	: name(i_name), currentBalance(i_currentBalance)
{
}

void BankAccount::Add(int i_transactionValue)
{
	currentBalance += i_transactionValue; // solution-2: comment this
	//currentBalance.fetch_add(i_transactionValue, std::memory_order_relaxed); // solution-2: uncomment-this
}

void BankAccount::Substract(int i_transactionValue)
{
	currentBalance -= i_transactionValue; // solution-2: comment this
	//currentBalance.fetch_sub(i_transactionValue, std::memory_order_relaxed); // solution-2: uncomment-this
}

std::string BankAccount::GetName() const
{
	return name;
}

int BankAccount::GetCurrentBalance() const
{
	return currentBalance;
}

void Notify(BankAccount&)
{
	 // Simulate a time-consuming messaging operation
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

void DoTransaction(BankAccount& i_fromAccount, BankAccount& i_toAccount, int i_transactionValue)
{
	INFO_LOG("transaction", "before doing transaction for: {} - {}", i_fromAccount.GetName(), i_toAccount.GetName());
	{
		//std::scoped_lock lock(i_fromAccount.mutex, i_toAccount.mutex); // solution-1: uncomment this

		std::lock_guard lkFrom(i_fromAccount.mutex); // solution-1, solution-2: comment this
		i_fromAccount.Substract(i_transactionValue);
		Notify(i_fromAccount);

		std::lock_guard lkTo(i_toAccount.mutex); // solution-1, solution-2: comment this
		i_toAccount.Add(i_transactionValue);
		Notify(i_toAccount);

		INFO_LOG("transaction", "after doing transaction for: {}: {} - {}: {}", i_fromAccount.GetName(), i_fromAccount.GetCurrentBalance(), i_toAccount.GetName(), i_toAccount.GetCurrentBalance());
	}
}

void TestTransactions()
{
	std::vector<std::thread> threads;
	Sample::BankAccount john("John", 100), jack("Jack", 200), foo("Foo", 300), bar("Bar", 400);

	threads.emplace_back(Sample::DoTransaction, std::ref(jack), std::ref(john), 50);
	threads.emplace_back(Sample::DoTransaction, std::ref(john), std::ref(foo), 50);
	threads.emplace_back(Sample::DoTransaction, std::ref(foo), std::ref(jack), 50);
	threads.emplace_back(Sample::DoTransaction, std::ref(bar), std::ref(jack), 50);

	for (std::thread& thread : threads)
	{
		thread.join();
	}
}
}