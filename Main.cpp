#include <iostream>
#include <vector>
#include <math.h>
#include <algorithm>
#include <map>

template<typename T>
struct minSet {
	struct item {
		int key;
		double prior;
		T value;
		item* l, * r;
		item() :key(0), prior(0), l(NULL), r(NULL) { };
		item(int key, double prior, const T& value) : key(key), prior(prior), value(value), l(NULL), r(NULL) { };
	};
	
	typedef item* pitem;
	pitem root;
	minSet() :root(NULL) {};
	void recurDel(pitem t) {
		if (t->l) recurDel(t->l);
		if (t->r) recurDel(t->r);
		delete(t);
	}
	~minSet() {
		if(root) recurDel(root);
	}
	void _split(pitem t, int key, pitem& l, pitem& r) {
		if (!t)
			l = r = NULL;
		else if (key < t->key)
			_split(t->l, key, l, t->l), r = t;
		else
			_split(t->r, key, t->r, r), l = t;
	}

	void _insert(pitem& t, pitem it) {
		if (!t)
			t = it;
		else if (it->prior < t->prior)
			_split(t, it->key, it->l, it->r), t = it;
		else
			_insert(it->key < t->key ? t->l : t->r, it);
	}

	void _merge(pitem& t, pitem l, pitem r) {
		pitem new_t = NULL;
		if (!l || !r)
			new_t = l ? l : r;
		else if (l->prior < r->prior)
			_merge(l->r, l->r, r), new_t = l;
		else
			_merge(r->l, l, r->l), new_t = r;
		t = new_t;
	}

	void _erase(pitem& t, int key) {
		if (!t) {
			return;
		}
		if (t->key == key) {
			pitem to_del = t;
			_merge(t, t->l, t->r);
			delete(to_del);
		}
		else
			_erase(key < t->key ? t->l : t->r, key);
	}

	pitem _unite(pitem l, pitem r) {
		if (!l || !r)  return l ? l : r;
		if (l->prior > r->prior)  std::swap(l, r);
		pitem lt, rt;
		split(r, l->key, lt, rt);
		l->l = unite(l->l, lt);
		l->r = unite(l->r, rt);
		return l;
	}

	pitem _search(int key) {
		pitem p = root;
		while (p && p->key != key) {
			p = (key < p->key ? p->l : p->r);
		}
		return p;
		
	}
	item getmin() {
		if (root) {
			return *root;
		}
		return item();
	}
	item get(int key) {
		pitem p = _search(key);
		if (p) {
			return *p;
		}else{
			return item();
		}
	}
	void erase(int key) {
		_erase(root, key);
	}
	bool is_empty() {
		return !root;
	}
	void update(int key, double prior, const T& value) {
		pitem it = _search(key);
		if (it) {
			erase(key);
		}
		it = new item(key, prior, value);
		_insert(root, it);
	}
};

struct graph {
	struct graphShadingEdge {
		int fineness; //Крупность дороги, т.е. чем более крупная дорога тем больше эта величина 
		double shading; // Затененность дороги выраженная в длине незатененной части
		double length; // Длина дороги 
		int node; //Вершина конца ребра
		int prevNode; //вершина начала ребра
	};

	struct graphNode {
		double x;
		double y;

	};
	struct graphRoute {
		std::vector<int> Edges;
		double shading;
	};
	
