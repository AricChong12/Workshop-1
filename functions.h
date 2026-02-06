#pragma once

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/statement.h>
#include <iostream>
#include <mysql_connection.h>
#include <mysql_driver.h>

using namespace std;


void Refresh();


//function prototypes
//user management funcs
void RegisterCustomer(sql::Connection* con, string CustomerName, string CustomerEmail, string CustomerPassword, string CustomerAddress, string CustomerPhone);
bool CheckCustomerCredentials(sql::Connection* con, string LoginEmail, string LoginPassword);
//void CheckAdminCredentials(sql::Connection* con, string LoginAdminEmail, string LoginAdminPassword);
bool CheckAdminCredentials(sql::Connection* con, string LoginEmail, string LoginPassword);



//customer dashboard funcs
void DisplayItemMenu(sql::Connection* con);
void AddToCart(sql::Connection* con, int LoginCustomerID, int BuyingItemID, int BuyingQuantity);
int FetchCustomerID(sql::Connection* con, string LoginEmail, string LoginPassword);
void DisplayOrderItemMenu(sql::Connection* con, int FetchCustomerID);
void CalculateTotalPaymentOfACustomer(sql::Connection* con, int FetchCustomerID);
void RemoveItem(sql::Connection* con, int ItemID, int FetchCustomerID);
void RemoveItemShip(sql::Connection* con, int FetchCustomerID, int GetOrderID);


//admin dashboard funcs
void DisplayCustomerAccList(sql::Connection* con);
int FetchAdminID(sql::Connection* con, string LoginEmail, string LoginPassword);
void UpdateCustomerEmail(sql::Connection* con, string NewCustomerEmail, int CustomerID);
void CreateCustomerAcc(sql::Connection* con, int LoginAdminID, int CustomerID);
void UpdateCustomerAccStatus(sql::Connection* con, int LoginAdminID, int CustomerID, string CustomerAccStatus);
bool CustomerAccToken(sql::Connection* con, int LoginAdminID, int CustomerID, string AccStatus);
void RemoveCustomerAccCustomer(sql::Connection* con, int RemoveCustomerAccID);
void RemoveCustomerAccManagement(sql::Connection* con, int RemoveCustomerAccID);
void RemoveCustomerAccOrder(sql::Connection* con, int RemoveCustomerAccID);
void CreateAdminAcc(sql::Connection* con, string AdminAccName, string AdminAccEmail, string AdminAccPass);
void DisplayAdminAccList(sql::Connection* con);
void UpdateAdminAccList(sql::Connection* con, int UpdateAdminAcc, string AdminNewName, string AdminNewEmail);
void DeleteAdminAcc(sql::Connection* con, int AdminAccDelete);

void RefillItem(sql::Connection* con, int RefillItemID, int RefillItemQuantity);
void UnloadItem(sql::Connection* con, int UnloadItemID, int UnloadItemQuantity);
//void CalculateLeftoverItems(sql::Connection* con);
void CalculateSoldItems(sql::Connection* con);
void TotalProfit(sql::Connection* con);

void DisplayItemMenu2(sql::Connection* con);
string getMaskedPass();
void DisplayOrderStatus(sql::Connection* con);
void CreateItem(sql::Connection* con, string ItemName, int ItemPrice, int ItemDiscount, int FetchAdminID, int ItemStock);
void CalculateItemSales(sql::Connection* con);
void UpdateOrderStatus(sql::Connection* con);
void UpdateOrderAddress(sql::Connection* con, string ShippingAddress, int FetchCustomerID, int GetOrderID);