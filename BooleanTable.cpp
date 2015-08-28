#include "BooleanTable.h"

BooleanTable::BooleanTable()
{	
}

BooleanTable::~BooleanTable()
{	
}

void BooleanTable::Add(std::vector<bool> in, std::vector<bool> out)
{
	BooleanEntry b;
	
	b.in  = in;
	b.out = out;
	
	entry.push_back(b);
}

void BooleanTable::Add(BooleanEntry b)
{
	entry.push_back(b);
}

BooleanEntry BooleanTable::Row(int i)
{
	return entry.at(i);
}

bool* BooleanTable::In(int i)
{
	bool* tab = new bool[entry.at(i).in.size()];
	
	for (int j=0; j<(int)entry.at(i).in.size(); j++)
		tab[j] = entry.at(i).in.at(j);
		
	return tab;
}

int BooleanTable::InSize(int i)
{
	return entry.at(i).in.size();
}

bool* BooleanTable::Out(int i)
{
	bool* tab = new bool[entry.at(i).out.size()];
	
	for (int j=0; j<(int)entry.at(i).out.size(); j++)
		tab[j] = entry.at(i).out.at(j);
		
	return tab;
}

int BooleanTable::OutSize(int i)
{
	return entry.at(i).out.size();
}

int BooleanTable::Size()
{
	return entry.size();
}

