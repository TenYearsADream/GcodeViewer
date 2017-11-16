#include "GcodeData.h"

using namespace GData;
using namespace System;
using namespace System::Collections;
using namespace System::Globalization;
using namespace System::IO;

//����������� Polyline, �������������� ������� ��������� �� ��������������� �����
GData::Polyline::Polyline(int _n)
{
	Polyline::x = gcnew array<float>(_n);
	Polyline::y = gcnew array<float>(_n);
	Polyline::z = gcnew array<float>(_n);
}

//�������� �������, ����� ���������� �� ���������� �����, 
//���������� ���������� ��������� ���������� ���������
Polyline ^ GData::Polyline::operator/(Polyline ^ p, float d)
{
	for (int i = 0; i < p->n; i++)
	{
		p->x[i] = p->x[i] / d;
		p->y[i] = p->y[i] / d;
		p->z[i] = p->z[i] / d;
	}
	return p;
}

//�����������, �������������� ��������� commands
GcodeData::GcodeData()
{
	commands = gcnew System::Collections::Generic::List<String^>(1);
	commands->Clear();
	errorRows = gcnew System::Collections::Generic::List<Int32>(1);
	errorRows->Clear();
	formatInfo = gcnew NumberFormatInfo();
}

GcodeData::~GcodeData()
{

}

//���������� ������, ���������� ������ ����� �� ������� �������� ������ � �������� ��������,
//���� ������ ����� ���, �� ���������� System::String::Empty, ���� ������ ��� ����� ������ �� ������ nulptr
//� backlastindex ���������� ������ �� ������� ����������� ����� �����
String ^ GData::GcodeData::getNumeric(String ^str, int startIndex, int &backlastindex)
{
	int start = startIndex, end = startIndex;
	bool isdot = false;
	while (start < str->Length &&((str[end] == 'Y')|| Char::IsLetter(str[start]) )&& !(str[start] == '.'))
	{
		start++;
		end++;
	}
	//���� �� �� ��������� �� ����� ������
	if (end < str->Length) {
		end++;
		//���� ���������� �� �����, ������� ���, ��� �� ������ ������
		if (str[start] == '.') isdot = true;
	}
	// (��������� end �� ��������� ������) � (������ �� end �� ������) � 
	// � ( ������ �������� ������ ��� ( (  ������ �������� ������ ) �����������_��� ����� � ����� ��� ���������� � �����) )
	//��������� �������� ��� ����������� ���� ����� � �����
	while (end < str->Length && !Char::IsWhiteSpace(str[end]) && !(str[end] == 'Y') &&!(str[end] == 'Z') 
		&& !(str[end] == 'F') && !(str[end] == 'S') && (((str[end] == '.') ^ isdot) || Char::IsDigit(str[end]))) {
		if (str[end] == '.') isdot = true;
		end++;
	}
	//��� �� ���� ����� ������?
	if (end < str->Length && str[end] == '.') {
		Console::Write("parseFloat:nulptr:dot id:" + start + "-" + end + " str-substr: " + str + "-" + str->Substring(start, end - start));
		return nullptr;
	}
	backlastindex = end;
	return str->Substring(start, end - start);
}

//��������� true, ���� ������� ���������� ���������� ������ ����� result �� ������ str,
//������� � ������� startIndex,
//�������� ������ �� ������� ����������� �����
bool GData::GcodeData::getInt(String ^str, int &result, int startIndex, int &backlastindex) {
	//������� ������ ���������� �����
	String^ resultStr = getNumeric(str, startIndex, backlastindex);
	//���� ������ ������, �� �������� ��������, �� ����, ���� ������� � ����������� ������� ��� �����
	if (System::String::Empty->Equals(resultStr)) {
		Console::WriteLine("not get int \"" + str + "\"is empty. index " + startIndex);
		return false;
	}
	//���� ������ ������ ������� nulptr
	if (resultStr == nullptr) {
		Console::WriteLine(" Cannot get int (nulptr)\"" + str + "\" index " + startIndex + "too many dots");
		return false;
	}
	//����� �������� ����� startindex+1, ������, ���� ��������� ������ �������� � �� ����������� �����,
	//�� ���� ��������� ����� ����� ����� �������� �� startIndex-1, ��� ��������� ������
	if (backlastindex - startIndex - 1 > resultStr->Length) {
		Console::WriteLine(" int is not that whitch  expected\"" + str + "\"need int by index " + startIndex);
		return false;
	}
	//�������� ��������� ���������� ����� ��������, ������
	if (!System::Int32::TryParse(resultStr, NumberStyles::Integer, formatInfo, result)) {
		Console::WriteLine("Falture to TryParse for int");
		return false;
	}
	return true;
}

