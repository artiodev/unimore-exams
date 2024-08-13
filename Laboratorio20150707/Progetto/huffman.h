#include <map>
#include <memory>
#include <vector>
#include <queue>

/*#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <functional>
#include <array>
#include <iomanip>//*/

template<typename T>
struct frequency {
	std::map<T, size_t> _count;

	void operator() (const T& sym) {
		++_count[sym];
	}

	auto begin() -> decltype(_count.begin()) { return _count.begin(); }
	auto end() -> decltype(_count.end()) { return _count.end(); }
	auto begin() const -> decltype(_count.begin()) { return _count.begin(); }
	auto end() const -> decltype(_count.end()) { return _count.end(); }
};

template<typename T>
class huffman {
public:
	struct elem {
		T _sym;
		uint8_t _len = 0;
		uint32_t _code = 0;

		elem() {}
		elem(const T& sym) : _sym(sym) {}

		bool operator<(const elem& rhs) const {
			if (_len < rhs._len)
				return true;
			else if (_len > rhs._len)
				return false;
			else
				return _sym < rhs._sym;
		}
	};

private:
	std::vector<elem> _table;

	struct node {
		elem _elem;
		size_t _count;
		node* _left = nullptr;
		node* _right = nullptr;
		node* _parent = nullptr;

		node(const T& sym, size_t count) : _elem(sym), _count(count) {}
		node(node* left, node* right) : _count(left->_count + right->_count), _left(left), _right(right) {
			_left->_parent = this;
			_right->_parent = this;
		}

		bool leaf() const {
			return _left == nullptr;
		}

		bool operator>(const node& rhs) const {
			return _count > rhs._count;
		}
	};

	static void create_table(node* node, std::vector<elem>& tab, uint8_t len = 0) {
		if (!node->leaf()) {
			create_table(node->_left, tab, len + 1);
			create_table(node->_right, tab, len + 1);
		}
		else {
			node->_elem._len = len;
			tab.push_back(node->_elem);
		}
	}

	template<typename T>
	struct ptr_greater : public std::binary_function<T, T, bool> {
		bool operator()(const T& lhs, const T& rhs) const {
			return *lhs > *rhs;
		}
	};

public:
	// Genera l'albero di Huffman e una tabella dei codici canonici a partire dalle frequenze dei dati
	// I dati devono essere ordinabili (deve esistere l'operatore <)
	huffman(const frequency<T>& f) {
		// Creo una coda con priorità per gestire l'ordinamento
		std::priority_queue<node*, std::vector<node*>, ptr_greater<node*>> pq;
		// Inserisco nella coda i puntatori ai nodi, gestiti da un vettore di unique_ptr 
		std::vector<std::unique_ptr<node>> nodes;
		for (auto x : f) {
			nodes.emplace_back(std::make_unique<node>(x.first, x.second));
			pq.push(nodes.back().get());
		}

		// Algoritmo di Huffman
		while (pq.size()>1) {
			auto n1 = pq.top();
			pq.pop();
			auto n2 = pq.top();
			pq.pop();

			nodes.emplace_back(std::make_unique<node>(n1, n2));
			pq.push(nodes.back().get());
		}
		node* root = pq.top();
		pq.pop();

		// Calcolo le lunghezze utilizzando l'albero e le inserisco nella tabella
		create_table(root, _table);

		// Ordino la tabella in base alla lunghezza e al simbolo
		sort(begin(_table), end(_table));

		// Calcolo i codici canonici
		uint8_t len = 0;
		uint32_t code = 0;
		for (auto& x : _table) {
			code <<= x._len - len;
			len = x._len;
			x._code = code;
			++code;
		}
	}

	const std::vector<elem>& table() const {
		return _table;
	}
};