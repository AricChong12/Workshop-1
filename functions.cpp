#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/statement.h>
#include <iostream>
#include <mysql_connection.h>
#include <mysql_driver.h>
#include <cppconn/prepared_statement.h>

#include <cstdlib>
#include <conio.h>
#include <iomanip>
#include <string>

//test
#include <ctime>
#include <sstream>

using namespace std;

int CurrentStock;
int TotalPayment;
int TotalPricePerOrder;
int OrderID, ItemID2;

//clear cli screen
void Refresh() {
	system("cls");
}



void RegisterCustomer(sql::Connection* con , string CustomerName, string CustomerEmail, string CustomerPassword, string CustomerAddress, string CustomerPhone) {
	try {
		sql::PreparedStatement* pstmt;
		pstmt = con->prepareStatement("INSERT into customer (Name, Email, Password, Address, PhoneNo) VALUES (?, ?, ?, ?, ?)");

		//insert data into db
		pstmt->setString(1, CustomerName);
		pstmt->setString(2, CustomerEmail);
		pstmt->setString(3, CustomerPassword);
		pstmt->setString(4, CustomerAddress);
		pstmt->setString(5, CustomerPhone);

		pstmt->executeUpdate();

		//display successful msg
		cout << "Registration Successful!" << endl << endl;
		delete pstmt;

		//call login method maybe

	}
	catch (sql::SQLException& e) { // catch errors
		cout << "Error: " << e.what() << endl;
	}
}




bool CheckCustomerCredentials(sql::Connection* con, string LoginEmail, string LoginPassword) {
	try {
		sql::PreparedStatement* pstmt;
		sql::ResultSet* res;

		pstmt = con->prepareStatement("SELECT Name, CUSTOMER_ID FROM customer WHERE Email = ? AND Password = ?");
		pstmt->setString(1, LoginEmail);
		pstmt->setString(2, LoginPassword);

		res = pstmt->executeQuery();

		//check email and password
		if (res->next()) {
			string LoginCustomerName = res->getString("Name");
			//int LoginCustomerID = res->getInt("CUSTOMER_ID");
			cout << "Login Successful! Welcome " << LoginCustomerName << "!" << endl << endl;
			//Refresh();
			return true;
		}
		else {
			cout << "Login Failed! Please Try Again!" << endl << endl;
			//added
			Refresh();
			return false;
		}
	}
	catch (sql::SQLException& e) { // catch errors
		cout << "Error: " << e.what() << endl;
	}
}










void DisplayItemMenu(sql::Connection* con) {
	try {
		sql::Statement* stmt;
		sql::ResultSet* res;

		stmt = con->createStatement();
		res = stmt->executeQuery("SELECT store.ADMIN_ID, store.ITEM_ID, store.StockQuantity,"
								"item.ItemName, item.ItemPricePerUnit, item.ItemDiscount "
								"FROM store "
								"JOIN item ON store.ITEM_ID = item.ITEM_ID");

		while (res->next()) {
			cout << "| "
				<< setw(7) << res->getInt("ITEM_ID") << " | "
				<< setw(15) << res->getString("ItemName") << " | RM "
				<< setw(6) << res->getInt("ItemPricePerUnit") << "| "
				<< setw(10) << res->getInt("ItemDiscount") << "% | "
				<< setw(15) << res->getInt("StockQuantity") << "   |" << endl << endl;


		}
		
		cout << "+---------+-----------------+----------+-------------+-------------------+" << endl;

		cout << endl;

		delete res;
		delete stmt;
	}
	catch(sql::SQLException& e){
		cout << "Error: " << e.what() << endl;
	}
}

void DisplayItemMenu2(sql::Connection* con) {
	try {
		sql::Statement* stmt;
		sql::ResultSet* res;

		stmt = con->createStatement();
		res = stmt->executeQuery("SELECT store.ADMIN_ID, store.ITEM_ID, store.StockQuantity,"
			"item.ItemName, item.ItemPricePerUnit, item.ItemDiscount "
			"FROM store "
			"JOIN item ON store.ITEM_ID = item.ITEM_ID");

		while (res->next()) {
			cout << "| "
				<< setw(8) << res->getInt("ITEM_ID") << " | "
				<< setw(20) << res->getString("ItemName") << " | RM"
				<< setw(12) << res->getInt("ItemPricePerUnit") << " | "
				<< setw(13) << res->getInt("ItemDiscount") << "% | "
				<< setw(10) << res->getInt("StockQuantity") << " |" << endl << endl;


		}
		
		cout << "+----------+----------------------+----------------+----------------+------------+" << endl;

		cout << endl;

		delete res;
		delete stmt;
	}
	catch (sql::SQLException& e) {
		cout << "Error: " << e.what() << endl;
	}
}