//��������� true, ���� ������� ���������� ���������� ������ ����� � ��������� ������ result
//�� ������ str, ������� � ������� startIndex,
//�������� ������ �� ������� ����������� �����
bool GData::GcodeData::getFloat(String ^str, float &result, int startIndex, int &backlastindex) {
	//������� ������ ���������� �����
	String^ resultStr = getNumeric(str, startIndex, backlastindex);
	//���� ������ ������, �� �������� ��������, �� ����, ���� ������� � ����������� ������� ��� �����
	if (System::String::Empty->Equals(resultStr)) {
		Console::WriteLine("not get float \"" + str + "\"is empty. index " + startIndex);
		return false;
	}
	//���� ������ ������ ������� nulptr
	if (resultStr == nullptr) {
		Console::WriteLine(" Cannot get int (nulptr)\"" + str + "\" index " + startIndex + "too many dots");
		return false;
	}

	//�������� ��������� ���������� ����� ��������, ������
	if (!System::Single::TryParse(resultStr, NumberStyles::AllowDecimalPoint|NumberStyles::AllowLeadingSign, formatInfo, result)) {
		Console::WriteLine("Falture to TryParse for float\"" + str + "\"-"+resultStr);
		return false;
	}
	return true;
}
//�������� ����� �� filepath
bool GcodeData::loadFile(String^ filepath)
{
	if (filepath == nullptr&&File::Exists(filepath))
		return false;
	try {
		commands->Clear();
		StreamReader ^sr = gcnew StreamReader(filepath);
		while (!sr->EndOfStream) {
			commands->Add(sr->ReadLine());
		}
		sr->Close();
		delete sr;
		tranlate(commands);
	}
	catch (System::OutOfMemoryException^ outME) {
		Console::WriteLine(outME->Message);
		return false;
	}
	catch (System::IO::IOException^ ioE) {
		Console::WriteLine(ioE->Message);
		return false;
	}
	return true;
}