	std::vector<graphShadingEdge> Edges;
	std::vector<graphNode> Nodes;
	std::vector<std::vector<int>> adjacencyMatrix;
	graphShadingEdge getShadingEdge(int edgeIndex);
	graphRoute getRoute(int startNode, int endNode);
	std::vector<int>& getadjacencyMatrix(int Node);
	double getLength2(int _Node1, int _Node2);
	graph();
};
graph::graph() {
	Edges = {
		{1, 2, 10, 1, 0},//0
		{1, 2, 10, 0, 1},//1

		{1, 0, 10, 0, 3},//2
		{1, 0, 10, 3, 0},//3

		{2, 1, 4, 1, 2},//4
		{2, 1, 4, 2, 1},//5

		{3, 1, 5, 1, 3},//6
		{3, 1, 5, 3, 1},//7

		{1, 10, 10, 2, 3},//8
		{1, 10, 10, 3, 2},//9

		{1, 10, 100, 2, 4},//10
		{1, 10, 100, 4, 2},//11

		{1, 2, 6, 3, 4},//12
		{1, 2, 6, 4, 3},//13
	};
	Nodes = {
		{0,0},
		{5, 0},
		{5, 5},
		{0, 5},
		{2.5, 10}
	};
	adjacencyMatrix = {
		{0, 3}, //0
		{1, 5, 7}, //1
		{4, 9, 11}, //2
		{2, 6, 8, 13}, //3
		{10, 12} //4
	};
}
graph::graphShadingEdge graph::getShadingEdge(int edgeIndex) {
	return Edges[edgeIndex];
}
std::vector<int>& graph::getadjacencyMatrix(int Node) {
	return adjacencyMatrix[Node];
}
/*
struct minimumsSet {
	struct minimumsSetElement {
		double key;
		double value;
		int edgeIndex;
		int nodeIndex;
		char inf;
	};
	std::vector<minimumsSetElement> _array;
	std::vector<char> used;
	bool is_empty() {
		bool ans = true;
		for (auto& e : used) {
			if (e == 0) {
				ans = false;
			}
		}
		return ans;
	}
	minimumsSetElement getMinimum() {
		minimumsSetElement ans = {
			0, 0, 0, 0, 1
		};
		for (int i = 0; i < _array.size(); i++) {
			if (used[i] == 0 &&(ans.inf || ans.key > _array[i].key)) {
				ans = _array[i];
			}
		}
		return ans;
	}

	minimumsSetElement get(int nodeIndex) {
		bool updated = false;
		minimumsSetElement ans = {
			0, 0, 0, 0, 1
		};
		for (int i = 0; i < _array.size(); i++) {
			if (used[i] == 0 &&nodeIndex == _array[i].nodeIndex) {
				ans = _array[i];
			}
		}
		return ans;
	}
	void update(int nodeIndex, double weight,  int edgeIndex, double shading) {
		bool updated = false;
		minimumsSetElement el = {
			 weight, shading, edgeIndex, nodeIndex
		};
		for (int i = 0; i < _array.size(); i++) {
			if (nodeIndex == _array[i].nodeIndex) {
				_array[i] = el;
				updated = true;
				used[i] = 0;
			}
		}
		if (!updated) {
			_array.push_back(el);
			used.push_back(0);
		}
	}

	void erase(int nodeIndex) {
		for (int i = 0; i < _array.size(); i++) {
			if (nodeIndex == _array[i].nodeIndex) {
				used[i] = 1;
			}
		}
	}
};
*/

struct minimumsSet {
	struct minimumsSetElement {
		double key;//Приоритет
		double value;
		int edgeIndex;
		int nodeIndex;//Ключ
		char inf;
	};
	struct minSetType {
		double value;
		int edgeIndex;
		char inf;
		minSetType() : value(0), edgeIndex(0), inf(1) {}
		minSetType(double value, int edgeIndex = 0, char inf = 1) : value(value), edgeIndex(edgeIndex), inf(inf) {};
	};
	minSet<minSetType> treap;
	bool is_empty() {
		return treap.is_empty();
	}
	minimumsSetElement getMinimum() {
		auto ret = treap.getmin();
		minimumsSetElement ans = {
			ret.prior,
			ret.value.value,
			ret.value.edgeIndex,
			ret.key,
			ret.value.inf
		};
		return ans;
	}

	minimumsSetElement get(int nodeIndex) {
		auto ret = treap.get(nodeIndex);
		minimumsSetElement ans = {
			ret.prior,
			ret.value.value,
			ret.value.edgeIndex,
			ret.key,
			ret.value.inf
		};
		return ans;
	}