void AddToCart(sql::Connection* con, int LoginCustomerID, int BuyingItemID, int BuyingQuantity) {
	try {
		
		sql::PreparedStatement* pstmt;
		
		pstmt = con->prepareStatement("INSERT into `order` (CUSTOMER_ID, ITEM_ID, ItemOrderQuantity, ShippingStatus) VALUES (?, ?, ?, 'Pending')"); 

		//insert data into db
		pstmt->setInt(1, LoginCustomerID);
		pstmt->setInt(2, BuyingItemID);
		pstmt->setInt(3, BuyingQuantity);


		pstmt->executeUpdate();
		

		
		
		sql::ResultSet* res;
		pstmt = con->prepareStatement("SELECT StockQuantity from store WHERE ITEM_ID = ?");
		pstmt->setInt(1, BuyingItemID);
		res = pstmt->executeQuery();

		if (res->next()) {
			CurrentStock = res->getInt("StockQuantity");// get current stock from db
			CurrentStock -= BuyingQuantity;

			pstmt = con->prepareStatement("UPDATE store SET StockQuantity = ? WHERE ITEM_ID = ?");
			pstmt->setInt(1, CurrentStock);
			pstmt->setInt(2, BuyingItemID);
			pstmt->executeUpdate();
		}

		

		delete pstmt;



	}
	catch (sql::SQLException& e) { // catch errors
		cout << "Error: " << e.what() << endl;
	}
}






int FetchCustomerID(sql::Connection* con, string LoginEmail, string LoginPassword) {
	try {
		sql::PreparedStatement* pstmt;
		sql::ResultSet* res;

		pstmt = con->prepareStatement("SELECT Name, CUSTOMER_ID FROM customer WHERE Email = ? AND Password = ?");
		pstmt->setString(1, LoginEmail);
		pstmt->setString(2, LoginPassword);

		res = pstmt->executeQuery();

		//check email and password
		if (res->next()) {
			string LoginCustomerName = res->getString("Name");
			int LoginCustomerID = res->getInt("CUSTOMER_ID");
			//cout << "Login Successful! Welcome " << LoginCustomerName << "!" << endl << endl;
			return LoginCustomerID;
		}
		else {
			cout << "Login Failed! Please Try Again!" << endl << endl;
		}
	}
	catch (sql::SQLException& e) { // catch errors
		cout << "Error: " << e.what() << endl;
	}
}




/*

void DisplayOrderItemMenu(sql::Connection* con, int FetchCustomerID) {
	try {
		//sql::Statement* stmt;
		sql::ResultSet* res;
		sql::PreparedStatement* pstmt;
		//ITEM_ID
		pstmt = con->prepareStatement("SELECT `order`.CUSTOMER_ID, `order`.ORDER_ID AS OrderItemID, item.ITEM_ID AS ItemItemID,"
			"item.ItemName, `order`.ItemOrderQuantity, item.ItemPricePerUnit, item.ItemDiscount, `order`.TotalPricePerOrder, `order`.ShippingStatus "
			"FROM item "
			"JOIN `order` ON item.ITEM_ID = `order`.ITEM_ID "
			"WHERE `order`.CUSTOMER_ID = ?");
		
		//int LoginCustomerID = FetchCustomerID(con, LoginEmail, LoginPassword);

		pstmt->setInt(1, FetchCustomerID);
		res = pstmt->executeQuery();



		

		while (res->next()) {
			
			//test
			int ItemID = res->getInt("OrderItemID");
			int ItemID2 = res->getInt("ItemItemID");

			//sum TODO
			int discount = res->getInt("ItemDiscount");
			TotalPricePerOrder = res->getInt("ItemOrderQuantity") * res->getInt("ItemPricePerUnit") * (1 - (double)discount / 100);

			sql::PreparedStatement* updateOrder = con->prepareStatement(
				"UPDATE `order` "
				"SET TotalPricePerOrder = ? "
				"WHERE CUSTOMER_ID = ? AND ITEM_ID = ?"
				//"WHERE ORDER_ID = ?"
			);

			//update problem


			updateOrder->setInt(1, TotalPricePerOrder);
			updateOrder->setInt(2, FetchCustomerID);
			updateOrder->setInt(3, ItemID2);

			updateOrder->executeUpdate();


			//OrderID2++;




			cout << "| "
				<< setw(6) << res->getInt("OrderItemID") << " | " 
				<< setw(20) << res->getString("ItemName") << " | " 
				<< setw(16) << res->getInt("ItemOrderQuantity") << " | RM" 
				<< setw(18) << res->getInt("ItemPricePerUnit") << " | RM"
				<< setw(20) << TotalPricePerOrder << " | "
				<< setw(16) << res->getString("ShippingStatus") << " |" << endl << endl;
				
				//res->getInt("TotalPricePerOrder") << endl;
				
				

		}

		
		//cout << "------------------------------------------------------------------------------------------------" << endl;
		cout << "+--------+----------------------+------------------+----------------------+------------------------+------------------+" << endl;

		cout << endl;


		


		delete res;
		
		delete pstmt;
	}
	catch (sql::SQLException& e) {
		cout << "Error: " << e.what() << endl;
	}
}

*/

//test

