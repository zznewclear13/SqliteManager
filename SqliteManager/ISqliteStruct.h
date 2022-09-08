#pragma once
#include <string>

// All database structs must inherit from ISqliteStruct.
class ISqliteStruct
{
public:
	// Get total propery count from database struct.
	virtual int getPropertyCount() const = 0;

	// Get property name at propertyID.
	virtual std::string getPropertyName(int propertyID) const = 0;
	// Get property value at propertyID.
	virtual std::string getPropertyValue(int propertyID) const = 0;

	// Get database struct format to create table.
	virtual std::string getTableFormat() const = 0;
	// Get database struct names as table columns.
	virtual std::string getTableColumns() const = 0;
	// Get database struct values as table values.
	virtual std::string getTableValues() const = 0;

	// Call back function fills std::vector<ISqliteStruct>,
	// the list is passed as void* userListPtr.
	virtual int sqlReadCallback(void* userListPtr, int count, char** data, char** columns) = 0;	
	virtual std::string toString() const = 0;

	virtual ~ISqliteStruct() {}
};

// use "concept" if c++20 or later 
#if _MSVC_LANG >= 202002L
#define SQLITE_STRUCT_TEMPLATE template<derived T>
template<typename T>
concept derived = std::is_base_of<ISqliteStruct, T>::value;
#else
#define SQLITE_STRUCT_TEMPLATE template<typename T>
#endif

// Call static functions from an interface class.
// https://stackoverflow.com/a/66927384
SQLITE_STRUCT_TEMPLATE
class SqliteStruct
{	
public:
	static T sqliteStruct;

	static std::string getPropertyName(int propertyID);
	static std::string getTableFormat();
	static std::string getTableColumns();

	static int sqlReadCallback(void* userListPtr, int count, char** data, char** columns);
};

SQLITE_STRUCT_TEMPLATE
inline std::string SqliteStruct<T>::getPropertyName(int propertyID)
{
	return sqliteStruct.getPropertyName(propertyID);
}

SQLITE_STRUCT_TEMPLATE
inline std::string SqliteStruct<T>::getTableFormat()
{
	return sqliteStruct.getTableFormat();
}

SQLITE_STRUCT_TEMPLATE
inline std::string SqliteStruct<T>::getTableColumns()
{
	return sqliteStruct.getTableColumns();
}

SQLITE_STRUCT_TEMPLATE
inline int SqliteStruct<T>::sqlReadCallback(void* userListPtr, int count, char** data, char** columns)
{
	return sqliteStruct.sqlReadCallback(userListPtr, count, data, columns);
}

