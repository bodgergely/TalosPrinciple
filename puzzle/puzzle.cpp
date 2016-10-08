#include <vector>
#include <list>
#include <unordered_map>
#include <cassert>
#include <iostream>
#include <sstream>
#include <cstring>
#include <chrono>
#include <unordered_set>
#include <ctime>

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
	
	string toString() const
	{
		switch(_type)
		{
		case ShapeType::L:
			return "L";
			
		case ShapeType::iL:
			return "iL";
			
		case ShapeType::Square:
			return "Square";
			
		case ShapeType::Col:
			return "Col";
			
		case ShapeType::Z:
			return "Z";
			
		case ShapeType::iZ:
			return "iZ";
			
		case ShapeType::T:
			return "T";
			
		}
	}

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
	Puzzle(int numRows, int numCols, const vector<Shape>& shapes, bool randomize = false, int timeout = 5) : _startShapes(shapes), _board(numRows, numCols),
																																					_randomize(randomize),
																																					_timeout(timeout){}
	void solve()
	{
		int tries = 1;
		while(true)
		{
			cout << "Attempt: " << tries++ << "\n";
			vector<Shape> shapesToStart = _startShapes;
			_timedOut = false;
			if(_randomize)
			{
				shapesToStart = randomize(_startShapes);
			}
			
			_startTime = chrono::system_clock::now();
			print(shapesToStart);
			if(_solve(shapesToStart))
				break;
				
			if(_randomize)
				continue;
			else
				break;
		}
	}
	void print()
	{
		cout << _board.toString() << endl;
	}

private:

	void print(const vector<Shape>& shapes)
	{
		for(const Shape& sh : shapes)
		{
			cout << sh.toString() << " ";
		}
		cout << "\n";
	}

	vector<Shape> randomize(const vector<Shape>& shapes)
	{
		srand(time(NULL));
		vector<Shape> shapesReordered;
		vector<int> indexes;
		unordered_set<int> indexchosen;
		int numofindexes = shapes.size();
		while(indexchosen.size()!=numofindexes)
		{
			int i = rand() % numofindexes;
			if(indexchosen.find(i)==indexchosen.end())
			{
				indexchosen.insert(i);
				indexes.push_back(i);
			}
		}
		
		for(int i=0;i<indexes.size();i++)
			shapesReordered.push_back(shapes[indexes[i]]);
		
		return shapesReordered;
	}

	bool _solve(vector<Shape> shapes)
	{
		for(int i=0;i<shapes.size();i++)
		{
			auto now = chrono::system_clock::now();
			if(!_solutionFound && _timedOut)
			{
				return false;
			}
			if(!_solutionFound && _timeout!= 0 && chrono::duration_cast<chrono::seconds>(now-_startTime).count() > _timeout)
			{
				cout << "Timed out\n";
				_timedOut = true;
				return false;
			}
			bool placed = place(shapes[i]);
			if(placed)
			{
				//cout << _board.toString() << endl;

				if(_board.isFull())
					return true;
				vector<Shape> newShapeSet = _excludeFromSet(i, shapes);
				bool solved = _solve(newShapeSet);
				if(solved)
				{
					cout << "Solution found!\n";
					_solutionFound = true;
					return true;
				}
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
	bool	_randomize;
	int _timeout;
	std::chrono::system_clock::time_point _startTime;
	bool _timedOut{false};
	bool _solutionFound{false};
};


class ArgParser
{
public:
	int Row() const {return _row;}
	int Col() const {return _col;}
	bool Rand() const {return _rand;}
	int TimeOut() const {return _timeout;}

	const vector<Shape>& Shapes() {return _shapes;}

	void parse(int argc, char** argv)
	{
		_row = atoi(argv[1]);
		_col = atoi(argv[2]);
		int numOfShapes = atoi(argv[3]);
		for(int i=4;i<4+numOfShapes;i++)
		{
			string shape = string(argv[i]);
			ShapeType type = shapeType(shape);
			_shapes.push_back(Shape(type, i-2));
		}
		
		if(argc >= 4+numOfShapes && string(argv[4+numOfShapes]) == string("-r") )
		{
			cout << "Randomizer mode set.\n";
			_rand = true;
		}
		
		if(argc >= 4+numOfShapes + 1)
		{
			_timeout = atoi(argv[4+numOfShapes+1]);
		}
		else
		{
			_timeout = 0;
		}
		cout << "time out: " << _timeout << endl;
		
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
	bool _rand {false};
	int _timeout;
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
	Puzzle puzzle(argsParser.Row(),argsParser.Col(), argsParser.Shapes(), argsParser.Rand(), argsParser.TimeOut());
	puzzle.solve();
	puzzle.print();
}





