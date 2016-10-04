#include <vector>
#include <list>
#include <unordered_map>
#include <cassert>
#include <iostream>
#include <sstream>
#include <cstring>

using namespace std;


enum class ShapeType
{
	L,
	iL,
	Square,
	Col,
	Z,
	iZ,
	T
};


struct Point
{
	Point(int r, int c) : row(r), col(c) {}
	int row;
	int col;
};

static const vector<Point> L_Desc_0 = {{1,1}, {2,1}, {3,1}, {3,2}};
static const vector<Point> L_Desc_1 = {{1,1}, {2,1}, {1,2}, {1,3}};
static const vector<Point> L_Desc_2 = {{1,1}, {1,2}, {2,2}, {3,2}};
static const vector<Point> L_Desc_3 = {{2,1}, {2,2}, {2,3}, {1,3}};

static const vector<vector<Point>> L_desc {L_Desc_0, L_Desc_1, L_Desc_2, L_Desc_3};

static const vector<Point> iL_Desc0 = {{1,2}, {2,2}, {3,2}, {3,1}};
static const vector<Point> iL_Desc1 = {{1,1}, {2,1}, {2,2}, {2,3}};
static const vector<Point> iL_Desc2 = {{1,1}, {1,2}, {2,1}, {3,1}};
static const vector<Point> iL_Desc3 = {{1,1}, {1,2}, {1,3}, {2,3}};

static const vector<vector<Point>> iL_desc {iL_Desc0, iL_Desc1, iL_Desc2, iL_Desc3};

static const vector<Point> Square = {{1,1}, {1,2}, {2,1}, {2,2}};
static const vector<vector<Point>> Square_desc {Square};

static const vector<Point> Col0 = {{1,1}, {2,1}, {3,1}, {4,1}};
static const vector<Point> Col1 = {{1,1}, {2,1}, {3,1}, {4,1}};
static const vector<vector<Point>> Col_desc {Col0, Col1};


static const vector<Point> Z0 = {{1,1}, {1,2}, {2,2}, {2,3}};
static const vector<Point> Z1 = {{1,2}, {2,1}, {2,2}, {3,1}};
static const vector<vector<Point>> Z_desc {Z0, Z1};

static const vector<Point> iZ0 = {{2,1}, {2,2}, {1,2}, {1,3}};
static const vector<Point> iZ1 = {{1,1}, {2,1}, {2,2}, {3,2}};
static const vector<vector<Point>> iZ_desc {iZ0, iZ1};


static const vector<Point> T0 = {{1,1}, {1,2}, {1,3}, {2,2}};
static const vector<Point> T1 = {{2,1}, {1,2}, {2,2}, {3,2}};
static const vector<Point> T2 = {{1,2}, {2,1}, {2,2}, {2,3}};
static const vector<Point> T3 = {{1,1}, {2,1}, {3,1}, {2,2}};
static const vector<vector<Point>> T_desc {T0, T1, T2, T3};



class Shape
{
public:
	Shape(ShapeType type, int id) : _id(id), _type(type)
	{

	}
	int id() const {return _id;}
	ShapeType type() const {return _type;}

private:
	int		  _id;
	ShapeType _type;
};




class Board
{
public:
	Board(int numRows, int numCols) : _b(nullptr), _nRows(numRows), _nCols(numCols)
	{
		_b = new char[numRows * numCols];
		memset(_b, 0, numRows * numCols);
	}

	bool isFull() const
	{
		for(int r=0;r<_nRows;r++)
		{
			for(int c=0;c<_nCols;c++)
			{
				if(get(r,c)==0)
					return false;
			}
		}
		return true;
	}

	int RowCount() const {return _nRows;}
	int ColCount() const {return _nCols;}

	bool place(const Shape& shape)
	{
		const vector<vector<Point>>* typePointer = nullptr;
		switch(shape.type())
		{
		case ShapeType::L:
			typePointer = &L_desc;
			break;
		case ShapeType::iL:
			typePointer = &iL_desc;
			break;
		case ShapeType::Square:
			typePointer = &Square_desc;
			break;
		case ShapeType::Col:
			typePointer = &Col_desc;
			break;
		case ShapeType::Z:
			typePointer = &Z_desc;
			break;
		case ShapeType::iZ:
			typePointer = &iZ_desc;
			break;
		case ShapeType::T:
			typePointer = &T_desc;
			break;
		}

		for(int r=0;r<_nRows;r++)
		{
			for(int c=0;c<_nCols;c++)
			{
				bool didPlace = tryPlace(*typePointer, r, c, shape.id());
				if(didPlace)
					return true;
			}
		}
		return false;
	}

	void erase(int id)
	{
		assert(_mapping.find(id)!=_mapping.end());
		vector<Point>& points = _mapping[id];
		for(auto& p : points)
		{
			put(0, p.row, p.col);
		}
		_mapping.erase(id);
	}

	string toString() const
	{
		stringstream ss;
		for(int r=0;r<RowCount();r++)
		{
			for(int c=0;c<ColCount();c++)
			{
				ss << '0' + get(r,c) - '0';
				if(c!=ColCount()-1)
					ss << ",";
			}
			ss << "\n";
		}

		return ss.str();
	}

