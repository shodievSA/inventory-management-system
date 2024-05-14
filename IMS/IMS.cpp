#include <iostream>
#include <string>
#include <libxl.h>
#include <conio.h>
#include <cstdlib>
#include "prototypes.h"
using namespace std;
using namespace libxl;

int main()
{
	createInventoryFile();
	showMainMenuOptions();

	return 0;
}

void showMainMenuOptions()
{
	clearScreen();

	int option;

	cout << "\033[0m1. Update inventory\n"
		<< "2. Search item\n"
		<< "3. Low stock\n"
		<< "4. Profits\n"
		<< "5. Inventory value\n"
		<< "6. Exit\n\n"
		<< "\033[32mEnter option (1-6): ";

	cin >> option;

	switch (option)
	{
	case 1: showUpdateInventoryOptions();
		break;

	case 2: searchItem();
		break;

	case 3: getSoonToBeReplenishedItems();
		break;

	case 4: getProfits();
		break;

	case 5: getInventoryValue();
		break;

	case 6:
	{
		cout << "\033[0m";
		exit(0);
	}
	break;

	default:
	{
		clearScreen();

		cout << "\033[31m\"" << option << "\" is invalid command!\033[0m\n\n"
			<< "\033[33mPress any key to return back to the menu.\033[0m\n\n";

		if (_getch())
			showMainMenuOptions();
	}
	}
}

void createInventoryFile()
{
	Book* book = xlCreateXMLBook();

	if (!book->load(L"inventory.xlsx"))
	{
		Sheet* sheet = book->addSheet(L"Sheet1");

		Format* tableTitle = book->addFormat();
		Format* tableTitleBorder = book->addFormat();
		Font* tableTitleFont = book->addFont();

		tableTitleFont->setSize(14);
		tableTitleFont->setColor(COLOR_WHITE);
		tableTitleFont->setBold();

		tableTitle->setFont(tableTitleFont);
		tableTitle->setAlignH(ALIGNH_CENTER);
		tableTitle->setAlignV(ALIGNV_CENTER);
		tableTitle->setBorder(BORDERSTYLE_THIN);
		tableTitle->setFillPattern(FILLPATTERN_SOLID);
		tableTitle->setPatternForegroundColor(COLOR_SEAGREEN);

		tableTitleBorder->setBorder(BORDERSTYLE_THIN);

		Format* tableHeader = book->addFormat();
		Font* tableHeaderFont = book->addFont();

		tableHeaderFont->setSize(13);
		tableHeaderFont->setColor(COLOR_WHITE);

		tableHeader->setAlignH(ALIGNH_CENTER);
		tableHeader->setAlignV(ALIGNV_CENTER);
		tableHeader->setBorder(BORDERSTYLE_THIN);
		tableHeader->setFont(tableHeaderFont);
		tableHeader->setFillPattern(FILLPATTERN_SOLID);
		tableHeader->setPatternForegroundColor(COLOR_DARKTEAL);

		for (int i = 1; i < 6; i++)
		{
			sheet->setCellFormat(2, i, tableTitleBorder);
		}

		sheet->writeStr(2, 0, L"INVENTORY", tableTitle);
		sheet->writeStr(3, 0, L"Item name", tableHeader);
		sheet->writeStr(3, 1, L"Qty purchased", tableHeader);
		sheet->writeStr(3, 2, L"Qty in-stock", tableHeader);
		sheet->writeStr(3, 3, L"Qty sold", tableHeader);
		sheet->writeStr(3, 4, L"Cost price (per item)", tableHeader);
		sheet->writeStr(3, 5, L"Selling price (per item)", tableHeader);

		sheet->setCol(0, 5, -1);
		sheet->setMerge(2, 2, 0, 5);
		sheet->setRowPx(2, 40);
		sheet->setRowPx(3, 50);

		book->save(L"inventory.xlsx");
	}

	book->release();
}

