#include "GcodeData.h"

using namespace GData;
using namespace System;
using namespace System::Collections;
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
	errorRows = gcnew System::Collections::Generic::List<Int32>(1);
}

GcodeData::~GcodeData()
{

}

//���������� ������, ���������� ������ ����� �� ������� �������� ������ � �������� ��������,
//���� ������ ����� ���, �� ���������� nulptr
String ^ GData::GcodeData::getStartFloat(String ^str, int startIndex)
{
	int start = startIndex, end = startIndex;
	while (System::Char::IsLetter(str[start]) && start + 1 < str->Length)
	{
		start++;
		end++;
	}
	if(end < str->Length) ++end;
	while (end < str->Length && !System::Char::IsWhiteSpace(str[end]) && System::Char::IsNumber(str[end])) {
		end++;
	}
	Console::Write(" start: " + start + " end: " + end + " float " + str->Substring(start, end - start));
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

System::Collections::Generic::List<Polyline^>^ GData::GcodeData::tranlate(System::Collections::Generic::List<String^>^ cmds)
{
	//���� ��������� �����, �� ������ � �����
	if (cmds == nullptr || cmds->Count < 1)
		return nullptr;
	//��������� ���������
	GData::GState gstate = GState::None;
	GData::Modal modalstate = Modal::None;

	System::Collections::Generic::List<Polyline^>^ lines;
	int i = 0;
	float numParam;
	//��������� �� ��������� comds
	while (i < cmds->Count) {
		String^ str = gcnew String(cmds[i]);
		str = str->Trim();
		if (String::IsNullOrWhiteSpace(str)) {
			i++;
			continue;
		}
		switch (str[0]) {
			//���������������� (��������) �������
		case 'G':
			Console::Write("G");
			break;
		case 'X':
		case 'Y':
		case 'Z':
			//�������� �������
		case 'P':
			//�������� ������� ������, ��� ��������� ��������� � ������, mm/min
		case 'F':
			//������� �������� ��������
		case 'S':
			//case R:	//�� ������������ � ���� ������, �������� ������������ ����� ��� ������
			//case D:	//�� ������������ � ���� ������, �������� ��������� ���������� �����������
			//case L:	//�� ������������ � ���� ������, ����� ������� ������������

			//�������� ���� ��� �������� ������������. ��������������� ���������� �� ��������� ����� �� ������ ���� �� ��� X.
		case 'I':
			//�������� ���� ��� �������� ������������. ��������������� ���������� �� ��������� ����� �� ������ ���� �� ��� Y
		case 'J':
			//�������� ���� ��� �������� ������������. ��������������� ���������� ���� �� ��� Z.
		case 'K':
			//��������������� (���������������) �������
		case 'M':
			;
		case 'T': //����� �����������
			break;
		default:	//��������� �������������������� (���������) ��������
			;
		}
		Console::Write(System::Single::TryParse(getStartFloat(str, 0), numParam));
		Console::WriteLine(numParam);
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


