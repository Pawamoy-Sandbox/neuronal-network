#ifndef __BOOLEAN_TABLE_H
#define __BOOLEAN_TABLE_H

#include <vector>

typedef struct boolean_entry BooleanEntry;

class BooleanTable
{
	public:
		BooleanTable();	
		~BooleanTable();
		
		void Add(std::vector<bool> in, std::vector<bool> out);
		void Add(BooleanEntry b);
		BooleanEntry Row(int i);
		bool* In(int i);
		bool* Out(int i);
		int Size();
		int InSize(int i);
		int OutSize(int i);
		
	private:
		std::vector<BooleanEntry> entry;
};

struct boolean_entry
{
	std::vector<bool> in;
	std::vector<bool> out;
};

#endif // __BOOLEAN_TABLE_H