void DisplayOrderItemMenu(sql::Connection* con, int FetchCustomerID) {
	try {
		sql::ResultSet* res;
		sql::PreparedStatement* pstmt;
		pstmt = con->prepareStatement(
			"SELECT `order`.CUSTOMER_ID, `order`.ORDER_ID AS OrderItemID, item.ITEM_ID AS ItemItemID, "
			"item.ItemName, `order`.ItemOrderQuantity, item.ItemPricePerUnit, item.ItemDiscount, `order`.TotalPricePerOrder, `order`.ShippingStatus "
			"FROM item "
			"JOIN `order` ON item.ITEM_ID = `order`.ITEM_ID "
			"WHERE `order`.CUSTOMER_ID = ?"
		);

		pstmt->setInt(1, FetchCustomerID);
		res = pstmt->executeQuery();

		while (res->next()) {
			int orderId = res->getInt("OrderItemID");
			int itemId = res->getInt("ItemItemID");
			int quantity = res->getInt("ItemOrderQuantity");
			int unitPrice = res->getInt("ItemPricePerUnit");
			int discount = res->getInt("ItemDiscount");

			// calculate total (keep as integer; adjust rounding if needed)
			int totalPricePerOrder = static_cast<int>(quantity * unitPrice * (1.0 - (double)discount / 100.0));

			// update the specific order row by ORDER_ID (primary key)
			sql::PreparedStatement* updateOrder = con->prepareStatement(
				"UPDATE `order` SET TotalPricePerOrder = ? WHERE ORDER_ID = ?"
			);

			updateOrder->setInt(1, totalPricePerOrder);
			updateOrder->setInt(2, orderId);
			int affected = updateOrder->executeUpdate();
			/*if (affected == 0) {
				cout << "Warning: UPDATE affected 0 rows for ORDER_ID=" << orderId << " ITEM_ID=" << itemId << endl;
			}*/
			delete updateOrder;

			cout << "| "
				<< setw(6) << orderId << " | "
				<< setw(20) << res->getString("ItemName") << " | "
				<< setw(16) << quantity << " | RM"
				<< setw(18) << unitPrice << " | RM"
				<< setw(20) << totalPricePerOrder << " | "
				<< setw(16) << res->getString("ShippingStatus") << " |" << endl << endl;
		}

		cout << "+--------+----------------------+------------------+----------------------+------------------------+------------------+" << endl << endl;

		delete res;
		delete pstmt;
	}
	catch (sql::SQLException& e) {
		cout << "Error: " << e.what() << endl;
	}
}


void CalculateTotalPaymentOfACustomer(sql::Connection * con, int FetchCustomerID) {
	try {
		int TotalPayment = 0;

		sql::PreparedStatement* pstmt;
		sql::ResultSet* res;

		
		pstmt = con->prepareStatement("SELECT ItemOrderQuantity, ItemPricePerUnit, ItemDiscount "
			"FROM `order`"
			"JOIN item ON `order`.ITEM_ID = item.ITEM_ID "
			"WHERE CUSTOMER_ID = ? AND ShippingStatus = 'Pending'");

		pstmt->setInt(1, FetchCustomerID);
		res = pstmt->executeQuery();


		while(res->next()) {
			
			int quantity = res->getInt("ItemOrderQuantity");
			int price = res->getInt("ItemPricePerUnit");
			int discountofitem = res->getInt("ItemDiscount");

			TotalPayment += quantity * price * (1 - (double)discountofitem / 100);
			
		}

		cout << "Total: RM" << TotalPayment << endl << endl;

		//added
		if (TotalPayment == 0) {
			cout << "You have no item to ship!" << endl << endl;
		}

		delete res;
		delete pstmt;
	}
	catch (sql::SQLException& e) {
		cout << "Error: " << e.what() << endl;
	}
}







/*
void RemoveItem(sql::Connection* con, int ItemID, int FetchCustomerID) {
	try {
		sql::PreparedStatement* pstmt;
		pstmt = con->prepareStatement("DELETE From `order` WHERE ITEM_ID = ? AND CUSTOMER_ID = ? AND ShippingStatus = 'Pending'");

		pstmt->setInt(1, ItemID);
		pstmt->setInt(2, FetchCustomerID);

		int GotRows = pstmt->executeUpdate();


		if (GotRows > 0) {
			cout << "Items Removed!" << endl << endl;
		}
		else {
			cout << "No item to be removed!" << endl << endl;
		}

		

		delete pstmt;
	}
	catch (sql::SQLException& e) {
		cout << "Error: " << e.what() << endl;
	}
}

*/


