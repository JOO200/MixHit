#ifndef _CVORRATSBEHAELTER_H_
#define _CVORRATSBEHAELTER_H_

#include <string>
#include <map>
#include "WString.h"

#define MAX_NUMBER_RESERVOIR 8


class SingleReservoir
{
public:
	SingleReservoir(int id, std::string pName, double pm, double pb, int pSumServed = 0);

	bool getInitState();

	int AmountToTime(double pml);

	void addToSum(int amount, bool reset = false);

	int getId();
	std::string getName();
	double getSum();

	double mm;
	double mb;
private:
	int id;
	std::string mName;
	int mSumServed;
};

class cReservoir
{
public:
	cReservoir();												// Konstruktor mit Namen
	~cReservoir();
	bool addReservoir(SingleReservoir* reservoir);							// Fuegt ein Vorratsbehaelter hinzu (ohne Parameter zur Berechung von Maeche in Zeit).
	bool addReservoir(int id, std::string pName, double pm, double pb);		// Fuegt ein Vorratsbehaelter hinzu (mit Parameter).
	bool addReservoir(int id, String pName, double pm, double pb);		// Fuegt ein Vorratsbehaelter hinzu (mit Parameter).
	void setParams(double pm[], double pb[]);					// Setzt alle Parameter fuer die umrechnung von Maenge in Zeit.
	std::string getReservoirName(int pIndex);						// Gibt den Namen des Vorrates an der gegebenen Stelle zurueck.
	int getReservoirIndex(std::string pName);						// Gibt den Index des Vorrates mit dem angegebenen Namen zurueck.
	int getReservoirIndex(String pName);						// Gibt den Index des Vorrates mit dem angegebenen Namen zurueck.
	int getNumberOfReservoir();									// Gibt die Anzahl an Verwendeten Vorratsgehaelter zurueck.
	SingleReservoir* getReservoir(int index);

	bool getInitStateAll();										// Gibt an, ob jeder Vorratsbehaelter initialisiert ist.
	int AmountToTime(int pIndex, int pml);						// Gibt die Oeffungszeit des Ventiles fuer die angegebene Maenge zurueck.
	void setMB(int pIndex, double pm, double pb);				// Set die Parameter fur die Umrechnung von Maenge in Zeit fuer den angegebenen Vorratsbehaelter.
	bool getInitState(int pIndex);								// Gibt an, ob der Vorratsbehaelter Initialisiert ist oder nicht.
	void addToSum(int pIndex, int pAmount);						// Fuegt eine Maenge zur gesamten ausgeschenkten Maenge eines Vorrates seit Programmstart hinzu (Angabe in ml).
	double getSum(int pIndex);									// Gibt die gesamte ausgeschenkte Maenge des angegebenen Vorrates seit Programmstart zurueck (Angabe in ml).
	double getM(int pIndex);									// Gibt die Steigung des Lienearen zusammenhangs zwischen Maenge und Zeit zurueck.
	double getB(int pIndex);					
private:
	std::map<int, SingleReservoir*> resMap;
	int mNumberOfReservoir;										// Anzahl an verwendeten Vorratsbehalter.
#if 0
	std::string mNames[MAX_NUMBER_RESERVOIR];						// Namen der Vorratsbehaelter
	//bool mInitOK[MaxNumberOfReservoir];							// Initialisierungszustaende der Vorratsbehaleter
	double mm[MAX_NUMBER_RESERVOIR];							// Steigung; Für die umrechnung von cl bzw. g in die Öffungszeig der Valve.
	double mb[MAX_NUMBER_RESERVOIR];							// Offset; Für die umrechnung von cl bzw. g in die Öffungszeig der Valve.
	int mSumServed[MAX_NUMBER_RESERVOIR];						// Gesamte ausgeschenkte Maenge des angegebenen Vorrates seit Programmstart zurueck (Angabe in ml).
#endif
};

#endif
