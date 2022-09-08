
#include "pch.h"
#include "sqlite3/sqlite3.h"

#include "ISqliteStruct.h"
#include "DatabaseStructs.h"
#include "SqliteHelper.h"

#include <iostream>
#include <vector>

int main()
{
	SqliteHelper<DatabaseStructs::User> userHelper{ "data.db", "UserTable" };
	DatabaseStructs::User userOne{ "Alice", 20220907 };
	DatabaseStructs::User userTwo{ "Bob", 20220908 };
	DatabaseStructs::User userThree{ "Charlie", 20220909 };
	DatabaseStructs::User userFour{ "David", 20220911 };

	bool succeeded{ userHelper.openSql() };
	if (succeeded)
	{
		std::cout << "Creating table...\n";
		userHelper.createTableIfNotExists();
		userHelper.dropTable();
		userHelper.createTableIfNotExists("NewUserTable");

		std::cout << "\nInserting data...\n";
		userHelper.insertData(userOne);
		userHelper.insertData(userTwo);
		userHelper.insertData(std::vector< DatabaseStructs::User>{userThree, userFour});
			
		std::cout << "\nUpdating data...\n";
		DatabaseStructs::User realUserFour{ "David", 20220910 };
		userHelper.updateData(realUserFour, std::vector<int>{1, 1}, "20220911");

		std::cout << "\nReading data...\n";
		std::vector<DatabaseStructs::User> findUsers{ userHelper.readData(1, "20220910") };
		for (const DatabaseStructs::User& user : findUsers)
		{
			std::cout << user.toString() << '\n';
		}

		std::cout << "\nDeleting data...\n";
		userHelper.deleteData(1, "20220908");


		std::cout << "\nReading all data...\n";
		std::vector<DatabaseStructs::User> allUsers{ userHelper.readAllData() };
		for (const DatabaseStructs::User& user : allUsers)
		{
			std::cout << user.toString() << '\n';
		}
	}
	else
	{
		std::cout << "Open Sql Failed.\n";
	}
	return 0;
}