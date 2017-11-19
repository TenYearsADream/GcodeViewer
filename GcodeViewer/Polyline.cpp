#include "Polyline.h"

using namespace GData;
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