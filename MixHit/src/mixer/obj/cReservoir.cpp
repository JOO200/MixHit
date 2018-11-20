#include "cReservoir.h"
#include "esp_log.h"

static const char* LOG_TAG = "Reservoir";
/*
cReservoir::cReservoir()
{
	mNumberOfReservoir = 0;
	for (int i = 0; i<MaxNumberOfReservoir; i++)
	{
		mNames[i] = "";
		mm[i] = 0;
		mb[i] = 0;
		mSumServed[i] = 0;
	}
}
*/
SingleReservoir::SingleReservoir(int pId, std::string pName, double pm, double pb, int pSumServed)
{
	id = pId;
	mName = pName;
	mm = pm;
	mb = pb;
	mSumServed = pSumServed;
}

int SingleReservoir::getId()
{
	return id;
}

bool SingleReservoir::getInitState()
{
	ESP_LOGD(LOG_TAG, "[%d] m=%f, b=%f, InitOk=xx", id, mm, mb);
	return mm != 0 || mb != 0;
}

std::string SingleReservoir::getName()
{
	return mName;
}

double SingleReservoir::getSum()
{
	return mSumServed;
}

void SingleReservoir::addToSum(int amount, bool reset) {
	if(reset) mSumServed = 0;
	mSumServed += amount;
}

int SingleReservoir::AmountToTime(double pml)
{
	double time = (double)pml * mm + mb;
	ESP_LOGD(LOG_TAG, "m=%f, b=%f, Time=%f", mm, mb, time);
	return time;
	/* Linearer Zusammenhang:
	Zeit (Einheit: ms) = Wunschmaenge (Einheit: ml) * Steigung (Einheit: ms / ml) + Offset (Einheit: ms);
	Wobei davon ausgegangen wird, dass ein Milliliter einem Gramm entsprincht.
	*/
}


cReservoir::cReservoir()
{
	mNumberOfReservoir = 0;
}

cReservoir::~cReservoir()
{/*
	for (std::map<int, cReservoir*>::iterator it = resMap.begin(); it != resMap.end(); ++it)
	{
		delete it->second;
	}*/
}

bool cReservoir::addReservoir(SingleReservoir* reservoir)
{
	if(resMap.count(reservoir->getId())) {
		ESP_LOGW("Reservoir", "Reservoir with Id %d already exist. Name: %s", reservoir->getId(), reservoir->getName().c_str());
		return false;
	}
	resMap.insert(std::pair<int, SingleReservoir*>(reservoir->getId(), reservoir));
	mNumberOfReservoir++; // Anzahl an verwendeten Vorratsbehaelter um eins erhoehen.
	return true;
}

bool cReservoir::addReservoir(int id, std::string pName, double pm, double pb)
{
	return addReservoir(new SingleReservoir(id, pName, pm, pb));
}

bool cReservoir::addReservoir(int id, String pName, double pm, double pb)
{
	return addReservoir(new SingleReservoir(id, pName.c_str(), pm, pb));
}

void cReservoir::setParams(double pm[], double pb[])
{
	// TODO: Params
}

std::string cReservoir::getReservoirName(int pIndex)
{
	std::map<int,SingleReservoir*>::iterator it = resMap.find(pIndex);
	if(it != resMap.end()) {
		return it->second->getName();
	}
	return "";
}

int cReservoir::getReservoirIndex(std::string pName)
{
	for (std::map<int, SingleReservoir*>::iterator it = resMap.begin(); it != resMap.end(); ++it)
	{
		if(it->second->getName() == pName) // Prueft, ob der Vorratsbehaelter an dieser Stelle den gesuchten Namen (pName) hat.
		{
			return it->first; // Falls dies der Fall ist, wird der Index dieses Vorratsbehaelters uebergeben.
		}
	}
	return -1; // falls keine Uebereinstimmung gefunden wurde.
}

int cReservoir::getReservoirIndex(String pName)
{
	return getReservoirIndex(std::string(pName.c_str()));
}

int cReservoir::getNumberOfReservoir()
{
	return mNumberOfReservoir;
}

SingleReservoir * cReservoir::getReservoir(int index)
{
	std::map<int, SingleReservoir*>::iterator it = resMap.find(index);
	if (it != resMap.end()) return it->second;
	else return NULL;
}

bool cReservoir::getInitStateAll()
{
	for (std::map<int, SingleReservoir*>::iterator it = resMap.begin(); it != resMap.end(); ++it)
		{
			if(it->second->getInitState() == false)
			{
				return false;
			}
		}
	// Falls alle Vorratsbehaelter initialisiert sind
	return true;
}

int cReservoir::AmountToTime(int pIndex, int pml)
{
	if (getReservoir(pIndex) != NULL) {
		return getReservoir(pIndex)->AmountToTime(pml);
	}
	return 0;
}

void cReservoir::setMB(int pIndex, double pm, double pb)
{
	if (getReservoir(pIndex) != NULL) {
		getReservoir(pIndex)->mm = pm;
		getReservoir(pIndex)->mb = pb;
	}
}

bool cReservoir::getInitState(int pIndex)
{
	if (getReservoir(pIndex) != NULL) {
		return getReservoir(pIndex)->getInitState();
	}
	return false;
}

void cReservoir::addToSum(int pIndex, int pAmount)
{
	if (getReservoir(pIndex) != NULL) {
		getReservoir(pIndex)->addToSum(pAmount);
	}
}

double cReservoir::getSum(int pIndex)
{
	if (getReservoir(pIndex) != NULL) {
		return getReservoir(pIndex)->getSum();
	}
	return -1;
}

double cReservoir::getM(int pIndex)
{
	if (getReservoir(pIndex) != NULL) {
		return getReservoir(pIndex)->mm;
	}
	return -1;
}

double cReservoir::getB(int pIndex)
{
	if (getReservoir(pIndex) != NULL) {
		return getReservoir(pIndex)->mb;
	}
	return -1;
}