void RemoveItem(sql::Connection* con, int ItemID, int FetchCustomerID) {
	try {
		sql::PreparedStatement* pstmt = con->prepareStatement(
			"SELECT SUM(ItemOrderQuantity) AS TotalQuantity FROM `order` WHERE  ITEM_ID = ? AND CUSTOMER_ID = ? AND ShippingStatus = 'Pending'"
		);

		pstmt->setInt(1, ItemID);
		pstmt->setInt(2, FetchCustomerID);

		sql::ResultSet* res = pstmt->executeQuery();

		int quantity = 0;

		if (res->next()) {
			quantity = res->getInt("TotalQuantity");
		}

		delete res;
		delete pstmt;

		if (quantity == 0) {
			cout << "No item to be removed!" << endl;
			return;
		}


		pstmt = con->prepareStatement(
			"UPDATE store SET StockQuantity = StockQuantity + ? WHERE Item_ID = ?"
		);

		pstmt->setInt(1, quantity);
		pstmt->setInt(2, ItemID);
		pstmt->executeUpdate();
		delete pstmt;

		pstmt = con->prepareStatement(
			"DELETE From `order` WHERE ITEM_ID = ? AND CUSTOMER_ID = ? AND ShippingStatus = 'Pending'"
		);

		pstmt->setInt(1, ItemID);
		pstmt->setInt(2, FetchCustomerID);

		int x = pstmt->executeUpdate();
		delete pstmt;

		if (x > 0) {
			cout << "Item removed!" << endl << endl;
		}
		else {
			cout << "No item to be removed!" << endl << endl;
		}


	}
	catch (sql::SQLException& e) {
		cout << "Error: " << e.what() << endl;
	}
}







void RemoveItemShip(sql::Connection* con, int FetchCustomerID, int GetOrderID) {
	try {
		sql::PreparedStatement* pstmt;
		pstmt = con->prepareStatement("UPDATE `order` SET ShippingStatus = ? WHERE CUSTOMER_ID = ? AND ShippingStatus = 'Pending' AND ORDER_ID = ?");

		pstmt->setString(1, "To Be Shipped"); //Shipped
		pstmt->setInt(2, FetchCustomerID);
		pstmt->setInt(3, GetOrderID);

		int GotRows = pstmt->executeUpdate();
		

		if (GotRows > 0) {
			cout << "Items Shipped!" << endl << endl;
		}
		else {
			cout << "Cart is empty!" << endl << endl;
		}


		
		

		delete pstmt;
	}
	catch (sql::SQLException& e) {
		cout << "Error: " << e.what() << endl;
	}
}












//admin module

bool CheckAdminCredentials(sql::Connection* con, string LoginEmail, string LoginPassword) {
	try {
		sql::PreparedStatement* pstmt;
		sql::ResultSet* res;

		pstmt = con->prepareStatement("SELECT Name, ADMIN_ID FROM admin WHERE Email = ? AND Password = ?");
		pstmt->setString(1, LoginEmail);
		pstmt->setString(2, LoginPassword);

		res = pstmt->executeQuery();

		//check email and password
		if (res->next()) {
			string LoginAdminName = res->getString("Name");
			//int LoginCustomerID = res->getInt("CUSTOMER_ID");
			cout << "Login Successful! Welcome " << LoginAdminName << "!" << endl << endl;
			return true;
		}
		else {
			cout << "Login Failed! Please Try Again!" << endl << endl;
			//added
			Refresh();
			return false;
		}
	}
	catch (sql::SQLException& e) { // catch errors
		cout << "Error: " << e.what() << endl;
	}
}



void DisplayCustomerAccList(sql::Connection * con) {
	try {
		sql::Statement* stmt;
		sql::ResultSet* res;

		stmt = con->createStatement();
		
		res = stmt->executeQuery("SELECT * FROM customer");

		cout << "(CUSTOMER ACCOUNT LIST)" << endl;
		cout << "+--------------+----------------------+--------------------------+----------------+" << endl;
		cout << "|  CustomerID  |     Customer Name    |      Customer Email      |  Account Status|" << endl;
		cout << "+--------------+----------------------+--------------------------+----------------+" << endl << endl;
		
		while (res->next()) {
			cout << "| "
				<< setw(12) << res->getInt("CUSTOMER_ID") << " | " 
				<< setw(20) << res->getString("Name") << " | "
				<< setw(24) << res->getString("Email") << " | ";

			sql::PreparedStatement* pstmt2;
			sql::ResultSet* res2;

			

			pstmt2 = con->prepareStatement("SELECT AccStatus FROM management WHERE CUSTOMER_ID = ?");

			pstmt2->setInt(1, res->getInt("CUSTOMER_ID"));
			res2 = pstmt2->executeQuery();


			if (res2->next()) {
				cout << setw(14) << res2->getString("AccStatus") << " |" << endl << endl;
			}
			else {
				cout << setw(14) << "-" << " |" << endl << endl;
			}

			delete res2;
			delete pstmt2;

		}
		
		cout << "+--------------+----------------------+--------------------------+----------------+" << endl;
		delete res;
		delete stmt;

		
	}
	catch (sql::SQLException& e) {
		cout << "Error: " << e.what() << endl;
	}
}


