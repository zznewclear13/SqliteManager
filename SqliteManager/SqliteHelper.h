#pragma once

#include "ISqliteStruct.h"
#include "DatabaseStructs.h"
#include "sqlite3/sqlite3.h"

#include <iostream>
#include <vector>

// Use SqliteHelper<ISqliteStuct> for sql operations.
SQLITE_STRUCT_TEMPLATE
class SqliteHelper
{
private:
	std::string m_DatabaseName;
	std::string m_TableName;
	sqlite3* m_Database = nullptr;
	char* m_ErrorMessage = nullptr;

public:
	SqliteHelper(const std::string& databaseName, const std::string& tableName);

	std::string getDatabaseName() const { return m_DatabaseName; }
	std::string getTableName() const { return m_TableName; }

	// Open sql.
	bool openSql();
	// Close sql.
	void closeSql();

	// Create table with current tableName if not exists.
	void createTableIfNotExists();
	// Change current table name, and create if not exist.
	void createTableIfNotExists(const std::string& tableName);
	// Drop current table.
	void dropTable();

	// Insert data to current table.
	void insertData(const T& data);
	// Insert a list of data to current table.
	void insertData(const std::vector<T>& dataList);

	// Update data if data property at propertyIDs[0] matches propertyValueStr,
	// properties after propertyIDs[1] will be updated to data's properties.
	void updateData(const T& data, const std::vector<int>& propertyIDs, const std::string& propertyValueStr);

	// Delete data if data property at propertyID matches propertyValueStr.
	void deleteData(int propertyID, const std::string& propertyValueStr);

	// Read data if data property at propertyID matches propertyValueStr.
	std::vector<T> readData(int propertyID, const std::string& propertyValueStr);
	// Read all data from current table.
	std::vector<T> readAllData();

	// For other operations.
	void customCommand(const std::string& command, int(*callback)(void*, int, char**, char**), void* data)
};

SQLITE_STRUCT_TEMPLATE
SqliteHelper<T>::SqliteHelper(const std::string& databaseName, const std::string& tableName)
	: m_DatabaseName{databaseName}, m_TableName{tableName}
{

}

SQLITE_STRUCT_TEMPLATE
bool SqliteHelper<T>::openSql()
{
	int rc{ sqlite3_open_v2(m_DatabaseName.c_str(), &m_Database, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, m_ErrorMessage)};
	return rc == SQLITE_OK;
}

SQLITE_STRUCT_TEMPLATE
void SqliteHelper<T>::closeSql()
{
	sqlite3_close_v2(m_Database);
}

SQLITE_STRUCT_TEMPLATE
void SqliteHelper<T>::createTableIfNotExists()
{
	std::string createStr{ "CREATE TABLE IF NOT EXISTS " + m_TableName
							+ " (" + SqliteStruct<T>::getTableFormat() + ");" };
	int rc{ sqlite3_exec(m_Database, createStr.c_str(), nullptr, nullptr, &m_ErrorMessage) };
	if (rc != SQLITE_OK)
	{
		std::cout << "SQL ERROR: " << m_ErrorMessage << '\n';
		sqlite3_free(m_ErrorMessage);
	}
}

SQLITE_STRUCT_TEMPLATE
void SqliteHelper<T>::createTableIfNotExists(const std::string& tableName)
{
	m_TableName = tableName;
	createTableIfNotExists();
}

SQLITE_STRUCT_TEMPLATE
void SqliteHelper<T>::dropTable()
{
	std::string dropStr{ "DROP TABLE " + m_TableName + ";" };
	int rc{ sqlite3_exec(m_Database, dropStr.c_str(), nullptr, nullptr, &m_ErrorMessage) };
	if (rc != SQLITE_OK)
	{
		std::cout << "SQL ERROR: " << m_ErrorMessage << '\n';
		sqlite3_free(m_ErrorMessage);
	}
}

SQLITE_STRUCT_TEMPLATE
void SqliteHelper<T>::insertData(const T& data)
{
	std::string insertStr{ "INSERT INTO " + m_TableName
							+ " (" + SqliteStruct<T>::getTableColumns() + ") VALUES ("
							+ data.getTableValues() + ");" };
	int rc{ sqlite3_exec(m_Database, insertStr.c_str(), nullptr, nullptr, &m_ErrorMessage) };
	if (rc != SQLITE_OK)
	{
		std::cout << "SQL ERROR: " << m_ErrorMessage << '\n';
		sqlite3_free(m_ErrorMessage);
	}
}