void searchItem()
{
	clearScreen();

	Book* book = xlCreateXMLBook();

	if (book->load(L"inventory.xlsx"))
	{
		Sheet* sheet = book->getSheet(0);

		int lastRow = sheet->lastRow();

		if (lastRow == 4)
		{
			cout << "\033[0mCurrently, your inventory is empty. You need to add items before you can use this command.\033[0m\n\n"
				<< "\033[32mPress any key to return back to the menu.\033[0m\n\n";

			if (_getch())
			{
				showMainMenuOptions();
			}
		}
		else
		{
			wchar_t itemName[50];

			cout << "\033[0m-> Search Item\n\n";

			cin.ignore();

			wcout << L"\033[34mWhat item are you looking for?\033[0m ";
			wcin.getline(itemName, 50);

			bool is_found = 0;
			int foundAt;

			findRow(&is_found, &foundAt, sheet, itemName);

			if (is_found)
			{
				int inStock = sheet->readNum(foundAt, 2);
				double sellingPrice = sheet->readNum(foundAt, 5);

				clearScreen();

				wcout << "Search result: \n\n"
					<< "\033[35mItem name:\033[0m " << itemName << endl << endl
					<< "\033[33mQuantity in-stock:\033[0m " << inStock << endl << endl
					<< "\033[34mSelling Price:\033[0m $" << sellingPrice << endl << endl;
			}
			else
			{
				clearScreen();

				wcout << "Item \033[34m\"" << itemName << "\"\033[0m does not exist in your inventory." << endl;
			}
		}

		cout << "\n\033[32mPress any key to return back to the menu.\033[0m\n\n";

		if (_getch())
		{
			clearScreen();
			showMainMenuOptions();
		}
	}

	book->release();
}

void getSoonToBeReplenishedItems()
{
	clearScreen();

	Book* book = xlCreateXMLBook();

	if (book->load(L"inventory.xlsx"))
	{
		Sheet* sheet = book->getSheet(0);

		int lastRow = sheet->lastRow();

		if (lastRow == 4)
		{
			cout << "\033[0mCurrently, your inventory is empty. You need to add items before you can use this command.\033[0m\n\n"
				<< "\033[32mPress any key to return back to the menu.\033[0m\n\n";

			if (_getch())
			{
				showMainMenuOptions();
			}
		}
		else
		{
			int itemNum = 1;
			bool is_found = 0;

			for (int i = 4; i < lastRow; i++)
			{
				int quantityInStock = sheet->readNum(i, 2);

				if (quantityInStock <= 50)
				{
					is_found = 1;

					const wchar_t* itemName = sheet->readStr(i, 0);

					wcout << "\033[0m" << itemNum
						<< "\n\n\033[33mItem name: " << itemName
						<< "\n\n\033[34mQuantity in-stock: " << quantityInStock << "\033[0m\n\n";

					itemNum++;
				}
			}

			if (!is_found)
			{
				cout << "\033[0mLooks like you don't have any low stock.\n";
			}

			cout << "\n\033[32mPress any key to return back to the menu.\033[0m\n\n";

			if (_getch())
			{
				showMainMenuOptions();
			}
		}
	}

	book->release();
}

void getProfits()
{
	clearScreen();

	Book* book = xlCreateXMLBook();

	if (book->load(L"inventory.xlsx"))
	{
		Sheet* sheet = book->getSheet(0);

		int lastRow = sheet->lastRow();

		if (lastRow == 4)
		{
			cout << "\033[0mCurrently, your inventory is empty. You need to add items before you can use this command.\n\n"
				<< "\033[32mPress any key to return back to the menu.\033[0m\n\n";

			if (_getch())
			{
				showMainMenuOptions();
			}
		}
		else
		{
			double totalRevenues = 0;
			double totalExpenses = 0;
			double totalProfits = 0;

			for (int i = 4; i < lastRow; i++)
			{
				double sellingPrice = sheet->readNum(i, 5);
				int quantitySold = sheet->readNum(i, 3);

				double costPrice = sheet->readNum(i, 4);
				int quantityPurchased = sheet->readNum(i, 1);

				totalRevenues += sellingPrice * quantitySold;
				totalExpenses += costPrice * quantityPurchased;
			}

			totalProfits = totalRevenues - totalExpenses;

			if (totalProfits > 0)
			{
				cout << "\033[36mProfits:\033[0m $" << totalProfits;
			}
			else
			{
				cout << "\033[36mProfits:\033[0m -$" << -(totalProfits);
			}

			cout << "\n\n\033[32mPress any key to return to the menu.\033[0m\n\n";

			if (_getch())
			{
				showMainMenuOptions();
			}
		}
	}

	book->release();
}

