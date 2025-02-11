#pragma once

namespace Sample
{
struct BankAccount
{
	std::mutex mutex;
public:
	BankAccount(std::string i_name, int i_currentBalance);
	void Add(int i_transactionValue);
	void Substract(int i_transactionValue);
	std::string GetName() const;
	int GetCurrentBalance() const;

private:
	std::string name;
	int currentBalance = 0; // solution-2: replace int type with std::atomic<int> or std::atomic_int
};

void TestTransactions();
}