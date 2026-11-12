#include <iostream>
#include <cmath>
#include <vector>
using namespace std;

//constants
const double pi = 3.14159265358979;
const double EPS0 = 8.854187817e-12;
const double MU0 = 4.0 * pi * 1e-7;
//new field, other classes now inherit from this class
class Field
{
	protected: //protected vs private from assignment 7. derived classes can interact but external code can't. also replaces old M and E arrays!
		double *value;
	public:
		Field()
		{
			value = new double[3];
			value[0] = 0.0;
			value[1] = 0.0;
			value[2] = 0.0;
		}
		//component constructor call, E field and M field will call this
		Field(double x, double y, double z)
		{
			value = new double[3];
			value[0] = x;
			value[1] = y;
			value[2] = z;
		}
		//copy constructor, needed because you can't call delete twice on the same object.
		Field(const Field &copy)
		{
			value = new double[3];
			for(int i = 0; i < 3; i++)
			{
				value[i] = copy.value[i];
			}
		}
		//copy assignment
		Field& operator = (const Field &copy)
		{
			for(int i = 0; i < 3; i++)
			{
				value[i] = copy.value[i];
			}
			return *this;
		}
		//destructor
		virtual ~Field()
		{
			delete[] value;
		}
		//new setters and getters
		void setX(double x)
		{
			value[0] = x;
		}
		void setY(double y)
		{
			value[1] = y;
		}
		void setZ(double z)
		{
			value[2] = z;
		}
		//getters
		double getX() const
		{
			return value[0];
		}
		double getY() const
		{
			return value[1];
		}
		double getZ() const
		{
			return value[2];
		}
		//mag calc, shared by both classes
		double calculateMagnitude() const
		{
			return sqrt(value[0] * value[0] + value[1] * value[1] + value[2] * value[2]);
		}
		//new magnitude function required by assignment
		void printMagnitude() const
		{
			cout << "Components: x = " << value[0] << ", y = " << value[1] << ", z = " << value[2] << "\n";
			cout << "Magnitude: " << calculateMagnitude() << "\n";
		}
};

//Electric Field code!
class Electric_Field : public Field
{
	private:
		double resultE;	//for Gauss' law calculation	
	public:
		Electric_Field() : Field(), resultE(0.0)
		{
		}
		Electric_Field(double x, double y, double z) : Field(x, y, z), resultE(0.0)
		{
		}
		Electric_Field(const Electric_Field &copy) : Field(copy), resultE(copy.resultE) //Field(copy) uses base class to create copy, then receives specific field data.
		{
		}
		Electric_Field& operator = (const Electric_Field &copy)
		{
			Field::operator = (copy);
			resultE = copy.resultE;
			return *this;
		}
		//original inner product
		double getInnerp() const
		{
			return value[0] * value[0] + value[1] * value[1] + value[2] * value[2];
		}

		//Gauss' Law. I don't understand any of this stuff about spherical charges, but this is what it's supposed to be. Q is coulombs, r is (radius?) in meters. 
		void calculateField(double q, double r)
		{
			if(r <= 0.0)
			{
				cout << "Distance can only be positive!\n";
				return; //nothing, void function. 
			}
			resultE = q / (4.0 * pi * r * r * EPS0);
		}
		//getter for resultE
		double getresultE() const
		{
			return resultE;
		}
		//adds fields component by component and returns new field
		Electric_Field operator+(const Electric_Field &rhs) const
		{
			return Electric_Field(value[0] + rhs.value[0], value[1] + rhs.value[1], value[2] + rhs.value[2]);
		}
		friend ostream& operator << (ostream &out, const Electric_Field &e)
		{
			out << "(" << e.value[0] << ", " << e.value[1] << ", " << e.value[2] << ")";
			return out;
		}
};

