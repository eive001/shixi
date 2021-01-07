#pragma once
#include <vector>
#include <iostream>
#include<string>
#include <fstream>
#include<map>
#include<bitset>
#include<sstream>
#include<math.h>

using namespace std;

vector<string> readFile; //读入的汇编指令
vector<string> assIns;   //汇编指令集
map<string, int> insArgNum;//指令集对应的操作数个数
map<string, int> cirAddress;//记录循环时的循环地址
vector<vector<string>> assSplitResult;
vector<unsigned int> assBinResult;

void  ReadAssemble() //用于读入汇编指令
{


	fstream    f("111.txt");
	string     line;
	while (getline(f, line))
	{
		readFile.push_back(line);
	}
	cout << "共有单词数目:" << readFile.size() << endl;
}
void  Display()
{
	int length = readFile.size();

	cout << "读入的文件是:" << endl;
	for (int i = 0; i < length; i++)
	{
		cout << readFile.at(i) << endl;
	}
}
void  StringToUpper() //转换为 大写
{
	int length = readFile.size();
	int lenLine;


	for (int i = 0; i < length; i++)
	{
		string &str = readFile.at(i);
		lenLine = str.length();
		for (int j = 0; j < lenLine; j++)
		{
			str[j] = toupper(str[j]);
		}

	}
}
void  SplitIns() //分割指令
{
	vector<string> strTemp;
	int  length = readFile.size();
	string str, result;
	for (int i = 0; i < length; i++)
	{
		str = readFile.at(i);

		stringstream input(str);
		//依次输出到result中，并存入res中 
		while (input >> result)
			strTemp.push_back(result);
		assSplitResult.push_back(strTemp);
		strTemp.clear();
	}
}
void  InitAssembleInstruction()
{
	assIns.clear();
	assIns.push_back("HLT");//
	assIns.push_back("JMP");//
	assIns.push_back("CJMP");//
	assIns.push_back("OJMP");//
	assIns.push_back("CALL");
	assIns.push_back("RET");//
	assIns.push_back("PUSH");//
	assIns.push_back("POP");//
	assIns.push_back("LOADB");//
	assIns.push_back("LOADW");//
	assIns.push_back("STOREB");//
	assIns.push_back("STOREW");//
	assIns.push_back("LOADI");//
	assIns.push_back("NOP");//
	assIns.push_back("IN");//
	assIns.push_back("OUT");//
	assIns.push_back("ADD");//
	assIns.push_back("ADDI");//
	assIns.push_back("SUB");//
	assIns.push_back("SUBI");//
	assIns.push_back("MUL");//
	assIns.push_back("DIV");//
	assIns.push_back("AND");//
	assIns.push_back("OR");//
	assIns.push_back("NOR");//
	assIns.push_back("NOTB");//
	assIns.push_back("SAL");//
	assIns.push_back("SAR");//
	assIns.push_back("EQU");
	assIns.push_back("LT");//
	assIns.push_back("LTE");//
	assIns.push_back("NOTC");
}
void  InitinsArgNum()
{
	insArgNum.clear();
	insArgNum["LOADI"] = 1;
	insArgNum["LOADB"] = 1;
	insArgNum["LOADW"] = 1;
	insArgNum["STOREB"] = 1;
	insArgNum["STOREW"] = 1;
	insArgNum["ADD"] = 0;
	insArgNum["ADDI"] = 1;
	insArgNum["SUB"] = 0;
	insArgNum["SUBI"] = 1;
	insArgNum["MUL"] = 0;
	insArgNum["DIV"] = 0;
	insArgNum["PUSH"] = 0;
	insArgNum["POP"] = 0;
	insArgNum["LTE"] = 0;
	insArgNum["LT"] = 0;
	insArgNum["CJMP"] = 0;
	insArgNum["JMP"] = 0;
	insArgNum["OJMP"] = 0;
	insArgNum["CALL"] = 0;
	insArgNum["RET"] = 0;
	insArgNum["HLT"] = 0;
	insArgNum["OUT"] = 1;
	insArgNum["IN"] = 1;
	insArgNum["NOP"] = 1;
	insArgNum["AND"] = 0;
	insArgNum["OR"] = 0;
	insArgNum["NOR"] = 0;
	insArgNum["NORB"] = 0;
	insArgNum["SAL"] = 0;
	insArgNum["NORB"] = 0;
	insArgNum["EQU"] = 0;
	insArgNum["NOTC"] = 0;
}