void UpdateCustomerEmail(sql::Connection* con, string NewCustomerEmail, int CustomerID) {
	
	/*if (NewCustomerEmail.empty() || NewCustomerEmail == " ") {
		return;
	}*/
	
	
	try {
		sql::PreparedStatement* pstmt;
		sql::ResultSet* res;
		pstmt = con->prepareStatement("SELECT * FROM customer WHERE CUSTOMER_ID = ?");

		pstmt->setInt(1, CustomerID);
		res = pstmt->executeQuery();

		if (res->next()) {
			delete pstmt;

			//sql::PreparedStatement* pstmt;
			pstmt = con->prepareStatement("UPDATE customer SET Email = ? WHERE CUSTOMER_ID = ?");
			pstmt->setString(1, NewCustomerEmail);
			pstmt->setInt(2, CustomerID);
			pstmt->executeUpdate();

			cout << "Customer email updated!" << endl;
		}
		else {
			cout << "Customer does not exit!" << endl;
		}

	}
	catch (sql::SQLException& e) {
		cout << "Error: " << e.what() << endl;
	}
}


void CreateCustomerAcc(sql::Connection* con, int LoginAdminID, int CustomerID) {
	try {
		sql::PreparedStatement* pstmt;
		//sql::ResultSet* res;
		pstmt = con->prepareStatement("INSERT INTO management (ADMIN_ID, CUSTOMER_ID, AccStatus) VALUES (?, ?, ?)");

		pstmt->setInt(1, LoginAdminID);
		pstmt->setInt(2, CustomerID);
		pstmt->setString(3, "Active");
		pstmt->executeUpdate();

		cout << "Account created!" << endl;

		delete pstmt;
		

		

	}
	catch (sql::SQLException& e) {
		cout << "Error: " << e.what() << endl;
	}
}


// rly update
void UpdateCustomerAccStatus(sql::Connection* con, int LoginAdminID, int CustomerID, string CustomerAccStatus) {
	try {
		sql::PreparedStatement* pstmt;
		//sql::ResultSet* res;
		pstmt = con->prepareStatement("UPDATE management SET AccStatus = ? WHERE CUSTOMER_ID = ?");

		pstmt->setString(1, CustomerAccStatus);
		pstmt->setInt(2, CustomerID);
		//pstmt->setString(3, CustomerAccStatus);
		pstmt->executeUpdate();

		cout << "Account status set!" << endl << endl;

		delete pstmt;




	}
	catch (sql::SQLException& e) {
		cout << "Error: " << e.what() << endl;
	}
}













int FetchAdminID(sql::Connection* con, string LoginEmail, string LoginPassword) {
	try {
		sql::PreparedStatement* pstmt;
		sql::ResultSet* res;

		pstmt = con->prepareStatement("SELECT Name, ADMIN_ID FROM admin WHERE Email = ? AND Password = ?");
		pstmt->setString(1, LoginEmail);
		pstmt->setString(2, LoginPassword);

		res = pstmt->executeQuery();

		//check email and password
		if (res->next()) {
			string LoginAdminName = res->getString("Name");
			int LoginAdminID = res->getInt("ADMIN_ID");
			//cout << "Login Successful! Welcome " << LoginCustomerName << "!" << endl << endl;
			return LoginAdminID;
		}
		else {
			cout << "Login Failed! Please Try Again!" << endl << endl;
		}
	}
	catch (sql::SQLException& e) { // catch errors
		cout << "Error: " << e.what() << endl;
	}
}


bool CustomerAccToken(sql::Connection* con, int LoginAdminID, int CustomerID, string AccStatus) {
	try {
		sql::PreparedStatement* pstmt;
		sql::ResultSet* res;

		pstmt = con->prepareStatement("SELECT COUNT(*) AS NumberOfCustomerAcc FROM management WHERE CUSTOMER_ID = ?");
		
		pstmt->setInt(1, CustomerID);
		res = pstmt->executeQuery();
		res->next();
		int count = res->getInt("NumberOfCustomerAcc");

		if (count == 0) {
			return false;
		}
		else {
			return true;
		}
	}
	catch (sql::SQLException& e) {
		cout << "Error: " << e.what() << endl;
	}
}


void RemoveCustomerAccCustomer(sql::Connection* con, int RemoveCustomerAccID) {
	try {
		sql::PreparedStatement* pstmt;
		pstmt = con->prepareStatement("DELETE FROM customer WHERE CUSTOMER_ID = ?");

		pstmt->setInt(1, RemoveCustomerAccID);
		pstmt->executeUpdate();

		cout << "Customer account deleted!" << endl << endl;

		delete pstmt;
	}
	catch (sql::SQLException& e) {
		cout << "Error: " << e.what() << endl;
	}
}



void RemoveCustomerAccManagement(sql::Connection* con, int RemoveCustomerAccID) {
	try {
		sql::PreparedStatement* pstmt;
		pstmt = con->prepareStatement("DELETE FROM management WHERE CUSTOMER_ID = ? AND AccStatus = 'Suspended'");

		pstmt->setInt(1, RemoveCustomerAccID);
		pstmt->executeUpdate();

		

		delete pstmt;
	}
	catch (sql::SQLException& e) {
		cout << "Error: " << e.what() << endl;
	}
}