SQLITE_STRUCT_TEMPLATE
void SqliteHelper<T>::insertData(const std::vector<T>& dataList)
{
	int dataCount{ static_cast<int>(dataList.size()) };
	std::string valueStr{ "" };
	for (int i{ 0 }; i < dataCount; ++i)
	{
		valueStr += "(" + dataList[i].getTableValues() + ")";
		if (i != dataCount - 1) valueStr += ", ";
	}

	std::string insertStr{ "INSERT INTO " + m_TableName
							+ " (" + SqliteStruct<T>::getTableColumns() + ") VALUES "
							+ valueStr + ";" };
	int rc{ sqlite3_exec(m_Database, insertStr.c_str(), nullptr, nullptr, &m_ErrorMessage) };
	if (rc != SQLITE_OK)
	{
		std::cout << "SQL ERROR: " << m_ErrorMessage << '\n';
		sqlite3_free(m_ErrorMessage);
	}
}

SQLITE_STRUCT_TEMPLATE
void SqliteHelper<T>::updateData(const T& data, const std::vector<int>& propertyIDs, const std::string& propertyValueStr)
{
	std::string propertyStr{ " SET " };
	int propertyIDCount = static_cast<int>(propertyIDs.size());
	for (int i{ 1 }; i < propertyIDCount; ++i)
	{
		propertyStr += SqliteStruct<T>::getPropertyName(propertyIDs[i]) + "="
			+ data.getPropertyValue(propertyIDs[i]);
		if (i != propertyIDCount - 1) propertyStr += ", ";
	}

	std::string updateStr{ "UPDATE " + m_TableName
		+ propertyStr
		+ " WHERE " + SqliteStruct<T>::getPropertyName(propertyIDs[0]) + "="
		+ propertyValueStr + ";" };

	int rc{ sqlite3_exec(m_Database, updateStr.c_str(), nullptr, nullptr, &m_ErrorMessage) };
	if (rc != SQLITE_OK)
	{
		std::cout << "SQL ERROR: " << m_ErrorMessage << '\n';
		sqlite3_free(m_ErrorMessage);
	}
}

SQLITE_STRUCT_TEMPLATE
void SqliteHelper<T>::deleteData(int propertyID, const std::string& propertyValueStr)
{
	std::string deleteStr{ "DELETE FROM " + m_TableName
		+ " WHERE " + SqliteStruct<T>::getPropertyName(propertyID) + "="
		+ propertyValueStr + ";" };

	int rc{ sqlite3_exec(m_Database, deleteStr.c_str(), nullptr, nullptr, &m_ErrorMessage) };
	if (rc != SQLITE_OK)
	{
		std::cout << "SQL ERROR: " << m_ErrorMessage << '\n';
		sqlite3_free(m_ErrorMessage);
	}
}

SQLITE_STRUCT_TEMPLATE
std::vector<T> SqliteHelper<T>::readData(int propertyID, const std::string& propertyValueStr)
{
	std::string selectStr{ "SELECT * FROM " + m_TableName
		+ " WHERE " + SqliteStruct<T>::getPropertyName(propertyID) + "="
		+ propertyValueStr + ";" };

	std::vector<T> dataList{};
	int rc{ sqlite3_exec(m_Database, selectStr.c_str(), SqliteStruct<T>::sqlReadCallback, reinterpret_cast<void*>(&dataList), &m_ErrorMessage)};	
	if (rc != SQLITE_OK)
	{
		std::cout << "SQL ERROR: " << m_ErrorMessage << '\n';
		sqlite3_free(m_ErrorMessage);
	}
	return dataList;
}

SQLITE_STRUCT_TEMPLATE
std::vector<T> SqliteHelper<T>::readAllData()
{
	std::string selectStr{ "SELECT * FROM " + m_TableName + ";" };

	std::vector<T> dataList{};
	int rc{ sqlite3_exec(m_Database, selectStr.c_str(), SqliteStruct<T>::sqlReadCallback, reinterpret_cast<void*>(&dataList), &m_ErrorMessage) };
	if (rc != SQLITE_OK)
	{
		std::cout << "SQL ERROR: " << m_ErrorMessage << '\n';
		sqlite3_free(m_ErrorMessage);
	}
	return dataList;
}

SQLITE_STRUCT_TEMPLATE
void SqliteHelper<T>::customCommand(const std::string& command, int(*callback)(void*, int, char**, char**), void* data)
{
	int rc{ sqlite3_exec(m_Database, command.c_str(), callback, data, &m_ErrorMessage) };
	if (rc != SQLITE_OK)
	{
		std::cout << "SQL ERROR: " << m_ErrorMessage << '\n';
		sqlite3_free(m_ErrorMessage);
	}
}