void getInventoryValue()
{
	clearScreen();

	Book* book = xlCreateXMLBook();

	if (book->load(L"inventory.xlsx"))
	{
		Sheet* sheet = book->getSheet(0);

		int lastRow = sheet->lastRow();

		if (lastRow == 4)
		{
			cout << "\033[0mCurrently, your inventory is empty. You need to add items before you can use this command.\033[0m"
				<< "\n\n\033[32mPress any key to return back to the menu.\033[0m\n\n";

			if (_getch())
			{
				showMainMenuOptions();
			}
		}
		else
		{
			double inventoryValue = 0;

			for (int i = 4; i < lastRow; i++)
			{
				double costPrice = sheet->readNum(i, 4);
				int quantityPurchased = sheet->readNum(i, 1);

				inventoryValue += costPrice * quantityPurchased;
			}

			cout << "\033[33mInventory value:\033[0m $" << inventoryValue
				<< "\n\n\033[32mPress any key to return to the menu.\033[0m\n\n";

			if (_getch())
			{
				showMainMenuOptions();
			}
		}
	}
}

void findRow(bool* is_found, int* foundAt, Sheet* sheet, wchar_t* itemName)
{
	const int STARTINDEX = 4;
	const int ENDINDEX = sheet->lastRow();

	for (int i = STARTINDEX; i < ENDINDEX; i++)
	{
		const wchar_t* name = sheet->readStr(i, 0);

		if (wcscmp(name, itemName) == 0)
		{
			*is_found = 1;
			*foundAt = i;

			break;
		}
	}
}

void showUpdateInventoryOptions()
{
	clearScreen();

	int option;

	cout << "\033[0m-> Update inventory\n\n"
		<< "1. Add item\n"
		<< "2. Delete item\n"
		<< "3. Update item\n"
		<< "4. Go back\n\n"
		<< "\033[32mEnter option (1-4): ";

	cin >> option;

	switch (option)
	{
	case 1: addItem();
		break;

	case 2: deleteItem();
		break;

	case 3: updateItem();
		break;

	case 4: showMainMenuOptions();
		break;

	default:
	{
		clearScreen();

		cout << "\033[31m\"" << option << "\" is invalid command!\033[0m\n\n"
			<< "\033[33mPress any key to return back to the menu.\033[0m\n\n";

		if (_getch())
		{
			showUpdateInventoryOptions();
		}
	}
	}
}

void updateItemName(Sheet* sheet, Book* book, int foundAt)
{
	clearScreen();

	wchar_t newItemName[30];

	cin.ignore();

	cout << "\033[35mEnter new item name:\033[0m ";
	wcin.getline(newItemName, 30);

	sheet->writeStr(foundAt, 0, newItemName);

	sheet->setCol(0, 5, -1);

	book->save(L"inventory.xlsx");

	clearScreen();

	cout << "Success! Data has been changed."
		<< "\n\n\033[32mPress any key to return back to the menu.\033[0m\n\n";

	if (_getch())
	{
		showUpdateInventoryOptions();
	}
}

void updateQuantityPurchased(Sheet* sheet, Book* book, int foundAt)
{
	clearScreen();

	int newQuantityPurchased;
	int newQuantitySold;

	do
	{
		clearScreen();

		cout << "\033[34mEnter new quantity purchased:\033[0m ";
		cin >> newQuantityPurchased;
	} while (newQuantityPurchased < 0);

	int quantityInStock = sheet->readNum(foundAt, 2);
	newQuantitySold = newQuantityPurchased - quantityInStock;

	sheet->writeNum(foundAt, 1, newQuantityPurchased);
	sheet->writeNum(foundAt, 3, newQuantitySold);

	book->save(L"inventory.xlsx");

	clearScreen();

	cout << "Success! Data has been changed.\n\n"
		<< "\033[32mPress any key to return back to the menu.\033[0m\n\n";

	if (_getch())
	{
		showUpdateInventoryOptions();
	}
}

void updateQuantityInStock(Sheet* sheet, Book* book, int foundAt)
{
	clearScreen();

	int newQuantityInStock;
	int newQuantitySold;

	do
	{
		clearScreen();

		cout << "\033[33mEnter new quantity in-stock:\033[0m ";
		cin >> newQuantityInStock;
	} while (newQuantityInStock < 0);

	int quantityPurchased = sheet->readNum(foundAt, 1);
	newQuantitySold = quantityPurchased - newQuantityInStock;

	sheet->writeNum(foundAt, 2, newQuantityInStock);
	sheet->writeNum(foundAt, 3, newQuantitySold);

	book->save(L"inventory.xlsx");
	book->release();

	clearScreen();

	cout << "Success! Data has been changed.\n\n"
		<< "\033[32mPress any key to return back to the menu.\033[0m\n\n";

	if (_getch())
	{
		showUpdateInventoryOptions();
	}
}