void RemoveCustomerAccOrder(sql::Connection* con, int RemoveCustomerAccID) {
	try {
		sql::PreparedStatement* pstmt;
		pstmt = con->prepareStatement("DELETE FROM `order` WHERE CUSTOMER_ID = ?");

		pstmt->setInt(1, RemoveCustomerAccID);
		pstmt->executeUpdate();



		delete pstmt;
	}
	catch (sql::SQLException& e) {
		cout << "Error: " << e.what() << endl;
	}
}





void CreateAdminAcc(sql::Connection* con, string AdminAccName, string AdminAccEmail, string AdminAccPass) {
	try {
		sql::PreparedStatement* pstmt;
		pstmt = con->prepareStatement("INSERT INTO admin (Name, Email, Password) VALUES (?, ?, ?)");

		pstmt->setString(1, "Admin " + AdminAccName);
		pstmt->setString(2, "admin." + AdminAccEmail);
		pstmt->setString(3, AdminAccPass);

		pstmt->executeUpdate();

		cout << "Admin account created!" << endl << endl;
		delete pstmt;
	}
	catch (sql::SQLException& e) {
		cout << "Error: " << e.what() << endl;
	}
}



void DisplayAdminAccList(sql::Connection* con) {
	try {
		sql::Statement* stmt;
		sql::ResultSet* res;

		stmt = con->createStatement();
		res = stmt->executeQuery("SELECT * FROM admin");

		while (res->next()) {
			cout << "| "
				<< setw(10) << res->getInt("ADMIN_ID") << " | "
				<< setw(12) << res->getString("Name") << " | "
				<< setw(29) << res->getString("Email") << " | " << endl << endl;
		}
	}
	catch (sql::SQLException& e) {
		cout << "Error: " << e.what() << endl;
	}
}





void UpdateAdminAccList(sql::Connection* con, int UpdateAdminAcc, string AdminNewName, string AdminNewEmail) {
	try {


		if (!AdminNewName.empty()) {
			sql::PreparedStatement* pstmt;
			pstmt = con->prepareStatement("UPDATE admin SET Name = ?, Email = ? WHERE ADMIN_ID = ?");

			pstmt->setString(1, "Admin " + AdminNewName);
			pstmt->setString(2, AdminNewEmail);
			pstmt->setInt(3, UpdateAdminAcc);

			pstmt->executeUpdate();

			cout << "Admin account updated!" << endl << endl;

			delete pstmt;
		}
		
	
	}
	catch (sql::SQLException& e) {
		cout << "Error: " << e.what() << endl;
	}
	
}

void DeleteAdminAcc(sql::Connection* con, int AdminAccDelete) {
	try {
		sql::PreparedStatement* pstmt;
		pstmt = con->prepareStatement("DELETE FROM admin WHERE ADMIN_ID = ?");

		pstmt->setInt(1, AdminAccDelete);
		pstmt->executeUpdate();

		cout << "Admin account deleted!" << endl << endl;

		delete pstmt;
	}
	catch (sql::SQLException& e) {
		cout << "Error: " << e.what() << endl;
	}
}


void RefillItem(sql::Connection*con, int RefillItemID, int RefillItemQuantity) {
	try {
		sql::PreparedStatement* pstmt;
		
		pstmt = con->prepareStatement("UPDATE store SET StockQuantity = StockQuantity + ? WHERE ITEM_ID = ?");

		pstmt->setInt(1, RefillItemQuantity);
		pstmt->setInt(2, RefillItemID);

		int rows = pstmt->executeUpdate();

		if (rows > 0) {
			cout << "Item has been refilled!" << endl << endl;
		}
		else {
			cout << "Item is not found!" << endl << endl;
		}

		delete pstmt;
		

	}
	catch (sql::SQLException& e) {
		cout << "Error: " << e.what() << endl;
	}
}


void UnloadItem(sql::Connection* con, int UnloadItemID, int UnloadItemQuantity) {
	try {
		sql::PreparedStatement* pstmt;

		pstmt = con->prepareStatement("UPDATE store SET StockQuantity = StockQuantity - ? WHERE ITEM_ID = ?");

		pstmt->setInt(1, UnloadItemQuantity);
		pstmt->setInt(2, UnloadItemID);

		int rows = pstmt->executeUpdate();

		if (rows > 0) {
			cout << "Item has been unloaded!" << endl << endl;
		}
		else {
			cout << "Item is not found!" << endl << endl;
		}

		delete pstmt;


	}
	catch (sql::SQLException& e) {
		cout << "Error: " << e.what() << endl;
	}
}

