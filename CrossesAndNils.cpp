#include <iostream>
#include <fstream>
using namespace std;
#include <vector>
#include <sstream>
#include <filesystem>

const int N = 3;
typedef vector<vector<int>> BOARD;

ofstream sw;

void print(BOARD& board)
{
	cout << "+";
	for (size_t j = 0; j < board[0].size(); ++j)
	{
		cout << "-+";
	}
	cout << endl;
	for (size_t i = 0; i < board.size(); ++i)
	{
		cout << "|";
		for (size_t j = 0; j < board[i].size(); ++j)
		{
			switch (board[i][j])
			{
			case +1: cout << "x"; break;
			case -1: cout << "o"; break;
			default: cout << " ";
			}
			cout << "|";
		}
		cout << endl;
		cout << "+";
		for (size_t j = 0; j < board[i].size(); ++j)
		{
			cout << "-+";
		}
		cout << endl;
	}
};

int evaluate(BOARD& board)
{
	// columns
	for (int i = 0; i < N; ++i)
	{
		int sum = 0;
		for (auto v : board)
			sum += v[i];
		if (sum == +N) return +1;
		if (sum == -N) return -1;
	}
	// rows
	for (int i = 0; i < N; ++i)
	{
		int sum = 0;
		for (auto v : board[i])
			sum += v;
		if (sum == +N) return +1;
		if (sum == -N) return -1;
	}
	// diags
	int sum = 0;
	for (int i = 0; i < N; ++i)
	{
		sum += board[i][i];
	}
	if (sum == +N) return +1;
	if (sum == -N) return -1;
	//
	sum = 0;
	for (int i = 0; i < N; ++i)
	{
		sum += board[i][N - i - 1];
	}
	if (sum == +N) return +1;
	if (sum == -N) return -1;
	return 0;
};

string get_string(BOARD& board)
{
	stringstream ss;
	for (size_t i = 0; i < board.size(); ++i)
	{
		ss << "[";
		for (size_t j = 0; j < board[i].size(); ++j)
		{
			switch (board[i][j])
			{
			case +1: ss << "x"; break;
			case -1: ss << "o"; break;
			default: ss << "_";
			}
		}
		ss << "]";
	}
	return ss.str();
};


int who_move(BOARD& board)
{
	int sum0 = 0;
	int sum1 = 0;
	int sum2 = 0;
	for (auto u : board)
	{
		for (auto v : u)
		{
			if (v == 0) sum0++;
			if (v == 1) sum1++;
			if (v == -1) sum2++;
		}
	}
	if (sum0 == 0) return 0;
	return sum1 <= sum2 ? 1 : -1;
};


int COUNT = 0;
int CROSS_WIN = 0;
int NILLS_WIN = 0;

bool move(BOARD board, filesystem::path dir)
{
	string s = get_string(board);
	int who_win = evaluate(board);
	for (auto& ch : s)
	{
		if (who_win == +1 && ch == 'x') ch = 'X';
		if (who_win == -1 && ch == 'o') ch = 'O';
	}
	dir /= s;
	//if (!filesystem::exists(dir))
	//{
	//	filesystem::create_directory(dir);
	//}
	sw << dir.generic_string().c_str() << endl;
	COUNT++;
	switch (who_win)
	{
	case +1: CROSS_WIN++; return false;
	case -1: NILLS_WIN++; return false;
	default:;
	}
	int who = who_move(board);
	bool moved = false;
	for (int i = 0; i < N; ++i)
		for (int j = 0; j < N; ++j)
			if (board[i][j] == 0)
			{
				board[i][j] = who;
				moved = true;
				move(board, dir);
				board[i][j] = 0; // restore the board
			}
	return moved;
};



int main()
{
	BOARD board;
	board.resize(N);
	for (int i = 0; i < N; ++i)
	{
		board[i].resize(N,0);
	}

	board[1][1] = 1; 
	cout << get_string(board) << endl;
	print(board);
	cout << evaluate(board) << endl;;
	cout << who_move(board) << endl;

	board[0][0] = 1;
	board[1][1] = 1;
	board[2][2] = 1;
	print(board);
	cout << get_string(board) << endl;
	cout << evaluate(board) << endl;
	cout << who_move(board) << endl;

	board[0][2] = -1;
	board[1][1] = +1;
	board[2][0] = -1;
	print(board);
	cout << get_string(board) << endl;
	cout << evaluate(board) << endl;
	cout << who_move(board) << endl;

	board[0][2] = -1;
	board[1][1] = -1;
	board[2][0] = -1;
	print(board);
	cout << get_string(board) << endl;
	cout << evaluate(board) << endl;
	cout << who_move(board) << endl;

	board[0][1] = 1;
	print(board);
	cout << evaluate(board) << endl;
	cout << who_move(board) << endl;

	// zero out the board
	for (auto& u : board)
	{
		for (auto& v : u)
		{
			v = 0;
		}
	}
	print(board);
	cout << evaluate(board) << endl;
	cout << who_move(board) << endl;

	cout << endl;
	cout << endl;
	cout << endl;
	cout << endl;

	auto dir = std::filesystem::current_path();
	sw.open((dir.generic_string() + "/CrossesAndNils_mt.txt").c_str());
	bool res = move(board, dir);
	sw.close();
	cout << COUNT << endl;       // 549946
	cout << CROSS_WIN << endl;   // 131184
	cout << NILLS_WIN << endl;   // 77904

	//int count = 0;
	//while (int res = who_move(board) != 0)
	//{
	//	count++;
	//	int i0 = 0;
	//	int j0 = 0;
	//	bool found = false;
	//	for (int i = 0; i < N; ++i)
	//		for (int j = 0; j < N; ++j)
	//			if (!found)
	//			{
	//				if (board[i][j] == 0)
	//				{
	//					i0 = i;
	//					j0 = j;
	//					found = true;
	//				}
	//			}
	//	if (found)
	//	{
	//		board[i0][j0] = who_move(board);
	//	}

	//	cout << endl;
	//	cout << count << endl;
	//	print(board);
	//	cout << evaluate(board) << endl;
	//}

}