void updateQuantitySold(Sheet* sheet, Book* book, int foundAt)
{
	clearScreen();

	int itemsSold;

	do
	{
		clearScreen();

		cout << "\033[36mHow many items have you sold?\033[0m ";
		cin >> itemsSold;
	} while (itemsSold < 0);

	int quantityInStock = sheet->readNum(foundAt, 2);
	int oldItemsSold = sheet->readNum(foundAt, 3);

	quantityInStock = quantityInStock - itemsSold;
	oldItemsSold = oldItemsSold + itemsSold;

	sheet->writeNum(foundAt, 2, quantityInStock);
	sheet->writeNum(foundAt, 3, oldItemsSold);

	book->save(L"inventory.xlsx");

	clearScreen();

	cout << "Success! Data has been changed."
		<< "\n\n\033[32mPress any key to return back to the menu.\033[0m\n\n";

	if (_getch())
	{
		showUpdateInventoryOptions();
	}

	book->release();
}

void updateCostPrice(Sheet* sheet, Book* book, int foundAt)
{
	double newCostPrice;

	clearScreen();

	do
	{
		clearScreen();

		cout << "\033[31mEnter new cost price:\033[0m $";
		cin >> newCostPrice;
	} while (newCostPrice < 0);

	sheet->writeNum(foundAt, 4, newCostPrice);

	clearScreen();

	cout << "Success! Data has been changed."
		<< "\n\n\033[32mPress any key to return back to the menu.\033[0m\n\n";

	book->save(L"inventory.xlsx");

	if (_getch())
	{
		showUpdateInventoryOptions();
	}

	book->release();
}

void updateSellingPrice(Sheet* sheet, Book* book, int foundAt)
{
	double newSellingPrice;

	clearScreen();

	do
	{
		cout << "\033[36mEnter new selling price (per item):\033[0m $";
		cin >> newSellingPrice;
	} while (newSellingPrice < 0);

	sheet->writeNum(foundAt, 5, newSellingPrice);

	clearScreen();

	cout << "Success! Data has been changed."
		<< "\n\n\033[32mPress any key to return back to the menu.\033[0m\n\n";

	book->save(L"inventory.xlsx");

	if (_getch())
	{
		showUpdateInventoryOptions();
	}

	book->release();
}

void updateItem()
{
	clearScreen();

	Book* book = xlCreateXMLBook();

	if (book->load(L"inventory.xlsx"))
	{
		Sheet* sheet = book->getSheet(0);

		int lastRow = sheet->lastRow();

		if (lastRow == 4)
		{
			cout << "\033[0mCurrently, your inventory is empty. You need to add items before you can use this command.\033[0m"
				<< "\n\n\033[32mPress any key to return back to the menu.\033[0m\n\n";

			book->release();

			if (_getch())
			{
				showUpdateInventoryOptions();
			}
		}
		else
		{
			wchar_t itemName[50];

			cin.ignore();

			wcout << L"\033[36mWhat item do you want to update?\033[0m ";
			wcin.getline(itemName, 50);

			bool is_found = 0;
			int foundAt;

			findRow(&is_found, &foundAt, sheet, itemName);

			if (is_found)
			{
				clearScreen();

				int option;

				wcout << "Item \033[36m\"" << itemName << "\"\033[0m has been found!\n\n"
					<< "What would you like to change?\n"
					<< "\n1. Item name."
					<< "\n2. Quantity purchased."
					<< "\n3. Quantity in-stock."
					<< "\n4. Quantity sold."
					<< "\n5. Selling price."
					<< "\n6. Cost price.\n\n"
					<< "\033[32mEnter option (1-6): ";

				cin >> option;

				switch (option)
				{
				case 1: updateItemName(sheet, book, foundAt);
					break;

				case 2: updateQuantityPurchased(sheet, book, foundAt);
					break;

				case 3: updateQuantityInStock(sheet, book, foundAt);;
					break;

				case 4: updateQuantitySold(sheet, book, foundAt);;
					break;

				case 5: updateSellingPrice(sheet, book, foundAt);
					break;

				case 6: updateCostPrice(sheet, book, foundAt);
					break;

				default:
				{
					clearScreen();

					cout << "Invalid option! Press any key to return back to the menu" << endl;

					if (_getch())
						showUpdateInventoryOptions();
				}
				}
			}
			else
			{
				clearScreen();

				wcout << "Item \033[36m\"" << itemName << "\"\033[0m does not exist in your inventory."
					<< "\n\n\033[32mPress any key to return back to the menu.\033[0m\n\n";

				if (_getch())
					showUpdateInventoryOptions();
			}
		}
	}
}

