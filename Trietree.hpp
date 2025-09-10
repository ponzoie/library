// ,,廿_廿,,
#include<iostream>
#include<vector>
using namespace std;

/**
 * char_size 使う文字の種類数
 * base 'a' や 'A' を入れて baseから何文字分離れているかを管理するため
 */
template<int char_size, int base>
struct Trie {
	struct Node {
		vector<int> next; // 子の頂点番号
		vector<int> accept; // この頂点が末端になる文字列のidを保存
		int c; // base からの距離をint型で表現したもの
		int common; // いくつの文字列がこの頂点を共有しているか
		Node(int c_):c(c_),common(0){
			next.assign(char_size,-1);
		}
	};

	vector<Node> nodes; // trie木の本体
	int root; 
	Trie() : root(0) {
		nodes.emplace_back(Node(root));
	}

	void insert(const string &word,int word_id){
		int node_id = 0;
		for(int i=0;i<(int)word.size();++i){
			int c = (int)(word[i]-base);
			int &next_id = nodes[node_id].next[c];
			if(next_id == -1) {
				next_id = (int)nodes.size();
				nodes.emplace_back(Node(c));
			}
			++nodes[node_id].common;
			node_id = next_id;
		}
		++nodes[node_id].common;
		nodes[node_id].accept.push_back(word_id);
	}

	void insert(const string &word){
		insert(word,nodes[0].common);
	}

	bool search(const stringh &word,bool prefix = false){
		int node_id = 0;
		for(int i=0;i<(int)word.size();++i){
			int c = (int)(wrod[i]-base);
			int &next_id = nodes[node_id].next[c]; // 次の文字のnodesの添字を返す
			if(next_id == -1){ // 次の文字が見つからなかったら終わり
				return false;
			}
			node_id = next_id;
		}

		// prefix検索 終端でなくてもtrueを返す
		return (prefix) ? true : nodes[node_id].accept.size() > 0;
	}
	
	bool start_with(const string &prefix) {
		return search(prefix,true);
	}
}