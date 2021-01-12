
#include "pch.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <sstream>
#include <string>
#include <iomanip>
#include <bitset>

using namespace std;

int Binary2Hex(std::string Binary)//32位转16进制
{
	std::bitset<32> set(Binary);
	int hex = set.to_ulong();
	return hex;
}

float GetFloat32(string Binary)//32位转浮点数
{
	int HexNumber = Binary2Hex(Binary);
	bool negative = !!(HexNumber & 0x80000000);
	int  exponent = (HexNumber & 0x7f800000) >> 23;
	int sign = negative ? -1 : 1;

	exponent -= 127;
	int power = -1;
	float total = 0.0;
	for (int i = 0; i < 23; i++)
	{
		int c = Binary[i + 9] - '0';
		total += (float)c * (float)pow(2.0, power);
		power--;
	}
	total += 1.0;
	float value = sign * (float)pow(2.0, exponent) * total;
	return value;
}

string dec2hex(int i) //将int转成16进制字符串
{
	stringstream ioss; //定义字符串流
	string s_temp; //存放转化后字符
	ioss << setiosflags(ios::uppercase) << hex << i; //以十六制(大写)形式输出
	ioss >> s_temp;
	return s_temp;
}

long byte2int(long bytearry[], int arry_len)//将long 型（4B）数组转换成int，byte[0]是最低位
{
	long result = 0;
	for (int i = arry_len - 1; i >= 0; i--)
		result += (long)bytearry[i] << i * 8;
	return result;
}

long long bytes2int(long long bytearry[], int arry_len)//将long long 型（8B）数组转换成int，byte[0]是最低位
{
	long long result = 0;
	for (int i = arry_len - 1; i >= 0; i--)
		result += (long long)bytearry[i] << i * 8;
	return result;
}

int getNBit(long in, int n) {
	return (in >> (n - 1)) & 1;
}

int getFOURBit(long in) {
	return in & 0xf;
}

string Binarycout(long n)//int转32位二进制
{
	string str;
	for (int i = 31; i >= 0; i--)
	{
		int c = ((n >> i) & 1);
		string s = to_string(c);
		str += s;
	}
	return str;
}

