#include "History.h"
#include <iostream>

using namespace std;

History::History(int nRows, int nCols) {
	m_rows = nRows;
	m_cols = nCols;
	for (int r = 1; r <= m_rows; r++)
		for (int c = 1; c <= m_cols; c++)
			historyGrid[r - 1][c - 1] = 0;

}

bool History::record(int r, int c) {
	if (r > m_rows || r < 0 || c > m_cols || c < 0)
		return false;
	historyGrid[r - 1][c - 1] += 1;
	return true;
}

void History::display() const {
	char displayGrid[MAXROWS][MAXCOLS];
	for (int r = 1; r <= m_rows; r++) {
		for (int c = 1; c <= m_cols; c++) {
			int count = historyGrid[r - 1][c - 1];
			if (count == 0)
				displayGrid[r - 1][c - 1] = '.';
			else if (count > 26)
				displayGrid[r - 1][c - 1] = 'Z';
			else
				displayGrid[r - 1][c - 1] = count + 64;
		}
	}
	clearScreen();
	for (int r = 1; r <= m_rows; r++)
	{
		for (int c = 1; c <= m_cols; c++)
			cout << displayGrid[r - 1][c - 1];
		cout << endl;
	}
	cout << endl;
}