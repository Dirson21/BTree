#include <iostream>
#include <vector>
#include <string>
#include <fstream>
using namespace std;
struct bTree
{
	vector<int> key;
	vector<string> name;
	bTree *father = NULL;
	vector<bTree*>vetki;
};

int find_index(vector<int> keys,long int key)
{
	for (int i = 0; i < keys.size(); i++)
	{
		if (key < keys[i])
			return i;
	}
	return keys.size();
}
void add_el(bTree *&tree, bTree *&main_tree, int key, string name, int i, int n,int m);//добавляет элемент
void split_tree(bTree  *tree, bTree *&main_tree , int n) //разделяет дерево
{
	int key = tree->key[n];
	string name = tree->name[n];
	bTree *tmp_left = new bTree;
	//запоминаем последние элементы в новой вершине и удаляем их
	tmp_left->key.insert(tmp_left->key.begin(), tree->key.begin()+n+1, tree->key.end());
	tmp_left->name.insert(tmp_left->name.begin(), tree->name.begin() + n + 1, tree->name.end());
	tmp_left->vetki.insert(tmp_left->vetki.begin(), tree->vetki.begin() + n + 1, tree->vetki.end());
	tree->key.erase(tree->key.begin() + n, tree->key.end());
	tree->name.erase(tree->name.begin() + n, tree->name.end());
	tree->vetki.erase(tree->vetki.begin() + n + 1, tree->vetki.end());
	if (tmp_left->vetki[0])
	{
		for (int i = 0; i < tmp_left->vetki.size(); i++)
		{
			tmp_left->vetki[i]->father = tmp_left;
		}
	}
	if (!tree->father)
	{
		bTree *tmpf = new bTree;
		tmpf->key.push_back(key);
		tmpf->name.push_back(name);
		tmpf->vetki.push_back(tree);
		tmpf->vetki.push_back(tmp_left);
		main_tree->father = tmpf;
		tmp_left->father = tmpf;
		main_tree = tmpf;
	}
	else
	{
		int index= find_index(tree->father->key, key);
		tmp_left->father = tree->father;
		tree->father->vetki.insert(tree->father->vetki.begin() + index+1, tmp_left);
		add_el(tree->father, main_tree, key, name, index, n, 0);
	}
}
void add_el(bTree *&tree, bTree *&main_tree, int key, string name, int i, int n, int m=1)
{
		tree->key.insert(tree->key.begin() + i, key);
		tree->name.insert(tree->name.begin() + i, name);
		if (m)
			tree->vetki.push_back(NULL);
		if (tree->key.size() > 2*n)
		{
			split_tree(tree, main_tree, n);
		}
}
bTree *find_el(bTree *tree, bTree *&main_tree, int key, int &index, int m=1)
{
	if (!tree)
	{
		bTree *tmp = new bTree;
		tmp->vetki.push_back(NULL);
		tmp->father = NULL;
		tree = tmp;
		index = 0;
		main_tree = tree;
		return tree;
	}
	bTree *tmp2 = NULL;
	if (key > tree->key[tree->key.size() - 1])
	{
		if (tree->vetki[tree->key.size()])
			tmp2 = find_el(tree->vetki[tree->key.size()],main_tree, key,index, m);
		else
		{
			if (m)
			{
				index = tree->key.size();
				return tree;
			}
			else return NULL;
		}
		return tmp2;
	}
	for (int i = 0; i < tree->key.size(); i++)
	{
		if (key == tree->key[i])//Если елемент есть в дереве
		{
			if (m)
			{
				cout << "Информация о ключе: " << tree->name[i] << endl;
				system("pause");
				return NULL;
			}
			index = i;
			return tree;
		}
		if (key < tree->key[i])
		{
			if (tree->vetki[i])
			{
				tmp2 = find_el(tree->vetki[i], main_tree, key, index, m);
			}
			else 
			{
				if (m)
				{
					index = i;
					return tree;
				}
				else return NULL;
			}
			return tmp2;
		}
	}
} 
bTree* find_neigh_el(bTree *tree) //нахождение соседнего по значению элемента
{
	if (tree->vetki[0])
		return find_neigh_el(tree->vetki[0]);
	return tree;
}
void corr_tr(bTree *tree, bTree *&main_tree, int pos, int n)//корректировка дерева при удалении элемента
{
	
	int i;
	bTree *tree_left, *tree_right;
	if (pos)
	{
		i = pos-1;
		tree_left = tree->father->vetki[pos - 1];
		tree_right = tree;
	}
	else
	{
		i = pos;
		tree_right = tree->father->vetki[pos + 1];
		tree_left = tree;
	}
	tree_left->key.push_back(tree->father->key[i]);
	tree_left->name.push_back(tree->father->name[i]);
	tree->father->key.erase(tree->father->key.begin() + i);
	tree->father->name.erase(tree->father->name.begin() + i);
	tree_left->key.insert(tree_left->key.end(), tree_right->key.begin(), tree_right->key.end());
	tree_left->name.insert(tree_left->name.end(), tree_right->name.begin(), tree_right->name.end());
	tree_left->vetki.insert(tree_left->vetki.end(), tree_right->vetki.begin(), tree_right->vetki.end());
	tree->father->vetki.erase(tree->father->vetki.begin() + i + 1 );
	delete tree_right;
	if (tree_left->vetki[0])
		for (int i = 0; i < tree_left->vetki.size(); i++)
			tree_left->vetki[i]->father = tree_left;
	if (tree_left->key.size() >= 2 * n + 1)
	{
		tree_right = NULL;
		tree = NULL;
		split_tree(tree_left, main_tree, n);
		tree = tree_left;
	}
	else if (tree_left->father->key.size() < n && tree_left->father->father)
	{
		if (tree_left->father->father)
			pos = find_index(tree_left->father->father->key, tree_left->key[i]);//находим индекс ветки для отца
		else pos = 0;
		corr_tr(tree_left->father, main_tree, pos, n);

	}
	if (tree_left->father->key.empty() && !tree_left->father->father)
	{
		delete main_tree;
		tree_left->father = NULL;
		main_tree = tree_left;
	}
}
void del_elem(bTree *tree, bTree *&main_tree, int index, int n)
{
	if (!tree->vetki[index])
	{
		int pos;
		if (tree != main_tree)
		{
			pos = find_index(tree->father->key, tree->key[index]);
		}
		tree->key.erase(tree->key.begin() + index);
		tree->name.erase(tree->name.begin() + index);
		tree->vetki.erase(tree->vetki.begin() + index);
		if (tree->key.size() < n && tree != main_tree)
		{
			corr_tr(tree, main_tree,pos, n);
		}
		else if (tree == main_tree && !tree->key.size())
		{
			delete main_tree;
			main_tree = NULL;
		}
	}
	else
	{
		bTree *tree_neigh = find_neigh_el(tree->vetki[index + 1]);
		tree->key[index] = tree_neigh->key[0];
		del_elem(tree_neigh, main_tree, 0, n);
	}
}
void print_tree(bTree *tree, int urov=0)
{
	if (tree)
	{
		for (int i = 0; i < urov; i++)
			printf_s(".");
		for (int i = 0; i < tree->key.size(); i++)
			printf_s("%d ", tree->key[i]);
		printf_s("\n");
		for (int i = 0; i < tree->vetki.size(); i++)
			print_tree(tree->vetki[i], urov + 1);
	}
}
int main(int argc, char *argv[])
{
	setlocale(LC_ALL, "rus");
	int N;
	vector<string> file_in;
	cout << "Введите порядок дерева" << endl;
	cin >> N;
	system("cls");
	string name;
	bTree *tree = NULL, *el_tree = NULL;
	int key, index, i;
	fstream f(argv[1]);
	if (!f)
	{
		cout << "Не удалось открыть файл" << endl;
		return 0;
	}
	while (!f.eof())
	{
		if (!(f >> key >> name))
			continue;
		el_tree = find_el(tree, tree, key, index);
		if (el_tree)
			add_el(el_tree, tree, key, name, index, N);
	}
	f.close();
	print_tree(tree);
	while (true)
	{
		cout << "Выберите действие" << endl;
		cout << "1. Найти/Вставить элемент" << endl;
		cout << "2. Удалить элемент" << endl;
		cout << "0. Выход" << endl;
		cin >> i;
		switch (i)
		{
		case 1:
			while (true)
			{
				cout << "Введите ключ. " << "Для выхода введите 0" << endl;
				cin >> key;
				if (key == 0) break;
				el_tree = find_el(tree, tree, key, index);
				if (el_tree)
				{
					f.open(argv[1], ios_base::app);
					cout << "Введите информацию" << endl;
					cin >> name;
					f << endl << key << " " << name;
					f.close();
					add_el(el_tree, tree, key, name, index, N);
				}
				system("cls");
				if (tree)
					print_tree(tree);
			}
			break;
		case 2:
			while (true)
			{
				cout << "Введите ключ для удаления" << "Для выхода введите 0" << endl;
				cin >> key;
				if (key == 0) break;
				if (tree)
				{
					el_tree = find_el(tree, tree, key, index, 0);
					if (el_tree)
					{
						del_elem(el_tree, tree, index, N);
					}
				}
				else
				{
					cout << "Ошибка!!! Пустое дерево\n";
					system("pause");
				}
				system("cls");
				print_tree(tree);
			}
			system("cls");
			break;
		case 0: exit(0);
		}
		
	}
}
	