/*
void CalculateLeftoverItems(sql::Connection*con) {
	try {
		sql::Statement* stmt;
		sql::ResultSet* res;

		stmt = con->createStatement();
		res = stmt->executeQuery("SELECT * FROM store JOIN item ON store.ITEM_ID = item.ITEM_ID");

		while (res->next()) {
			cout << res->getInt("ITEM_ID") << ",  "
				<< res->getString("ItemName") << ",  "
				<< res->getInt("StockQuantity") << endl;
		}
		delete res;
		delete stmt;
		
	}
	catch (sql::SQLException& e) {
		cout << "Error: " << e.what() << endl;
	}
}
*/

//test
void CalculateItemSales(sql::Connection* con) {
	try {
		sql::Statement* stmt;
		sql::ResultSet* res;

		stmt = con->createStatement();
		res = stmt->executeQuery("SELECT item.ITEM_ID, item.ItemName, SUM(`order`.ItemOrderQuantity) AS Total, SUM(`order`.TotalPricePerOrder) AS TotalMoney FROM `order` JOIN item ON `order`.ITEM_ID = item.ITEM_ID WHERE ShippingStatus = 'Shipped' GROUP BY item.ITEM_ID, item.ItemName");

		while (res->next()) {
			cout << "| " << setw(8) << res->getInt("ITEM_ID")
				<< "| " << setw(18) << res->getString("ItemName")
				<< "| " << setw(16) << res->getInt("Total") 
				<< "|" << setw(27) << "RM " << res->getInt("TotalMoney") << "|" << endl << endl;
		}

		//cout << "+---------+-------------------+-----------------+" << endl;
		cout << "+---------+-------------------+-----------------+-----------------------------+" << endl;
		delete res;
		delete stmt;

	}
	catch (sql::SQLException& e) {
		cout << "Error: " << e.what() << endl;
	}
}







void CalculateSoldItems(sql::Connection* con) {
	try {
		sql::Statement* stmt;
		sql::ResultSet* res;

		stmt = con->createStatement();
		res = stmt->executeQuery("SELECT SUM(ItemOrderQuantity) AS TotalSoldQuantity FROM `order` WHERE ShippingStatus = 'Shipped'");

		while (res->next()) {
			cout << res->getInt("TotalSoldQuantity") << endl << endl;
		}

		//cout << "----------------------------------" << endl;
		//cout << "---------------------------------------" << endl;
		//cout << "-------------------------------------------------" << endl;
		cout << "-------------------------------------------------------------------------------" << endl;
		delete res;
		delete stmt;
		
	}
	catch (sql::SQLException& e) {
		cout << "Error: " << e.what() << endl;
	}
}



void TotalProfit(sql::Connection* con) {
	try {
		sql::Statement* stmt;
		sql::ResultSet* res;

		stmt = con->createStatement();
		res = stmt->executeQuery("SELECT SUM(TotalPricePerOrder) AS TotalProfit FROM `order` WHERE ShippingStatus = 'Shipped'");

		

		while (res->next()) {
			cout << "RM " << res->getInt("TotalProfit") << endl;
		}

		delete res;
		delete stmt;
		
	}
	catch (sql::SQLException& e) {
		cout << "Error: " << e.what() << endl;
	}
}


string getMaskedPass() {
	string passwd = "";
	char ch;

	while (true) {
		ch = _getch();

		if (ch == 13) {
			cout << endl;
			break;
		}


		if (ch == 8) {
			if (!passwd.empty()) {
				cout << "\b \b";
				passwd.pop_back();
			}
		}
		else {
			passwd.push_back(ch);
			cout << "*";
		}

	}
	return passwd;
}



void DisplayOrderStatus(sql::Connection * con) {
	try {
		sql::Statement* stmt;
		sql::ResultSet* res;
		
		stmt = con->createStatement();
		res = stmt->executeQuery("SELECT * FROM customer AS customer INNER JOIN `order` AS `order` ON `order`.CUSTOMER_ID = customer.CUSTOMER_ID INNER JOIN item as item ON item.ITEM_ID = `order`.ITEM_ID");

		cout << "(ORDER STATUS)" << endl;
		cout << "+---------------+-----------------+---------------------+-------------+--------------------+-------------------+" << endl;
		cout << "|  Customer ID  |  Customer Name  |      Item Name      |   Quantity  |   Shipping Status  |  Shipping Address |"<< endl;
		cout << "+---------------+-----------------+---------------------+-------------+--------------------+-------------------+" << endl;
		
		
		cout << endl;

		while (res->next()) {

			string shippingAddress;
			if (res->isNull("ShippingAddress")) {
				shippingAddress = "-";
			}
			else {
				shippingAddress = res->getString("ShippingAddress");
				if (shippingAddress.empty()) {
					shippingAddress = "-";
				}
			}




			cout << "| "
				<< setw(13) << res->getInt("CUSTOMER_ID") << " | "
				<< setw(15) << res->getString("Name") << " | "
				<< setw(19) << res->getString("ItemName") << " | "
				<< setw(11) << res->getInt("ItemOrderQuantity") << " | "
				<< setw(18) << res->getString("ShippingStatus") << " | " 
				<< setw(17) << shippingAddress << " | " << endl << endl;
		}

		//cout << "+---------------+-----------------+---------------------+-------------+--------------------+----------------" << endl << endl;
		cout << "+---------------+-----------------+---------------------+-------------+--------------------+-------------------+" << endl << endl;
		delete res;
		delete stmt;

	}
	catch (sql::SQLException& e) {
		cout << "Error: " << e.what() << endl;
	}
}


