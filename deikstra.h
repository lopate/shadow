#pragma once
#include <vector>
struct graph {
	struct graphShadingEdge {
		int fineness; //Крупность дороги, т.е. чем более крупная дорога тем больше эта величина 
		double shading; // Затененность дороги выраженная в длине незатененной части
		double length; // Длина дороги 
		int node; //Вершина конца ребра
		int prevNode; //вершина начала ребра
	}; //Структура с информацией о затенении ребра

	struct graphRoute {
		std::vector<int> Edges;
		double shading;
	}; //Структура с маршрутом
	std::vector<int>& getadjacencyMatrix(int Node); //Реализауия матрица смежности
	graphShadingEdge getShadingEdge(int edgeIndex); //Реализация расчета затенений
	graphRoute getRoute(int startNode, int endNode); //Реализация построения маршрута
	double getLength2(int _Node1, int _Node2); //Квадрат длины ребра 
	graph(); // Инициализация всего графа
};