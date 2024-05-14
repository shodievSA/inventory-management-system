#ifndef PROTOTYPES_H
#define PROTOTYPES_H

class Item
{
private:

	wchar_t itemName[50];
	double quantityPurchased;
	double quantityInStock;
	double quantitySold;
	double costPrice;
	double sellingPrice;

public:

	void createNewItem();
	const wchar_t* getItemName() const { return itemName; }
	double getQuantityPurchased() const { return quantityPurchased; }
	double getQuantityInStock() const { return quantityInStock; }
	double getQuantitySold() const { return quantitySold; }
	double getCostPrice() const { return costPrice; }
	double getSellingPrice() const { return sellingPrice; }
};

void showMainMenuOptions();
void showUpdateInventoryOptions();
void addItem();
void clearScreen();
void createInventoryFile();
void searchItem();
void getSoonToBeReplenishedItems();
void getProfits();
void getInventoryValue();
void findRow(bool* is_found, int* foundAt, libxl::Sheet* sheet, wchar_t* itemName);
void deleteItem();
void updateItem();
void updateItemName(libxl::Sheet* sheet, libxl::Book* book, int foundAt);
void updateQuantityPurchased(libxl::Sheet* sheet, libxl::Book* book, int foundAt);
void updateQuantitySold(libxl::Sheet* sheet, libxl::Book* book, int foundAt);
void updateQuantityInStock(libxl::Sheet* sheet, libxl::Book* book, int foundAt);
void updateCostPrice(libxl::Sheet* sheet, libxl::Book* book, int foundAt);
void updateSellingPrice(libxl::Sheet* sheet, libxl::Book* book, int foundAt);

#endif 
