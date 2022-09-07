#include "pch.h"
#include "DatabaseStructs.h"
#include <vector>
#include <iostream>

namespace DatabaseStructs
{
	User::User(const std::string& name, long ID)
		: m_Name{ name }, m_ID{ ID }
	{

	}

	int User::getPropertyCount() const
	{
		return 2;
	}

	std::string User::getPropertyValue(int propertyID) const
	{
		static std::string propertyValueStrs[]{
			'\n' + m_Name + '\n', std::to_string(m_ID)
		};

		return propertyValueStrs[propertyID % getPropertyCount()];
	}

	std::string User::getPropertyName(int propertyID) const
	{
		static std::string propertyNameStrs[]{
			"Name", "ID"
		};

		return propertyNameStrs[propertyID % getPropertyCount()];
	}

	std::string User::getTableFormat() const
	{
		static std::string tableFormatStr{
			"Name TEXT, ID INTEGER"
		};

		return tableFormatStr;
	}

	std::string User::getTableColumns() const
	{
		static std::string tableColumnStr{
			"Name, ID"
		};

		return tableColumnStr;
	}

	std::string User::getTableValues() const
	{
		std::string tableValueStr{ '\'' + m_Name + "\', " + std::to_string(m_ID) };
		return tableValueStr;
	}

	int User::sqlReadCallback(void* userListPtr, int count, char** data, char** columns)
	{
		try
		{
			if (count != getPropertyCount())
			{
				std::string errorMessage{ "Property Count" + std::to_string(getPropertyCount()) + "does not match with count " + std::to_string(count) + '.'};
				throw new std::exception(errorMessage.c_str());
			}
			std::vector<User>* userVectorPtr = reinterpret_cast<std::vector<User>*>(userListPtr);
			User tempUser = User{ "Temp", 0 };
			tempUser.m_Name = data[0];
			tempUser.m_ID = std::stol(data[1]);
			userVectorPtr->push_back(tempUser);
			return 0;
		}
		catch (const std::exception& e)
		{
			std::cout << e.what() << '\n';
			return -1;
		}
	}

	std::string User::toString() const
	{
		std::string outputStr{ "Name: " + m_Name + ", ID: " + std::to_string(m_ID)};
		return outputStr;
	}

}


