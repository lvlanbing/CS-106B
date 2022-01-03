// This is the CPP file you will edit and turn in.
// Also remove these comments here and add your own.
// TODO: remove this comment header!

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "grid.h"
#include "gwindow.h"
#include "simpio.h"
#include "strlib.h"
#include "lifegui.h"
using namespace std;

void welcome();
void open_file(string& filename, ifstream& file_stream, int& row, int& col);
Grid<char> create_grid(ifstream& file_stream, int row, int col);
void next_grid(Grid<char>& gird, bool);
bool life_judge(Grid<char> grid, int, int, bool);
void animate(Grid<char>& grid, bool);
bool wrapping();
void menu();

void welcome() {
    cout<<"Welcome to the CS 106B Game life,\n"<<
          "a simulation of the lifecycle of a bacteria colony.\n"<<
          "Cell (X) live and die by the following rules:\n"<<
          "- A cell with 1 or fewer neighbors dies.\n"<<
          "- Locations with 2 neighbors remain stable.\n"<<
          "- Locations with 3 neighbors will create life.\n"<<
          "- A cell with 4 or more neighbors dies.\n"<<endl;
}

void open_file(string &filename, ifstream& file_stream, int &row, int &col) {
    filename = getLine("Grid input  file name?");
    while(!fileExists(filename)) {
        cout<<"Unable to open that file.   Try again\n";
        filename = getLine("Grid input  file name?");
    }

    string line;
    openFile(file_stream,filename);
    getline(file_stream, line);
    row = stringToInteger(line);
    getline(file_stream, line);
    col = stringToInteger(line);
}
Grid<char> create_grid(ifstream& file_stream, int row, int col) {
    int i = 0;
    Grid<char> grid(row, col);
    string line;
    while(getline(file_stream, line)) {
        if(i == row) {
            break;
        }
        for(int j = 0; j < col; j++) {
            grid[i][j] = line[j];
            cout<<line[j];
        }
        i++;
        cout<<endl;
    }
    file_stream.close();
    return grid;
}

bool life_judge(Grid<char> grid, int row, int col, bool wrap) {
    //统计周围有多少个细胞
    int num = 0;
    for(int i = -1; i <= 1; i++) {
        for(int j = -1; j <= 1; j++) {
            if(i == 0 && j == 0) {
                continue;
            }
            if(wrap) { //边界问题，true：四周看成在一起的
                int r = (row + i + grid.numRows()) % grid.numRows();
                int c = (col + j + grid.numCols()) % grid.numCols();
                if(grid[r][c] == 'X')
                    num++;
            }   else if(grid.inBounds(row + i, col + j) && grid[row + i][col + j] == 'X') {
                num++;
            }
        }
    }

    // 判断, X为真， -为假
    if(num ==  0 || num == 1 || num >= 4)
        return false;
    else if(num == 3)
        return true;
    else if(grid[row][col] == 'X')
        return true;
    return false;
}
void next_grid(Grid<char>& grid, bool wrap) {
    Grid<char> next_grid(grid.numRows(), grid.numCols());
    for(int i = 0; i < grid.numRows(); i++) {
        for(int j = 0; j < grid.numCols(); j++) {
            if(life_judge(grid, i, j, wrap)){
                cout<<'X';
                next_grid[i][j] = 'X';
            } else {
                cout<<'-';
                next_grid[i][j] = '-';
            }
        }
        cout<<endl;
    }
    // 更新grid
    grid = next_grid;
}
void animate(Grid<char>& grid, bool wrap) {
    string frames = getLine("How many frames?");
    for(int i = 0; i < frames.length(); i++) {
        if(frames[i] < '0' || frames[i] > '9') {
            cout<<"Illegal integer format. Try again.\n";
            frames = getLine("How many frames?");
            i--;
        }
    }
    //多次迭代
    int num = stringToInteger(frames);
    for(int i = 0; i < num; i++) {
        clearConsole();
        next_grid(grid, wrap);
        pause(50);
    }
}
//请求是否有边界问题
bool wrapping() {
    string option = getLine("Should the simulation wrap around the grid (y/n)?");
    if(option.size() != 1) {
        cout<<"Invalid choice; please try agin.\n";
        return wrapping();
    }
    char ch = option.front();
    switch (ch) {
    case 'n':
    case 'N':
        return false;
    case 'y':
    case 'Y':
        return true;
    default:
        cout<<"Invalid choice; please try agin.\n";
        return wrapping();
    }
}
void menu(Grid<char>& grid, bool wrap) {
    string option = getLine("a)nimate, t)ick, q)uit?");
    if(option.size() != 1) {
        cout<<"Invalid choice; please try agin.\n";
        menu(grid, wrap);
    }
    char ch = option.front();
    switch (ch) {
    case 'a':
    case 'A': {
        animate(grid, wrap);
        menu(grid, wrap);
        break;
    }
    case 't':
    case 'T': {
        next_grid(grid, wrap);
        menu(grid, wrap);
        break;
    }
    case 'q':
    case 'Q': {
        break;
    }
    default: {
        cout<<"Invalid choice; please try agin.\n";
        menu(grid, wrap);
    }
    }
}
int main() {
    welcome();
    string filename;
    ifstream file_stream;
    int row, col;
    bool wrap;
    //读取文件内容
    open_file(filename, file_stream, row, col);
    Grid<char> grid;
    wrap = wrapping();
    grid = create_grid(file_stream, row,  col);
    menu(grid, wrap);
    cout << "Have a nice Life!" << endl;
    return 0;
}
