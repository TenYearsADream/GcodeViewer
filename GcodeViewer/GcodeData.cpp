#include "GcodeData.h"


using namespace GData;
using namespace System;
using namespace System::Collections;
using namespace System::Globalization;
using namespace System::IO;



//�����������, �������������� ��������� commands
GcodeData::GcodeData()
{
	commands = gcnew System::Collections::Generic::List<String^>(1);
	commands->Clear();
	errorRows = gcnew System::Collections::Generic::List<Int32>(1);
	errorRows->Clear();
	minX = 0; maxX = 0;
	minY = 0; maxY = 0;
	minZ = 0; maxZ = 0;
}

GcodeData::~GcodeData()
{

}

//�������� ����� �� filepath
bool GcodeData::loadFile(String^ filepath)
{
	if (filepath == nullptr || !File::Exists(filepath))
		return false;
	Kadr::reset(true);
	resetMinMax();
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
// � ������� ����� ��������� ������ � ��������� polylines
Generic::List<Polyline^>^ GData::GcodeData::tranlate(System::Collections::Generic::List<String^>^ cmds)
{
	//���� ��������� �����, �� ������ � d�����
	if (cmds == nullptr || cmds->Count < 1)
		return nullptr;
	//��������� ������
	GData::Kadr^ kstate = gcnew Kadr();
	polylines = gcnew Generic::List<Polyline^>();
	errorRows->Clear();
	//��������� �� ��������� comds
	int i = 0, j = 0;
	while (i < cmds->Count) {

		String^ str = gcnew String(cmds[i]);
		Polyline^ line = gcnew Polyline(0);
		if (!Kadr::getPolyline(str, line)) {
			errorRows->Add(i);
		}
		else {
			polylines->Insert(j, line);			//������� ����� � ������ ���������
			j++;
		}
		i++;
	}
	checkMinMax();
	return polylines;
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
//��������� ����������� ����������� ���������� ���� ������
bool GcodeData::checkMinMax() {
	if (polylines != nullptr) {
		float tmpX = 0, tmpY = 0, tmpZ = 0;
		for (int i = 0; i < polylines->Count; i++) {
			if (polylines[i]->x != nullptr) {

				for (int j = 0; j < polylines[i]->x->Count; j++) {
					tmpX = polylines[i]->x[j];
					tmpY = polylines[i]->y[j];
					tmpZ = polylines[i]->z[j];
					if (minX > tmpX) minX = tmpX;
					if (maxX < tmpX) maxX = tmpX;
					if (minY > tmpY) minY = tmpY;
					if (maxY < tmpY) maxY = tmpY;
					if (minZ > tmpZ) minZ = tmpZ;
					if (maxZ < tmpZ) maxZ = tmpZ;
				}
			}
			else {
				Console::WriteLine("\n checkminmax polylines[" + i + "]-> is nulptr ");
			}
		}
	}
	else {
		Console::Write("\n checkminmax polylines is nulptr ");
	}
	return true;
}

void GData::GcodeData::resetMinMax()
{
	minX = 0; maxX = 0;
	minY = 0; maxY = 0;
	minZ = 0; maxZ = 0;
}

