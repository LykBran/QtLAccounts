#pragma once

#include <fstream>
#include <string>
#include <vector>

struct Account
{
    std::string name, username, password;
};

template <class _Elem, class _Traits>
std::basic_ostream<_Elem, _Traits>& operator<<(std::basic_ostream<_Elem, _Traits>& os, const Account& account)
{
    os << account.name << ' ' << account.username << ' ' << account.password;
    return os;
}

class AccountsManager
{
protected:
    std::vector<Account> accounts;
    std::string filename;
    std::fstream f;

public:
    void get_accounts();
    std::vector<Account> all_accounts();
    void write_account(Account account);
    void erase_account(size_t index);
    void edit_account(size_t index, Account new_account);

    static size_t countLines(std::string filename);
    AccountsManager(const std::string& file = "accounts.lad");
    ~AccountsManager() = default;
};
