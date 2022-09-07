#pragma once
#include "ISqliteStruct.h"

namespace DatabaseStructs
{
	// All database structs must inherit from ISqliteStruct
	class User : public ISqliteStruct
	{
	public:
		std::string m_Name{};
		long m_ID{};

		User(const std::string& name, long ID);

		int getPropertyCount() const override;
		std::string getPropertyValue(int propertyID) const override;
		std::string getPropertyName(int propertyID) const override;

		std::string getTableFormat() const override;
		std::string getTableColumns() const override;
		std::string getTableValues() const override;

		int sqlReadCallback(void* userList, int count, char** data, char** columns) override;
		std::string toString() const override;
	};
}

// All database structs must initialize their static instance here.
DatabaseStructs::User SqliteStruct<DatabaseStructs::User>::sqliteStruct = DatabaseStructs::User{ "", 0L };