int searchLocate(string str)
{
	int count = 0;
	int length = assIns.size();
	for (int i = 0; i < length; i++)
	{
		if (str == assIns.at(i))
		{
			return i;
		}
	}
	return -1;

}
void  AssembleToBinary()
{
	//寄存器处理，预备部份

	map<string, int> registerValue;  // 记录寄存器对应的值
	registerValue["Z"] = 0;
	registerValue["A"] = 1;
	registerValue["B"] = 2;
	registerValue["C"] = 3;
	registerValue["D"] = 4;
	registerValue["E"] = 5;
	registerValue["F"] = 6;
	registerValue["G"] = 7;


	unsigned int result;
	int length = assSplitResult.size();
	vector<string> strTemp;
	string stringOp;
	for (int i = 0; i < length; i++)
	{
		result = 0;
		strTemp = assSplitResult.at(i);
		stringOp = strTemp.at(0);
		int lentemp = stringOp.size();
		if (':' == stringOp[lentemp - 1])
		{
			stringOp.erase(stringOp.end() - 1);
			cirAddress[stringOp] = 4 * i;
			vector<string>::iterator k = strTemp.begin();
			strTemp.erase(k);
			stringOp = strTemp.at(0);
		}
		//TODO
		int temp = searchLocate(stringOp);
		result = searchLocate(stringOp) << 27;

		if (stringOp == "CJMP")
		{
			int a = cirAddress[strTemp.at(1)];
			result += a;
		}

		int  registNumber, operandNumber;
		operandNumber = insArgNum[stringOp];
		registNumber = strTemp.size() - 1 - operandNumber;

		if (1 == operandNumber)
		{
			result += atoi(strTemp.at(strTemp.size() - 1).c_str());
		}

		if (1 <= registNumber)
		{
			stringOp = strTemp.at(1);
			result += registerValue[stringOp] << 24;
		}
		if (2 <= registNumber)
		{
			stringOp = strTemp.at(2);
			result += registerValue[stringOp] << 20;
		}
		if (3 <= registNumber)
		{
			stringOp = strTemp.at(3);
			result += registerValue[stringOp] << 16;
		}

		assBinResult.push_back(result);
	}
}



//void OutputBinary()
//{
//
//	int length = assBinResult.size();
//
//	unsigned int a = 0;
//
//	for (int i = 0; i < length; i++)
//	{
//		a = assBinResult.at(i);
//		cout << "0x" << hex << a << endl;
//
//	}
//
//}

void Output(vector<string> &binString) 
{
	binString.clear();
	string str = "0x";
	unsigned int intTemp = 0;
	
	int length = assBinResult.size();

	for (int i = 0; i < length; i++)
	{
		
		str = "0x";
		intTemp = assBinResult.at(i);
		for (int j = 0; j < 32; j++)
		{
			
			int a = 0;
			a = intTemp / ((int)pow(2,31-j));
			intTemp = intTemp % ((int)pow(2, 31 - j));
			

			if ( 0 == a)
			{
				str += "0";
			}
			else if (1 == a)
			{
				str += "1";
			}
		}
		binString.push_back(str);

	}

}




void test()
{

	vector<string> result;
	InitAssembleInstruction();
	InitinsArgNum();
	ReadAssemble();
	Display();
	StringToUpper();

	SplitIns();
	AssembleToBinary();
	Output(result);

	for (int i = 0; i < result.size(); i++)
	{
		cout << result.at(i) << endl;
	}


}