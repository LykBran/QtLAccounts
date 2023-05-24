#include "AccountsManager.h"

#include <iostream>
#include <sstream>
#include <stdexcept>

using namespace std;

void AccountsManager::get_accounts()
{
    streampos pos = f.tellg();
    f.clear();
    f.seekg(ios_base::beg);
    string line;
    accounts.clear();
    while (getline(f, line))
    {
        stringstream ss;
        ss << line;
        Account acc;
        ss >> acc.name >> acc.username >> acc.password;
        accounts.push_back(acc);
    }
    f.seekg(pos);
}

vector<Account> AccountsManager::all_accounts()
{
    get_accounts();
    return accounts;
}

void AccountsManager::write_account(Account account)
{
    streampos pos = f.tellp();
    f.clear();
    f.seekp(ios_base::end);
    f << account.name << ' ' << account.username << ' ' << account.password << endl;
    f.seekp(pos);
    get_accounts();
}

void AccountsManager::erase_account(size_t index)
{
    size_t linenum = countLines(filename);
    if (index > linenum)
    {
        throw std::out_of_range("The index should be smaller than the number of the accounts.");
    }
    streampos posg = f.tellg(), posp = f.tellp();
    f.clear();
    ofstream outfile("tmp");
    for (size_t n = 0; n < linenum; ++n)
    {
        if (n != index - 1)
        {
            outfile << accounts[n] << endl;
        }
    }
    f.close();
    f.clear();
    outfile.close();
    remove(filename.c_str());
    rename("tmp", filename.c_str());
    f.open(filename, ios_base::in | ios_base::out | ios_base::app);
    f.seekg(posg);
    f.seekp(posp);
    get_accounts();
}

void AccountsManager::edit_account(size_t index, Account new_account)
{
    size_t linenum = countLines(filename);
    if (index > linenum)
    {
        throw std::out_of_range("The index should be smaller than the number of the accounts.");
    }
    streampos posg = f.tellg(), posp = f.tellp();
    f.clear();
    ofstream outfile("tmp");
    for (size_t n = 0; n < linenum; ++n)
    {
        if (n == index - 1)
        {
            outfile << new_account << endl;
        }
        else
        {
            outfile << accounts[n] << endl;
        }
    }
    f.close();
    f.clear();
    outfile.close();
    remove(filename.c_str());
    rename("tmp", filename.c_str());
    f.open(filename, ios_base::in | ios_base::out | ios_base::app);
    f.seekg(posg);
    f.seekp(posp);
    get_accounts();
}

size_t AccountsManager::countLines(string filename)
{
    ifstream inf(filename, ios_base::in);
    string tmp;
    size_t n = 0;
    while (getline(inf, tmp, '\n'))
    {
        ++n;
    }
    return n;
}

AccountsManager::AccountsManager(const string& file)
{
    f.open(file, ios_base::in | ios_base::out | ios_base::app);
    filename = file;
    get_accounts();
}