void CreateItem(sql::Connection* con, string ItemName, int ItemPrice, int ItemDiscount, int FetchAdminID, int ItemStock) {
	try {

		sql::PreparedStatement* pstmtCheckExist = con->prepareStatement("SELECT ITEM_ID FROM item WHERE ItemName = ?");
		pstmtCheckExist->setString(1, ItemName);
		sql::ResultSet* resCheckExist = pstmtCheckExist->executeQuery();

		if (resCheckExist->next()) {
			cout << "Item already exists" << endl;
			return;
		}
		delete resCheckExist;
		delete pstmtCheckExist;


		sql::PreparedStatement* pstmt;
		pstmt = con->prepareStatement("INSERT INTO item (ItemName, ItemPricePerUnit, ItemDiscount) VALUES (?, ?, ?)");

		pstmt->setString(1, ItemName);
		pstmt->setInt(2, ItemPrice);
		pstmt->setInt(3, ItemDiscount);

		pstmt->executeUpdate();

		//cout << "Item Added Successful!" << endl << endl;

		delete pstmt;


		//retrieve and store item id in var
		sql::PreparedStatement* pstmt2 = con->prepareStatement("SELECT ITEM_ID from item WHERE ItemName = ?");
		pstmt2->setString(1, ItemName);

		sql::ResultSet* res = pstmt2->executeQuery();

		if (res->next()) {
			ItemID2 = res->getInt("ITEM_ID");
		}
		else {
			cout << "Item could not be found!" << endl << endl;
		}

		delete pstmt2;


		sql::PreparedStatement* pstmt3;
		pstmt3 = con->prepareStatement("INSERT INTO store (ADMIN_ID, ITEM_ID, StockQuantity) VALUES (?, ?, ?)");

		pstmt3->setInt(1, FetchAdminID);
		pstmt3->setInt(2, ItemID2);
		pstmt3->setInt(3, ItemStock);

		pstmt3->executeUpdate();

		cout << "Item and Stock Added Successful!" << endl << endl;

		delete pstmt3;



	}
	catch (sql::SQLException& e) {
		cout << "Error: " << e.what() << endl;
	}
}







void UpdateOrderStatus(sql::Connection* con) {
	try {
		

		sql::PreparedStatement* pstmt;
		pstmt = con->prepareStatement("UPDATE `order` SET ShippingStatus = 'Shipped' WHERE ShippingStatus = 'To Be Shipped'");

		pstmt->executeUpdate();
		delete pstmt;

		/*
		sql::Statement* stmt;
		sql::ResultSet* res;

		stmt = con->createStatement();
		res = stmt->executeQuery("SELECT * FROM customer AS customer INNER JOIN `order` AS `order` ON `order`.CUSTOMER_ID = customer.CUSTOMER_ID INNER JOIN item as item ON item.ITEM_ID = `order`.ITEM_ID");

		


		cout << "(ORDER STATUS)" << endl;
		cout << "+---------------+-----------------+---------------------+-------------+--------------------+" << endl;
		cout << "|  Customer ID  |  Customer Name  |      Item Name      |   Quantity  |   Shipping Status  |" << endl;
		cout << "+---------------+-----------------+---------------------+-------------+--------------------+" << endl;

		cout << endl;

		while (res->next()) {
			cout << "| "
				<< setw(13) << res->getInt("CUSTOMER_ID") << " | "
				<< setw(15) << res->getString("Name") << " | "
				<< setw(19) << res->getString("ItemName") << " | "
				<< setw(11) << res->getInt("ItemOrderQuantity") << " | "
				<< setw(18) << res->getString("ShippingStatus") << " | " << endl << endl;
		}

		cout << "+---------------+-----------------+---------------------+-------------+--------------------+" << endl << endl;

		delete res;
		delete stmt; */


		DisplayOrderStatus(con);


	}
	catch (sql::SQLException& e) {
		cout << "Error: " << e.what() << endl;
	}
}


void UpdateOrderAddress(sql::Connection* con, string ShippingAddress, int FetchCustomerID, int GetOrderID) {
	try {


		sql::PreparedStatement* pstmt;
		pstmt = con->prepareStatement("UPDATE `order` SET ShippingAddress = ? WHERE CUSTOMER_ID = ? AND ORDER_ID = ?");

		pstmt->setString(1, ShippingAddress);
		pstmt->setInt(2, FetchCustomerID);
		pstmt->setInt(3, GetOrderID);

		pstmt->executeUpdate();

		cout << "Shipping Address inserted!" << endl << endl;

		delete pstmt;

		


		


	}
	catch (sql::SQLException& e) {
		cout << "Error: " << e.what() << endl;
	}
}








