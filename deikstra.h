#pragma once
#include <vector>
struct graph {
	struct graphShadingEdge {
		int fineness; //��������� ������, �.�. ��� ����� ������� ������ ��� ������ ��� �������� 
		double shading; // ������������ ������ ���������� � ����� ������������ �����
		double length; // ����� ������ 
		int node; //������� ����� �����
		int prevNode; //������� ������ �����
	}; //��������� � ����������� � ��������� �����

	struct graphRoute {
		std::vector<int> Edges;
		double shading;
	}; //��������� � ���������
	std::vector<int>& getadjacencyMatrix(int Node); //���������� ������� ���������
	graphShadingEdge getShadingEdge(int edgeIndex); //���������� ������� ���������
	graphRoute getRoute(int startNode, int endNode); //���������� ���������� ��������
	double getLength2(int _Node1, int _Node2); //������� ����� ����� 
	graph(); // ������������� ����� �����
};