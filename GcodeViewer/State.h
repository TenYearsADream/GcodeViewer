#pragma once
namespace GData {
	//������������ ��������� �� �������� � ����������� ��������� �������� ��� �������� �� ������������������
	//� ������ �������
	public enum class GState {
		None = -1, NotLoad = 0, LineRun = 1, CircClockwise = 2, CircCntrClockwise = 3, NewNullCoor = 10,
		SurfaceXY = 17, SurfaceZX = 18, SurfaceYZ = 19
	};
	//������������ ��������� �� �������� � ����������� ��������� �������� ��� �������� �� ������������������
	//� ������ �������
	public enum class MState {
		None = -1, PreStart = 0, StopNoReset = 2, StartRotateClockwise = 3, StartRotateCntrClockwise = 4,
		StopRotate = 5, ChangeInsrtument = 6, StopWithReset = 30
	};
}