	void update(int nodeIndex, double weight, int edgeIndex, double shading) {
		bool updated = false;
		minSetType el = {
			 shading, edgeIndex, 0
		};
		treap.update(nodeIndex, weight, el);
	}

	void erase(int nodeIndex) {
		treap.erase(nodeIndex);
	}
};

struct usedSet {
	std::map<int, bool> _map;
	void update(int nodeIndex, char used) {
		_map[nodeIndex] = used;
	}
	bool get(int nodeIndex) {
		return _map[nodeIndex];
	}
};

struct valueSet {
	struct valueSetElement {
		int nodeIndex;
		double value;
		int edgeIndex;
		int prevNodeIndex;
	};
	std::map<int, valueSetElement> _map;
	void update(int nodeIndex, double value, int edgeIndex, int prevNodeIndex) {
		valueSetElement el = {
			 nodeIndex, value, edgeIndex, prevNodeIndex
		};
		_map[nodeIndex] = el;
	}
	valueSetElement get(int nodeIndex) {
		return _map[nodeIndex];
	}
};

double graph::getLength2(int _Node1, int _Node2) {
	graphNode Node1 = Nodes[_Node1], Node2 = Nodes[_Node2];
	return sqrt((Node1.x - Node2.x)* (Node1.x - Node2.x) + (Node1.y - Node2.y) * (Node1.y - Node2.y));
}
double getRemotennesWeight(graph& _graph, int startNode, int endNode, int EdgeNode, double fineness) {
	// -O(exp(x^2))
	return std::min(std::exp(_graph.getLength2(startNode, EdgeNode)/fineness), std::exp(_graph.getLength2(endNode, EdgeNode)) / fineness);
}

double getEdgeWeight(graph& _graph, double shading, double length,int startNode, int endNode, int EdgeNode, double fineness) {
	return shading + 0.1 * length + getRemotennesWeight(_graph, startNode, endNode, EdgeNode, fineness);
}

std::vector<double> trans_finesness = {
	10,
	50,
	100,
	500
};


 
graph::graphRoute graph::getRoute(int startNode, int endNode) {
	minimumsSet minSet;
	usedSet usedSet;
	valueSet valueSet;
	graph::graphRoute ans;
	bool getans = false;
	minSet.update(startNode, 0, 0, 0);
	valueSet.update(startNode, 0, 0, 0);
	usedSet.update(0, 0);
	while (getans == 0 || minSet.is_empty() == 0) {
		minimumsSet::minimumsSetElement el = minSet.getMinimum();
		if (el.nodeIndex == endNode) {
			getans = true;
			int node = endNode;
			ans.shading = el.value;
			while (node != startNode) {
				valueSet::valueSetElement route_el = valueSet.get(node);
				ans.Edges.push_back(route_el.edgeIndex);
		
				node = route_el.prevNodeIndex;
			}
			break;
		}
		for (auto& e : getadjacencyMatrix(el.nodeIndex)) {
			graph::graphShadingEdge curEdge = getShadingEdge(e);
			double new_length = el.key + getEdgeWeight(*this, curEdge.shading, curEdge.length, startNode, endNode, curEdge.node, trans_finesness[curEdge.fineness]);
			double new_shading = el.value + curEdge.shading;
			if (usedSet.get(curEdge.node) == 0 && (minSet.get(curEdge.node).inf || minSet.get(curEdge.node).key > new_length)) {
				minSet.update(curEdge.node, new_length, e, new_shading);
				valueSet.update(curEdge.node, new_shading, e, el.nodeIndex);
			}
		}
		minSet.erase(el.nodeIndex);
		usedSet.update(el.nodeIndex, 1);
	}
	return ans;
}
int main() {
	graph plGraph;
	int node1, node2;
	std::cin >> node1 >> node2;
	graph::graphRoute route = plGraph.getRoute(node1, node2);
	std::cout << route.shading << std::endl;
	for (auto& e : route.Edges) {
		std::cout << e << " ";
	}
	std::cout << std::endl;
}