void deleteItem()
{
	clearScreen();

	Book* book = xlCreateXMLBook();

	if (book->load(L"inventory.xlsx"))
	{
		Sheet* sheet = book->getSheet(0);

		int lastRow = sheet->lastRow();

		if (lastRow == 4)
		{
			cout << "\033[0mCurrently, your inventory is empty. You need to add items before you can use this command.\033[0m"
				<< "\n\n\033[32mPress any key to return back to the menu.\033[0m\n\n";

			book->release();

			if (_getch())
				showUpdateInventoryOptions();
		}
		else
		{
			wchar_t itemName[50];

			cin.ignore();

			wcout << L"\033[33mWhat item do you want to delete?\033[0m ";
			wcin.getline(itemName, 50);

			bool is_found = 0;
			int foundAt;

			findRow(&is_found, &foundAt, sheet, itemName);

			clearScreen();

			if (is_found)
			{
				char userResponse;

				wcout << "Item \033[33m\"" << itemName << "\"\033[0m has been found!\n\n"
					<< "\033[31mAre you sure you want to remove it from your inventory? (y/n) ";

				cin >> userResponse;

				if (userResponse == 'y' || userResponse == 'Y')
				{
					sheet->removeRow(foundAt, foundAt);

					book->save(L"inventory.xlsx");

					clearScreen();

					std::wcout << "\033[0mItem \033[33m\"" << itemName << "\"\033[0m has been removed from your inventory.\n\n"
						<< "\033[32mPress any key to return back to the menu.\033[0m\n\n";

					if (_getch())
					{
						showUpdateInventoryOptions();
					}
				}
				else if (userResponse == 'n' || userResponse == 'N')
				{
					showUpdateInventoryOptions();
				}
				else
				{
					clearScreen();

					cout << "\033[31m" << userResponse << " is invalid input!\033[0m\n\n"
						<< "\033[32mPress any key to return back to the menu.\033[0m\n\n";

					if (_getch())
					{
						showUpdateInventoryOptions();
					}
				}
			}
			else
			{
				wcout << "Item \033[33m\"" << itemName << "\"\033[0m could not be found in your inventory.";
			}

			book->release();

			cout << "\n\n\033[32mPress any key to return back to the menu.\033[0m\n\n";

			if (_getch())
			{
				showUpdateInventoryOptions();
			}
		}
	}
}

