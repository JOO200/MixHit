#include "cCocktail.h"
#include "cJSON.h"
#include "esp_log.h"
#include <stddef.h>

cCocktail::cCocktail(std::string pCocktailName, int pFuel)
{
	mCocktailName = pCocktailName;
	mFuel = pFuel;
}

cCocktail::cCocktail(std::string pCocktailName, std::vector<cIngredient> pIngredients, int pFuel)
{
	mCocktailName = pCocktailName;
	mFuel = pFuel;
	mIngredients = pIngredients;
}

std::string cCocktail::getCocktailName()
{
	return mCocktailName;
}
int cCocktail::getNumberOfIngredients()
{
	return mIngredients.size();
}
cIngredient cCocktail::getIngredient(int pIndex)
{
	return mIngredients[pIndex];
}

cCocktail * cCocktail::fromJson(std::string json)
{
	cJSON *root = cJSON_Parse(json.c_str());
	if (!cJSON_HasObjectItem(root, "Name") ||
		!cJSON_HasObjectItem(root, "Volume") ||
		!cJSON_HasObjectItem(root, "Inhaltsstoffe")) {
		cJSON_Delete(root);
		return NULL;
	}
	std::string name = cJSON_GetObjectItem(root, "Name")->valuestring;
	int volume = cJSON_GetObjectItem(root, "Volume")->valueint;
	cJSON *ingArray = cJSON_GetObjectItem(root, "Inhaltsstoffe");
	std::vector<cIngredient> ingredients;
	cJSON *element;
	cJSON_ArrayForEach(element, ingArray) {
		if (!cJSON_HasObjectItem(element, "Zutat") || !cJSON_HasObjectItem(element, "Anteil")) {
			cJSON_Delete(root);
			return NULL;
		}
		std::string ingName = cJSON_GetObjectItem(element, "Zutat")->valuestring;
		int percent = cJSON_GetObjectItem(element, "Anteil")->valueint;
		ingredients.push_back(cIngredient(String(ingName.c_str()), percent));
	}
	cJSON_Delete(root);
	return new cCocktail(name, ingredients, volume);
}

std::string cCocktail::toJson() {
	return "";
}

int cCocktail::getFuel()
{
	return mFuel;
}
