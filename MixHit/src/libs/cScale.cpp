#include "cScale.h"

cScale::cScale()
{
	mTare = 643, 4368;
}
cScale::cScale(byte pPinScaleData, byte pPinScaleCLK)
{
	mHX711Scale = HX711(pPinScaleData, pPinScaleCLK);
	//mHX711Scale.set_scale(1);
	mTare = 650.3619+24;
}
double cScale::getWeight()
{


	return (double)mHX711Scale.get_units(10) * 0.0047733 - mTare; // Cast als Double, damit das Zwischenergebniss nicht gerundet wird.
  //return (double)mHX711Scale.get_units(10); // Cast als Double, damit das Zwischenergebniss nicht gerundet wird.


}
double cScale::getWeight(int pPoints)
{


	return (double)mHX711Scale.get_units(pPoints) * 0.0047733 - mTare; // Cast als Double, damit das Zwischenergebniss nicht gerundet wird.


}
void cScale::Tare()
{
	mTare = 0;
	mTare = getWeight();
}



// Folgende Werte wurden durch Messen ermittelt: Mit 5 V Spannungsversorgung HX711
//    Gewicht (G)    digitaler Wert (DW)
// 1: 0g      =  134800
// 2: 100g    =  155750 -->Prüfgewicht
// Linearer Zusammenhang: G = m * DW + b
// m = (G1 - G2) / (DW1 - DW2) = (0 - 100) / (134800 - 155750) = 0,0047733 ;
// b = G1 - m * DW1 = 0 - 0,0047733 * 134800= - 643,4368






//Folgende Werte wurden durch Messen ermittelt: Mit 3,3 V Spannungsversorgung HX711
  //    Gewicht (G)    digitaler Wert (DW)
  // 1: 0g      =  61330
  // 2: 100g    =  71800 -->Prüfgewicht
  // Linearer Zusammenhang: G = m * DW + b
  // m = (G1 - G2) / (DW1 - DW2) = (0 - 100) / (61330 - 71800) =0,009551 ;
  // b = G1 - m * DW1 = 0 - 0,009551 * 61330= - 585,77