//Magnetic field code! also inherits from Field now.
class Magnetic_Field : public Field
{
	private:
		double resultB; //ampere's law result
	public:
		Magnetic_Field() : Field(), resultB(0.0)
		{
		}
		Magnetic_Field(double x, double y, double z) : Field(x, y, z), resultB(0.0)
		{
		}
		Magnetic_Field(const Magnetic_Field &copy) : Field(copy), resultB(copy.resultB)
		{
		}
		Magnetic_Field& operator = (const Magnetic_Field &copy)
		{
			Field::operator = (copy);
			resultB = copy.resultB;
			return *this;
		}
		//original unit vector code
		void uVector() const
		{
			double mag = calculateMagnitude();
			if(mag == 0.0)
			{
				cout << "Undefined vector.\n";
				return;
			}
			cout << "Magnetic Field unit vector: " << value[0]/mag << ", " << value[1]/mag << ", " << value[2]/mag << "\n";
		}
		//ampere's law. again, haven't taken these classes yet so this is a little foreign to me. I'm only in calc 1/phys 1. I in amps, r in meters again, B or teslas. 
		void calculateField(double i, double r)
		{
			if(r <= 0.0)
			{
				cout << "Distance can be positive only!\n";
				return;
			}
			resultB = (MU0 * i) / (2.0 * pi * r);
		}
		//getters and setters for new values
		double getresultB() const
		{
			return resultB;
		}
		Magnetic_Field operator + (const Magnetic_Field &rhs) const
		{
			return Magnetic_Field(value[0] + rhs.value[0], value[1] + rhs.value[1], value[2] + rhs.value[2]);
		}
		friend ostream& operator << (ostream &out, const Magnetic_Field &m)
		{
			out << "(" << m.value[0] << ", " << m.value[1] << ", " << m.value[2] << ")";
			return out;
		}
};

//new main
int main()
{

	//creating new fields
	Electric_Field EF1(0, 1e5, 1e3); //from rubric
	Magnetic_Field MF1(0.5, 1.2, 2.0);
	//cout chains
	cout << "====Default fields====\n";
	cout << "Electric Field:\n";
	EF1.printMagnitude(); //call function and print values
	cout << "EF1 inner product: " << EF1.getInnerp() << "\n\n";
	cout << "Magnetic Field:\n";
	MF1.printMagnitude();
	MF1.uVector();
	//Gauss and Ampere stuff
	double q = 2e-6; //2 microcoulombs, random
	double re = 0.05; //5cm
	double i = 10.0; //10 amps
	double rb = 0.02; //2cm
	
	EF1.calculateField(q, re);
	MF1.calculateField(i, rb);

	cout << "\n====Gauss' and Ampere's law values====\n";
	cout << "E at r = " << re << "m from q = " << q << " C: " << EF1.getresultE() << " V/m\n";
	cout << "B at r = " << rb << "m from i = " << i << " A: " << MF1.getresultB() << "T\n";

	Electric_Field ef1, ef2(1.0, 2.0, 3.0), ef3(4.0, 5.0, 6.0);
	ef1 = ef2 + ef3; //operator+ builds a temporary copy and operator = builds a deep one
	Magnetic_Field mf1, mf2(0.1, 0.2, 0.3), mf3(0.4, 0.5, 0.6);
	mf1 = mf2 + mf3;

	cout << "\n====Overloaded operators====\n";
	cout << "ef2 = " << ef2 << "\n";
	cout << "ef3 = " << ef3 << "\n";
	cout << "ef1 = ef2 + ef3: " << ef1 << endl;
	cout << "mf2 = " << mf2 << "\n";
	cout << "mf3 = " << mf3 << "\n";
	cout << "mf1 = mf2 + mf3: " << mf1 << endl;
	//copy demonstration to verify it works
	Electric_Field EF2 = EF1;
	EF2.setX(99.0);
	cout << "\nCopy test: EF1: " << EF1 << " EF2(copy, x value changed): " << EF2 << endl;

	return 0;
}