void addItem()
{
	char confirmation;

	Item* item = new Item;

	item->createNewItem();

	wcout << "Details:\n\n"
		<< "\033[34mItem name:\033[0m " << item->getItemName()
		<< "\n\n\033[32mQuantity purchased:\033[0m " << item->getQuantityPurchased()
		<< "\n\n\033[36mQuantity in-stock:\033[0m " << item->getQuantityInStock()
		<< "\n\n\033[33mCost price (per item):\033[0m $" << item->getCostPrice()
		<< "\n\n\033[35mSelling price:\033[0m $" << item->getSellingPrice()
		<< "\n\n\nConfirm? (y/n): ";

	cin >> confirmation;

	if (confirmation == 'y' || confirmation == 'Y')
	{
		clearScreen();

		Book* book = xlCreateXMLBook();

		if (book->load(L"inventory.xlsx"))
		{
			Sheet* sheet = book->getSheet(0);
			int lastRow = sheet->lastRow();

			Font* font = book->addFont();
			font->setSize(12);

			Format* itemNameFormat = book->addFormat();

			itemNameFormat->setBorder(BORDERSTYLE_THIN);
			itemNameFormat->setAlignH(ALIGNH_CENTER);
			itemNameFormat->setAlignV(ALIGNV_CENTER);
			itemNameFormat->setNumFormat(NUMFORMAT_TEXT);
			itemNameFormat->setFont(font);

			Format* quantityPurchasedFormat = book->addFormat();

			quantityPurchasedFormat->setBorder(BORDERSTYLE_THIN);
			quantityPurchasedFormat->setAlignH(ALIGNH_CENTER);
			quantityPurchasedFormat->setAlignV(ALIGNV_CENTER);
			quantityPurchasedFormat->setNumFormat(NUMFORMAT_NUMBER_SEP);
			quantityPurchasedFormat->setFont(font);

			Format* inStockFormat = book->addFormat();

			inStockFormat->setBorder(BORDERSTYLE_THIN);
			inStockFormat->setAlignH(ALIGNH_CENTER);
			inStockFormat->setAlignV(ALIGNV_CENTER);
			inStockFormat->setNumFormat(NUMFORMAT_NUMBER_SEP);
			inStockFormat->setFont(font);

			Format* quantitySoldFormat = book->addFormat();

			quantitySoldFormat->setBorder(BORDERSTYLE_THIN);
			quantitySoldFormat->setAlignH(ALIGNH_CENTER);
			quantitySoldFormat->setAlignV(ALIGNV_CENTER);
			quantitySoldFormat->setNumFormat(NUMFORMAT_NUMBER_SEP);
			quantitySoldFormat->setFont(font);

			Format* costPriceFormat = book->addFormat();

			costPriceFormat->setBorder(BORDERSTYLE_THIN);
			costPriceFormat->setAlignH(ALIGNH_CENTER);
			costPriceFormat->setAlignV(ALIGNV_CENTER);
			costPriceFormat->setNumFormat(NUMFORMAT_CURRENCY_D2_NEGBRARED);
			costPriceFormat->setFont(font);

			Format* sellingPriceFormat = book->addFormat();

			sellingPriceFormat->setBorder(BORDERSTYLE_THIN);
			sellingPriceFormat->setAlignH(ALIGNH_CENTER);
			sellingPriceFormat->setAlignV(ALIGNV_CENTER);
			sellingPriceFormat->setNumFormat(NUMFORMAT_CURRENCY_D2_NEGBRARED);
			sellingPriceFormat->setFont(font);

			sheet->writeStr(lastRow, 0, item->getItemName(), itemNameFormat);
			sheet->writeNum(lastRow, 1, item->getQuantityPurchased(), quantityPurchasedFormat);
			sheet->writeNum(lastRow, 2, item->getQuantityInStock(), inStockFormat);
			sheet->writeNum(lastRow, 3, item->getQuantitySold(), quantitySoldFormat);
			sheet->writeNum(lastRow, 4, item->getCostPrice(), costPriceFormat);
			sheet->writeNum(lastRow, 5, item->getSellingPrice(), sellingPriceFormat);

			sheet->setCol(0, 5, -1);
			sheet->setRowPx(lastRow, 50);

			book->save(L"inventory.xlsx");

			wcout << "Item \033[36m\"" << item->getItemName() << "\"\033[0m has been registered in your inventory."
				<< "\n\n\033[32mPress any key to return back to the menu.\033[0m\n\n";

			if (_getch())
			{
				showUpdateInventoryOptions();
			}
		}

		book->release();
	}
	else if (confirmation == 'n' || confirmation == 'N')
	{
		delete item;
		showUpdateInventoryOptions();
	}
	else
	{
		clearScreen();

		delete item;

		cout << "\033[31m\"" << confirmation << "\" is invalid command!\033[0m"
			<< "\n\n\033[32mPress any key to return back to the menu.\033[0m\n\n";

		if (_getch())
		{
			showUpdateInventoryOptions();
		}
	}
}

void clearScreen() { system("cls"); }

void Item::createNewItem()
{
	clearScreen();

	cin.ignore();

	wcout << L"\033[34mItem name:\033[0m ";
	wcin.getline(itemName, 50);

	do
	{
		clearScreen();

		cout << "\033[32mQuantity purchased:\033[0m ";
		cin >> quantityPurchased;
	} while (quantityPurchased < 0);

	do
	{
		clearScreen();

		cout << "\033[36mQuantity in-stock:\033[0m ";
		cin >> quantityInStock;
	} while (quantityInStock < 0);

	quantitySold = quantityPurchased - quantityInStock;

	do
	{
		clearScreen();

		cout << "\033[33mCost price (per item):\033[0m $";
		cin >> costPrice;
	} while (costPrice < 0);

	do
	{
		clearScreen();

		cout << "\033[35mSelling price (per item):\033[0m $";
		cin >> sellingPrice;
	} while (sellingPrice < 0);

	clearScreen();
}