	char get(int row, int col) const
	{
		return _b[_nCols * row + col];
	}

	void put(char val, int row, int col)
	{
		_b[_nCols * row + col] = val;
	}


private:

	bool tryPlace(vector<vector<Point>> desc, int r, int c, int id)
	{
		for(auto& rotation : desc)
		{
			int numOfPointsInRot = rotation.size();
			int gC = 0;
			for(auto& point : rotation)
			{
				Point p(r + point.row-1, c + point.col-1);
				if(! (p.row<_nRows && p.col<_nCols && get(p.row, p.col) == 0) )
					break;
				else
					gC++;
			}
			if(gC == numOfPointsInRot)
			{
				// place it and return true
				for(auto& point : rotation)
				{
					Point p(r + point.row-1, c + point.col-1);
					put(id, p.row, p.col);
				}
				assert(_mapping.find(id)==_mapping.end());
				//cout << toString() << endl;
				_mapping[id] = saveMapping(rotation,r,c);
				return true;
			}
		}
		return false;
	}

	vector<Point> saveMapping(const vector<Point>& rot, int r, int c)
	{
		vector<Point> cp;
		for(const Point& p : rot)
		{
			cp.push_back(Point(r + p.row-1, c + p.col-1));
		}

		return cp;
	}



	char* _b;
	int _nRows;
	int _nCols;
	unordered_map<int, vector<Point>> _mapping;

};


ostream& operator<<(ostream& os, const Board& board)
{
	os << board.toString();
	return os;
}


class Puzzle
{
public:
	Puzzle(int numRows, int numCols, const vector<Shape>& shapes) : _startShapes(shapes), _board(numRows, numCols) {}
	void solve()
	{
		_solve(_startShapes);
	}
	void print()
	{
		cout << _board.toString() << endl;
	}

private:
	bool _solve(vector<Shape> shapes)
	{
		for(int i=0;i<shapes.size();i++)
		{
			bool placed = place(shapes[i]);
			if(placed)
			{
				//cout << _board.toString() << endl;

				if(_board.isFull())
					return true;
				vector<Shape> newShapeSet = _excludeFromSet(i, shapes);
				bool solved = _solve(newShapeSet);
				if(solved)
					return true;
				else
				{
					_board.erase(shapes[i].id());
				}
			}

		}

		return false;
	}

	vector<Shape> _excludeFromSet(int i, const vector<Shape>& shapes)
	{
		vector<Shape> sh;
		for(int j=0;j<shapes.size();j++)
		{
			if(j!=i)
				sh.push_back(shapes[j]);
		}

		return std::move(sh);

	}

	bool place(const Shape& shape)
	{
		return _board.place(shape);
	}

private:
	vector<Shape> _startShapes;
	Board _board;
};


class ArgParser
{
public:
	int Row() const {return _row;}
	int Col() const {return _col;}

	const vector<Shape>& Shapes() {return _shapes;}

	void parse(int argc, char** argv)
	{
		_row = atoi(argv[1]);
		_col = atoi(argv[2]);
		for(int i=3;i<argc;i++)
		{
			string shape = string(argv[i]);
			ShapeType type = shapeType(shape);
			_shapes.push_back(Shape(type, i-2));
		}
	}
private:
	ShapeType shapeType(const string& shape)
	{
		if(shape == "T")
			return ShapeType::T;
		else if(shape == "Col")
			return ShapeType::Col;
		else if(shape == "L")
			return ShapeType::L;
		else if(shape == "iL")
			return ShapeType::iL;
		else if(shape == "Square")
			return ShapeType::Square;
		else if(shape == "Z")
			return ShapeType::Z;
		else if(shape == "iZ")
			return ShapeType::iZ;
		else
			throw std::runtime_error("Bad shape type!");
	}
private:
	int _row;
	int _col;
	vector<Shape> _shapes;
};

int main(int argc, char** argv)
{
	/*
	vector<Shape> shapes;
	shapes.push_back(Shape(ShapeType::T, 1));
	shapes.push_back(Shape(ShapeType::T, 2));
	shapes.push_back(Shape(ShapeType::Col, 3));
	shapes.push_back(Shape(ShapeType::L, 4));
	shapes.push_back(Shape(ShapeType::L, 5));
	shapes.push_back(Shape(ShapeType::iL, 6));
	shapes.push_back(Shape(ShapeType::iL, 7));
	shapes.push_back(Shape(ShapeType::iL, 8));
	shapes.push_back(Shape(ShapeType::Square, 9));
	shapes.push_back(Shape(ShapeType::Square, 10));
	shapes.push_back(Shape(ShapeType::Z, 11));
	shapes.push_back(Shape(ShapeType::Z, 12));
	Puzzle puzzle(6, 8, shapes);
	*/

	ArgParser argsParser;
	argsParser.parse(argc, argv);
	Puzzle puzzle(argsParser.Row(),argsParser.Col(), argsParser.Shapes());
	puzzle.solve();
	puzzle.print();
}