//������� ������������� �������� ������� ��������� � ����������, 
// � ������� ����� ��������� ������ � ��������� polyline
Generic::List<Polyline^>^ GData::GcodeData::tranlate(System::Collections::Generic::List<String^>^ cmds)
{
	//���� ��������� �����, �� ������ � d�����
	if (cmds == nullptr || cmds->Count < 1)
		return nullptr;
	//��������� ��������� (��������)
	GData::GState gstate = GState::None;
	GData::Modal mstate = Modal::None;
	//��������� ������
	GData::kadr^ kstate = gcnew kadr();
	System::Collections::Generic::List<Polyline^>^ lines;
	int i = 0;
	bool errR;
	float paramFloat;
	int paramInt;
	int curindex, tmpindex;
	//��������� �� ��������� comds
	while (i < cmds->Count) {

		String^ str = gcnew String(cmds[i]);
		str = str->Trim();
		//���� ������ ������, �� ��������� ������
		if (String::IsNullOrWhiteSpace(str)) {
			i++;
			continue;
		}
		//���������� ����� �������� �� ������
		curindex = 0;
		errR = false;
		kstate->reset();

		while (curindex < str->Length) {

			paramInt = -1;
			paramFloat = safe_cast<float>(-0.0001);

			tmpindex = curindex;

			switch (str[curindex]) {
				//���������������� (��������) �������
			case 'G':
				if (!getInt(str, paramInt, tmpindex, curindex)) {
					errorRows->Add(i);
					errR = true;
					curindex = str->Length;
					continue;
				}
				gstate = safe_cast<GState>(paramInt);
				Console::Write("G");
				break;
			case 'X':
				if (!getFloat(str, paramFloat, tmpindex, curindex)) {
					errorRows->Add(i);
					errR = true;
					curindex = str->Length;
					continue;
				}
				kstate->x = paramFloat;
				kstate->bx = true;
				Console::Write("X" + paramFloat);
				break;
			case 'Y':
				if (!getFloat(str, paramFloat, tmpindex, curindex)) {
					errorRows->Add(i);
					errR = true;
					curindex = str->Length;
					continue;
				}
				kstate->y = paramFloat;
				kstate->by = true;
				Console::Write("Y");
				break;
			case 'Z':
				if (!getFloat(str, paramFloat, tmpindex, curindex)) {
					errorRows->Add(i);
					errR = true;
					curindex = str->Length;
					continue;
				}
				kstate->z = paramFloat;
				kstate->bz = true;
				Console::Write("Z");
				break;
				//�������� �������
			case 'P':
				if (!getInt(str, paramInt, tmpindex, curindex)) {
					errorRows->Add(i);
					errR = true;
					curindex = str->Length;
					continue;
				}
				Console::Write("P");
				break;
				//�������� ������� ������, ��� ��������� ��������� � ������, mm/min
			case 'F':
				if (!getFloat(str, paramFloat, tmpindex, curindex)) {
					errorRows->Add(i);
					errR = true;
					curindex = str->Length;
					continue;
				}
				kstate->f = paramFloat;
				kstate->bf = true;
				Console::Write("F");
				break;
				//������� �������� ��������
			case 'S':
				if (!getInt(str, paramInt, tmpindex, curindex)) {
					errorRows->Add(i);
					errR = true;
					curindex = str->Length;
					continue;
				}
				kstate->s = paramInt;
				kstate->bs = true;
				Console::Write("S");
				break;
				//case R:	//�� ������������ � ���� ������, �������� ������������ ����� ��� ������
				//case D:	//�� ������������ � ���� ������, �������� ��������� ���������� �����������
				//case L:	//�� ������������ � ���� ������, ����� ������� ������������
				//�������� ���� ��� �������� ������������. ��������������� ���������� �� ��������� ����� �� ������ ���� �� ��� X.
			case 'I':
				if (!getFloat(str, paramFloat, tmpindex, curindex)) {
					errorRows->Add(i);
					errR = true;
					curindex = str->Length;
					continue;
				}
				kstate->i = paramFloat;
				kstate->bi = true;
				Console::Write("I");
				break;
				//�������� ���� ��� �������� ������������. ��������������� ���������� �� ��������� ����� �� ������ ���� �� ��� Y
			case 'J':
				if (!getFloat(str, paramFloat, tmpindex, curindex)) {
					errorRows->Add(i);
					errR = true;
					curindex = str->Length;
					continue;
				}
				kstate->j = paramFloat;
				kstate->bj = true;
				Console::Write("J");
				break;
				//�������� ���� ��� �������� ������������. ��������������� ���������� ���� �� ��� Z.
			case 'K':
				if (!getFloat(str, paramFloat, tmpindex, curindex)) {
					errorRows->Add(i);
					errR = true;
					curindex = str->Length;
					continue;
				}
				kstate->k = paramFloat;
				kstate->bk = true;
				Console::Write("K");
				break;
				//��������������� (���������������) �������
			case 'M':
				if (!getInt(str, paramInt, tmpindex, curindex)) {
					errorRows->Add(i);
					errR = true;
					curindex = str->Length;
					continue;
				}
				mstate = safe_cast<Modal>(paramInt);
				Console::Write("M");
				break;
				//����� �����������
			case 'T':
				if (!getInt(str, paramInt, tmpindex, curindex)) {
					errorRows->Add(i);
					errR = true;
					curindex = str->Length;
					continue;
				}
				Console::Write("T");
				break;
			default:	//��������� �������������������� (���������) ��������
				Console::WriteLine("Wrong content in this \"" + str + "\" from index " + tmpindex);
				errorRows->Add(i);
				errR = true;
				curindex = str->Length;
				continue;
			}
		}
		if (kstate->bf&&kstate->bs) {

			if (kstate->bx) {
			}
			if (kstate->by) {
			}
			if (kstate->bz) {

			}
			if (kstate->bz) {

			}
			if (kstate->bz) {

			}
			if (kstate->bz) {
			}
		}
		i++;
	}

	return lines;
}


String^ GcodeData::str::get(int index) {
	if (index < commands->Count) {
		return commands[index];
	}
	else return nullptr;
}
void GcodeData::str::set(int index, String^ value) {
	commands[index] = value;
}


