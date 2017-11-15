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
}

GcodeData::~GcodeData()
{

}

//���������� ������, ���������� ������ ����� �� ������� �������� ������ � �������� ��������,
//���� ������ ����� ���, �� ���������� System::String::Empty, ���� ������ ��� ����� ������ �� ������ nulptr
//� backlastindex ���������� ������ �� ������� ����������� ����� �����
String ^ GData::GcodeData::getFloat(String ^str, int startIndex, int &backlastindex)
{
	int start = startIndex, end = startIndex;
	bool isdot = false;
	while (start < str->Length && Char::IsLetter(str[start]) && !(str[start] == '.'))
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
	while (end < str->Length && !Char::IsWhiteSpace(str[end]) && !(str[end] == 'Y')&& (((str[end] == '.') ^ isdot) || Char::IsNumber(str[end]))) {
		if (str[end] == '.') isdot = true;
		end++;
	}
	//��� �� ���� ����� ������?
	if (end < str->Length && str[end] == '.') {
		Console::Write("parseFloat:nulptr:dot id:" + start + "-" + end + " str-substr: " +str+ "-" + str->Substring(start, end - start));
		return nullptr;
	}
	Console::Write(" id:" + start + "-" + end + " ");
	backlastindex = end;
	return str->Substring(start, end - start);
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
System::Collections::Generic::List<Polyline^>^ GData::GcodeData::tranlate(System::Collections::Generic::List<String^>^ cmds)
{
	//���� ��������� �����, �� ������ � �����
	if (cmds == nullptr || cmds->Count < 1)
		return nullptr;
	//��������� ���������
	GData::GState gstate = GState::None;
	GData::Modal modalstate = Modal::None;
	NumberFormatInfo^ formatInfo = gcnew NumberFormatInfo();
	formatInfo;
	System::Collections::Generic::List<Polyline^>^ lines;
	int i = 0;
	float numParam;
	int curindex, tmpindex;
	//��������� �� ��������� comds
	while (i < cmds->Count) {

		String^ str = gcnew String(cmds[i]);
		str = str->Trim();
		if (String::IsNullOrWhiteSpace(str)) {
			i++;
			continue;
		}
		curindex = 0;
		numParam = -1;
		Console::WriteLine("");
		//while for curindex
		tmpindex = curindex;
//		wchar_t tmpchar = safe_cast<wchar_t>(str[curindex]);
		switch (str[curindex]) {
			//���������������� (��������) �������
		case 'G':
			Console::Write("G");
			break;
		case 'X':
			Console::Write("X");
			break;
		case 'Y':
			Console::Write("Y");
			break;
		case 'Z':
			Console::Write("Z");
			break;
			//�������� �������
		case 'P':
			Console::Write("P");
			break;
			//�������� ������� ������, ��� ��������� ��������� � ������, mm/min
		case 'F':
			Console::Write("F");
			break;
			//������� �������� ��������
		case 'S':
			Console::Write("S");
			break;
			//case R:	//�� ������������ � ���� ������, �������� ������������ ����� ��� ������
			//case D:	//�� ������������ � ���� ������, �������� ��������� ���������� �����������
			//case L:	//�� ������������ � ���� ������, ����� ������� ������������

			//�������� ���� ��� �������� ������������. ��������������� ���������� �� ��������� ����� �� ������ ���� �� ��� X.
		case 'I':
			Console::Write("I");
			break;
			//�������� ���� ��� �������� ������������. ��������������� ���������� �� ��������� ����� �� ������ ���� �� ��� Y
		case 'J':
			Console::Write("J");
			break;
			//�������� ���� ��� �������� ������������. ��������������� ���������� ���� �� ��� Z.
		case 'K':
			Console::Write("K");
			break;
			//��������������� (���������������) �������
		case 'M':
			Console::Write("M");
			break;
			//����� �����������
		case 'T':
			Console::Write("T");
			break;
		default:	//��������� �������������������� (���������) ��������
			Console::WriteLine("Wrong content in this \"" + str + "\" from index " + tmpindex);
			errorRows->Add(i);
			curindex = str->Length;
			i++;
			continue;
		}
		//������� ������ ���������� �����
		String^ resultGetFloat = getFloat(str, tmpindex, curindex);
		Console::Write(resultGetFloat);
		//���� ������ ������, �� �������� ��������, �� ����, ���� ������� � ����������� ������� ��� �����
		if (System::String::Empty->Equals(resultGetFloat)) {
			Console::WriteLine("not get float in this \"" + str + "\" from index " + tmpindex);
		}
		else {
			//���� ������ ������ ������� nulptr, 
			//������� ����� ���� ������ � ���� ��������� �����, ���������� ������ � ���� �������
			if (resultGetFloat == nullptr) {
				Console::WriteLine(" Cannot get float (nulptr) in this \"" + str + "\" from index " + tmpindex);
				errorRows->Add(i);
				curindex = str->Length;
				i++;
				continue;
			}
			else
			{
				//�������� ��������� ���������� ����� ��������, ������
				if (!System::Single::TryParse(resultGetFloat, NumberStyles::AllowDecimalPoint,formatInfo , numParam))
							Console::WriteLine("Falture to TryParse for float");
			}
		}
		Console::WriteLine(" tmpindex: " + tmpindex + " curindex: " + curindex + " param: " + numParam);
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