int main()
{
	//Oscilloscope1
	ifstream infile("C:\\Users\\13953\\Desktop\\Oscilloscope1.trs", ios::binary);
	ofstream out_tag_length("tag_length.txt");
	ofstream out_info("info.txt");
	ofstream out_wave("wave.txt");

	if (!infile)
	{
		cerr << "Failed to open the file!";
		return 1;
	}

	long NT_decimal = 0, NS_decimal = 0, SC_decimal = 0, DS_decimal = 0, TS_decimal = 0;//DS和TS是可选的
	unsigned char u_data[1];
	infile.read((char*)u_data, 1);

	while (u_data[0] != 0x5F)//读头信息
	{
		switch (u_data[0])
		{
		case 0x41: {
			infile.read((char*)u_data, 1);
			out_tag_length << "NT=" << (int)u_data[0] << "\n";
			long NT[4];
			for (int i = 0; i < 4; i++)
			{
				infile.read((char*)u_data, 1);
				NT[i] = (int)u_data[0];
			}
			NT_decimal = byte2int(NT, 4);
			out_info << "NT=" << NT_decimal << "\n";
			break;
		}

		case 0x42: {
			infile.read((char*)u_data, 1);
			out_tag_length << "NS=" << (int)u_data[0] << "\n";
			long NS[4];
			for (int i = 0; i < 4; i++)
			{
				infile.read((char*)u_data, 1);
				NS[i] = (int)u_data[0];
			}
			NS_decimal = byte2int(NS, 4);
			out_info << "NS=" << NS_decimal << "\n";
			break;
		}

		case 0x43: {
			infile.read((char*)u_data, 1);
			out_tag_length << "SC=" << (int)u_data[0] << "\n";
			infile.read((char*)u_data, 1);
			unsigned char SC = u_data[0];
			SC_decimal = (long)SC;
			out_info << "SC=" << SC_decimal << "\n";
			break;
		}

		case 0x44: {
			infile.read((char*)u_data, 1);
			out_tag_length << "DS=" << (int)u_data[0] << "\n";
			long DS[2];
			for (int i = 0; i < 2; i++)
			{
				infile.read((char*)u_data, 1);
				DS[i] = (int)u_data[0];
			}
			DS_decimal = byte2int(DS, 2);
			out_info << "DS=" << DS_decimal << "\n";
			break;
		}

		case 0x45: {
			infile.read((char*)u_data, 1);
			out_tag_length << "TS=" << (int)u_data[0] << "\n";
			infile.read((char*)u_data, 1);
			unsigned char TS = u_data[0];
			TS_decimal = (long)TS;
			out_info << "TS=" << TS_decimal << "\n";
			break;
		}

		case 0x46: {
			infile.read((char*)u_data, 1);
			out_tag_length << "GT=" << (int)u_data[0] << "\n";
			const int ff = (int)u_data[0];
			long long* GT = new long long[ff];
			unsigned long long f = 0;
			for (int i = 0; i < ff; i++)
			{
				infile.read((char*)u_data, 1);
				GT[i] = (int)u_data[0];
			}
			f = bytes2int(GT, ff);
			out_info << "GT=" << f << "\n";
			break;
		}

		case 0x47: {
			infile.read((char*)u_data, 1);
			out_tag_length << "DC=" << (int)u_data[0] << "\n";
			const int ff = (int)u_data[0];
			long long* DC = new long long[ff];
			unsigned long long f = 0;
			for (int i = 0; i < ff; i++)
			{
				infile.read((char*)u_data, 1);
				DC[i] = (int)u_data[0];
			}
			f = bytes2int(DC, ff);
			out_info << "DC=" << f << "\n";
			break;
		}

		case 0x48: {
			infile.read((char*)u_data, 1);
			out_tag_length << "XO=" << (int)u_data[0] << "\n";
			long XO[4], a = 0;
			for (int i = 0; i < 4; i++)
			{
				infile.read((char*)u_data, 1);
				XO[i] = (int)u_data[0];
			}
			a = byte2int(XO, 4);
			out_info << "XO=" << a << "\n";
			break;
		}

		case 0x49: {
			infile.read((char*)u_data, 1);
			out_tag_length << "XL=" << (int)u_data[0] << "\n";
			const int ff = (int)u_data[0];
			long long* XL = new long long[ff];
			unsigned long long f = 0;
			for (int i = 0; i < ff; i++)
			{
				infile.read((char*)u_data, 1);
				XL[i] = (int)u_data[0];
			}
			f = bytes2int(XL, ff);
			out_info << "XL=" << f << "\n";
			break;
		}

		case 0x4A: {
			infile.read((char*)u_data, 1);
			out_tag_length << "YL=" << (int)u_data[0] << "\n";
			const int ff = (int)u_data[0];
			long long* YL = new long long[ff];
			unsigned long long f = 0;
			for (int i = 0; i < ff; i++)
			{
				infile.read((char*)u_data, 1);
				YL[i] = (int)u_data[0];
			}
			f = bytes2int(YL, ff);
			out_info << "YL=" << f << "\n";
			break;
		}

		case 0x4B: {
			infile.read((char*)u_data, 1);
			out_tag_length << "XS=" << (int)u_data[0] << "\n";
			long XS[4], a = 0;
			for (int i = 0; i < 4; i++)
			{
				infile.read((char*)u_data, 1);
				XS[i] = (int)u_data[0];
			}
			a = byte2int(XS, 4);
			out_info << "XS=" << a << "\n";
			break;
		}

		case 0x4C: {
			infile.read((char*)u_data, 1);
			out_tag_length << "YS=" << (int)u_data[0] << "\n";
			long YS[4], a = 0;
			for (int i = 0; i < 4; i++)
			{
				infile.read((char*)u_data, 1);
				YS[i] = (int)u_data[0];
			}
			a = byte2int(YS, 4);
			out_info << "YS=" << a << "\n";
			break;
		}

		case 0x4D: {
			infile.read((char*)u_data, 1);
			out_tag_length << "TO=" << (int)u_data[0] << "\n";
			long TO[4], a = 0;
			for (int i = 0; i < 4; i++)
			{
				infile.read((char*)u_data, 1);
				TO[i] = (int)u_data[0];
			}
			a = byte2int(TO, 4);
			out_info << "TO=" << a << "\n";
			break;
		}

		case 0x4E: {
			infile.read((char*)u_data, 1);
			out_tag_length << "LS=" << (int)u_data[0] << "\n";
			infile.read((char*)u_data, 1);
			unsigned char LS = u_data[0];
			int a = (int)LS;
			out_info << "LS=" << a << "\n";
			break;
		}

		default: {
			break;
		}

		}

		infile.read((char*)u_data, 1);

	}

	infile.read((char*)u_data, 1);
	out_info.close();
	out_tag_length.close();

	int bit_5 = getNBit(SC_decimal, 5);
	int per_sample_sapce = getFOURBit(SC_decimal);

	ofstream out_text_in("text_in.txt");
	ofstream out_text_out("text_out.txt");
	for (int i = 0; i < NT_decimal; i++)
	{

		if (TS_decimal != 0)
		{
			ofstream out_title_space("title_space.txt");
			for (int j = 0; j < TS_decimal; j++)//title space per trace
			{
				infile.read((char*)u_data, 1);
				out_title_space << dec2hex(u_data[0]) << ' ';
			}
			out_tag_length.close();

		}

		if (DS_decimal != 0)
		{
			for (int j = 0; j < DS_decimal / 2; j++)//明文为前一半字节数
			{
				infile.read((char*)u_data, 1);
				out_text_in << dec2hex(u_data[0]) << ' ';
			}
			out_text_in << "\n";

			for (int j = 0; j < DS_decimal / 2; j++)//密文为后一半字节数
			{
				infile.read((char*)u_data, 1);
				out_text_out << dec2hex(u_data[0]) << ' ';
			}
			out_text_out << "\n";

		}

		if (per_sample_sapce == 4)
		{
			long data[4];
			if (bit_5 == 1)//bit5==1表示是ieee754浮点数表示，需要转换
			{
				for (int j = 0; j < NS_decimal; j++)
				{
					for (int i = 0; i < 4; i++)
					{
						infile.read((char*)u_data, 1);
						data[i] = (int)u_data[0];
					}
					long a = byte2int(data, 4);
					string s = Binarycout(a);
					float f = GetFloat32(s);
					out_wave << setiosflags(ios::fixed) << setprecision(6) << f << ' ';
				}
			}
			else//如果bit_5==0的话，读出来的就是整型，不是ieee754，所以不需要转换
			{
				for (int j = 0; j < NS_decimal; j++)
				{
					for (int i = 0; i < 4; i++)
					{
						infile.read((char*)u_data, 1);
						data[i] = (int)u_data[0];
					}
					long a = byte2int(data, 4);
					out_wave << a << ' ';
				}
			}

		}

		else if (per_sample_sapce == 1)
		{
			if (bit_5 == 1)
			{
				for (int j = 0; j < NS_decimal; j++)
				{
					infile.read((char*)u_data, 1);
					unsigned char	 ch = u_data[0];
					long a = (long)ch;
					string s = Binarycout(a);
					float f = GetFloat32(s);
					out_wave << setiosflags(ios::fixed) << setprecision(6) << f << ' ';
				}
			}
			else
			{
				for (int j = 0; j < NS_decimal; j++)
				{
					infile.read((char*)u_data, 1);
					unsigned char	 ch = u_data[0];
					long a = (long)ch;
					out_wave << a << ' ';
				}
			}
		}

		else if (per_sample_sapce == 2)
		{
			long data[2];
			if (bit_5 == 1)
			{
				for (int j = 0; j < NS_decimal; j++)
				{
					for (int i = 0; i < 2; i++)
					{
						infile.read((char*)u_data, 1);
						data[i] = (int)u_data[0];
					}
					long a = byte2int(data, 2);
					string s = Binarycout(a);
					float f = GetFloat32(s);
					cout << f << endl;
					out_wave << setiosflags(ios::fixed) << setprecision(6) << f << ' ';
				}
			}
			else
			{
				for (int j = 0; j < NS_decimal; j++)
				{
					for (int i = 0; i < 2; i++)
					{
						infile.read((char*)u_data, 1);
						data[i] = (int)u_data[0];
					}
					long a = byte2int(data, 2);
					out_wave << a << ' ';
				}
			}
		}

		out_wave << "\n";
	}

	infile.close();
	out_text_in.close();
	out_text_out.close();
	out_wave.close();

	return 0;
}

