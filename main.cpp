#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/statement.h>
#include <iostream>
#include <mysql_connection.h>
#include <mysql_driver.h>


#include <cstdlib>

#include <conio.h>

//test
#include <ctime>
#include <sstream>


#include "functions.h"


using namespace std;

string CustomerName, CustomerEmail, CustomerPassword, CustomerAddress, CustomerPhone;
string LoginEmail, LoginPassword, LoginRole;
string LoginAdminEmail, LoginAdminPassword;
int LoginCustomerID;

int BuyingItemID;
int BuyingQuantity;
char CartQuestion;

char UserChoice;
//char ..
int UserChoiceCart;

int ItemID;


int ChoiceAdminDashboard;
int ManageCustomerChoice;
int CustomerAccID;
string NewCustomerEmail, CustomerAccStatus;
int CustomerID;
bool CustomerAccDetection;
int RemoveCustomerAccID;
int ManageStoreChoice;
int ManageAdminChoice;

string AdminAccName, AdminAccEmail, AdminAccPass;
int UpdateAdminAcc;
string AdminNewName, AdminNewEmail;
int AdminAccDelete;
int RefillItemID, RefillItemQuantity;
int UnloadItemID, UnloadItemQuantity;
int GenReportChoice, ViewOrderChoice;

int ItemPrice, ItemDiscount, ItemStock;
string ItemName;
string ShippingAddress;
int GetOrderID;

sql::mysql::MySQL_Driver* driver;
sql::Connection* con;

int main()
{


    try {



        driver = sql::mysql::get_mysql_driver_instance();
        con = driver->connect("tcp://localhost:3306",
            "root", "");

        con->setSchema("bookshop_management_db");

        sql::Statement* stmt;
        stmt = con->createStatement();

        cout << "Connected to db" << endl << endl;


    }
    catch (sql::SQLException& e) {
        std::cerr << "SQL Error: " << e.what() << std::endl;
    }

    int ChoiceMainMenu;

    do {
        //main menu ui
        cout << "--------Main Menu--------" << endl;
        cout << "Welcome to Bookshop Management System" << endl;
        cout << "1. Register" << endl;
        cout << "2. Login" << endl;
        cout << "3. Exit" << endl;

        //get user input
        cout << "Enter your choice: ";
        cin >> ChoiceMainMenu;
        cout << endl;

        switch (ChoiceMainMenu) {
        case 1:
            //register ui
            Refresh();

            cout << endl;
            cout << "Bookshop Management System - Register" << endl;

            //get customer name
            cout << "Enter Name: ";
            cin >> CustomerName;


            //get customer email
            cout << "Enter Email: ";
            cin >> CustomerEmail;


            //get customer password
            cout << "Enter Password: ";
            CustomerPassword = getMaskedPass();
            //cin >> CustomerPassword;


            //get customer address
            cout << "Enter Address: ";
            cin >> CustomerAddress;

            //get customer phone
            cout << "Enter Phone Number (XXX-XXXXXXX) : ";
            cin >> CustomerPhone;
            cout << endl;


            RegisterCustomer(con, CustomerName, CustomerEmail, CustomerPassword, CustomerAddress, CustomerPhone);

            /*
            //get user input
            cout << "Enter your Main Menu Choice: ";
            cin >> ChoiceMainMenu;
            cout << endl;
            */

            Refresh();

            break;
        case 2:
            //login ui
            Refresh();


            cout << "Bookshop Management System - Login" << endl << endl;

            //get user login email
            cout << "Enter Email: ";
            cin >> LoginEmail;

            //get user login password
            cout << "Enter Password: ";
            LoginPassword = getMaskedPass();
            //cin >> LoginPassword;


            //get role
            cout << "Enter Role (Customer): ";
            cin >> LoginRole;

            if (LoginRole == "Customer" || LoginRole == "customer") {


                if (!CheckCustomerCredentials(con, LoginEmail, LoginPassword)) {

                    break;
                }
                else {
                    int ChoiceCustomerDashboard;

                    do {
                        //customer dashboard ui 
                        Refresh();
                        cout << "Bookshop Management System - Customer Dashboard" << endl;

                        cout << "1. Shop Bookshop" << endl;
                        cout << "2. Cart" << endl;
                        cout << "3. Logout" << endl;
                        //cout << "4. Customer Dashboard" << endl;

                        //get user choice
                        cout << "Enter your choice: ";
                        cin >> ChoiceCustomerDashboard;
                        cout << endl;

                        switch (ChoiceCustomerDashboard) {
                        case 1:
                            //shop bookshop
                            do {

                                Refresh();

                                cout << "Bookshop Management System - Customer Dashboard" << endl;
                                cout << "> Shop Bookshop" << endl << endl;


                                cout << "(BOOKSHOP ITEM MENU)" << endl;
                                cout << "+---------+-----------------+----------+-------------+-------------------+" << endl;
                                cout << "| Item ID |      Item       |   Price  |   Discount  |   Stock Quantity  |" << endl;
                                cout << "+---------+-----------------+----------+-------------+-------------------+" << endl;

                                DisplayItemMenu(con);


                                cout << "Enter Item ID to buy: ";
                                cin >> BuyingItemID;

                                cout << "Enter Buying Quantity: ";
                                cin >> BuyingQuantity;

                                cout << "Add to Cart? (y/n): ";
                                cin >> CartQuestion;
                                cout << endl;

                                if (CartQuestion == 'y' || CartQuestion == 'Y') {
                                    LoginCustomerID = FetchCustomerID(con, LoginEmail, LoginPassword);
                                    //AddToCart(con, LoginCustomerID, BuyingItemID, BuyingQuantity);



                                    while (true) {




                                        AddToCart(con, LoginCustomerID, BuyingItemID, BuyingQuantity);
                                        break;



                                    }

                                    break;

                                }
                                else if (CartQuestion == 'n' || CartQuestion == 'N') {
                                    break;
                                }
                                else {
                                    cout << "Invalid choice, Please Try Again!" << endl;
                                }



                            } while (CartQuestion == 'y' || CartQuestion == 'Y');


                            break;


                        case 2:
                            //cart system



                            do {
                                Refresh();
                                cout << "Bookshop Management System - Customer Dashboard" << endl;
                                cout << "> Cart" << endl << endl;

                                cout << "(CART ITEM MENU)" << endl;
                                cout << "+--------+----------------------+------------------+----------------------+------------------------+------------------+" << endl;
                                cout << "|OrderID |        Item          |  Buying Quantity |  Original Price (RM) |  Discounted Price (RM) |  Shipping Status |" << endl;
                                cout << "+--------+----------------------+------------------+----------------------+------------------------+------------------+" << endl;

                                if (LoginCustomerID <= 0) {
                                    LoginCustomerID = FetchCustomerID(con, LoginEmail, LoginPassword);
                                }





                                cout << endl;
                                DisplayOrderItemMenu(con, LoginCustomerID);
                                CalculateTotalPaymentOfACustomer(con, LoginCustomerID);


                                cout << "Select your choice: " << endl;
                                cout << "1. Checkout" << endl;
                                cout << "2. Remove Item" << endl;
                                cout << "3. Customer Dashboard" << endl;

                                UserChoiceCart = 0; //reset value
                                cout << "Enter your choice: ";
                                cin >> UserChoiceCart;
                                cout << endl;






                                switch (UserChoiceCart) {
                                case 1:
                                    //checkout

                                     
                                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                                    cout << "Enter your shipping address: ";
                                    getline(cin, ShippingAddress);
                                   



                                    char Checkout;

                                    cout << "Do you want to checkout? (y/n): ";
                                    cin >> Checkout;
                                    cout << endl;

                                    if (Checkout == 'y' || Checkout == 'Y') {
                                        
                                        
                                        cout << "Enter Order ID: ";
                                        cin >> GetOrderID;


                                        //ship item
                                        RemoveItemShip(con, LoginCustomerID, GetOrderID);
                                        UpdateOrderAddress(con, ShippingAddress, LoginCustomerID, GetOrderID);

                                    }
                                    else if (Checkout == 'n' || Checkout == 'N') {
                                        break;
                                    }
                                    else {
                                        cout << "Invalid choice, Please Try Again!" << endl;
                                    }


                                    break;
                                case 2:
                                    //remove item 

                                    cout << "Enter Item ID: ";
                                    cin >> ItemID;

                                    RemoveItem(con, ItemID, LoginCustomerID);
                                    break;
                                case 3:
                                    break;
                                default:
                                    cout << "Invalid choice, Please Try Again!" << endl;
                                    break;
                                }



                            } while (UserChoiceCart != 3);





                            break;

                        case 3:
                            //logout
                            Refresh();
                            break;


                            //case 4:
                            //    //customer dashboard
                            //    
                            //    cout << "You are already in Customer Dashboard!" << endl << endl;
                            //    break;

                        default:

                            cout << "Invalid choice, Please Try Again!" << endl;
                            break;

                        }

                    } while (ChoiceCustomerDashboard != 3);
                }

                //put here back if wrong



            }
            else if (LoginRole == "Admin" || LoginRole == "admin") {

                if (!CheckAdminCredentials(con, LoginEmail, LoginPassword)) {
                    break;
                }
                else {
                    //admin login
                    int LoginAdminID = FetchAdminID(con, LoginEmail, LoginPassword);

                    do {
                        Refresh();
                        cout << "Bookshop Management System - Admin Dashboard" << endl << endl;

                        cout << "1. Manage Customer Accounts" << endl;
                        cout << "2. Manage Admin Accounts" << endl;
                        cout << "3. Manage Store" << endl;
                        cout << "4. Manage Order Status" << endl;
                        cout << "5. Generate Report" << endl;
                        cout << "6. Logout" << endl;

                        cout << "Enter your choice: ";
                        cin >> ChoiceAdminDashboard;
                        cout << endl;

                        Refresh();

                        switch (ChoiceAdminDashboard) {
                        case 1:
                            //manage customer acc



                            do {
                                Refresh();
                                cout << "Bookshop Management System - Admin Dashboard" << endl;
                                cout << "> Manage Customer Accounts" << endl << endl;

                                //display customer acc list

                                DisplayCustomerAccList(con);
                                cout << endl;


                                cout << "1. Update Customer Accounts" << endl;
                                cout << "2. Delete Customer Accounts" << endl;
                                cout << "3. Admin Dashboard" << endl;

                                cout << "Enter your choice: ";
                                cin >> ManageCustomerChoice;
                                cout << endl;


                                switch (ManageCustomerChoice) {
                                case 1:

                                    //update customer acc
                                    cout << "Enter Account ID: ";
                                    cin >> CustomerAccID;

                                    CustomerID = CustomerAccID;

                                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                                    cout << "Enter Account New Email: ";
                                    //cin >> NewCustomerEmail;
                                    getline(cin, NewCustomerEmail);

                                   

                                    cout << "Enter Account Status (Active, Suspended): ";
                                    //cin >> CustomerAccStatus;
                                    getline(cin, CustomerAccStatus);
                                    //cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                    cout << endl;

                                    CustomerAccDetection = CustomerAccToken(con, LoginAdminID, CustomerID, CustomerAccStatus);

                                    if (!CustomerAccDetection) {



                                        CreateCustomerAcc(con, LoginAdminID, CustomerID);

                                        UpdateCustomerEmail(con, NewCustomerEmail, CustomerID);
                                        UpdateCustomerAccStatus(con, LoginAdminID, CustomerID, CustomerAccStatus);
                                    }
                                    else {
                                        //CreateCustomerAcc(con, LoginAdminID, CustomerID);
                                        UpdateCustomerEmail(con, NewCustomerEmail, CustomerID);
                                        UpdateCustomerAccStatus(con, LoginAdminID, CustomerID, CustomerAccStatus);
                                    }



                                    break;


                                case 2:
                                    //delete customer acc

                                    cout << "Enter Account ID to delete: ";
                                    cin >> RemoveCustomerAccID;
                                    cout << endl;


                                    RemoveCustomerAccManagement(con, RemoveCustomerAccID);
                                    RemoveCustomerAccOrder(con, RemoveCustomerAccID);
                                    RemoveCustomerAccCustomer(con, RemoveCustomerAccID);

                                    break;

                                case 3:
                                    break;
                                default:
                                    cout << "Invalid choice, Please Try Again!" << endl;
                                    break;
                                }






                            } while (ManageCustomerChoice != 3);

                            break;
                        case 2:
                            //manage admin acc


                            do {
                                Refresh();
                                cout << "Bookshop Management System - Admin Dashboard" << endl;
                                cout << "> Manage Admin Accounts" << endl << endl;

                                cout << "(ADMIN ACCOUNT LIST)" << endl;
                                cout << "+------------+--------------+-------------------------------+" << endl;
                                cout << "|  Admin ID  |  Admin Name  |          Admin Email          |" << endl;
                                cout << "+------------+--------------+-------------------------------+" << endl << endl;

                                //call display func below
                                DisplayAdminAccList(con);

                                //cout << "---------------------------------------------" << endl;
                                cout << "+------------+--------------+-------------------------------+" << endl;

                                cout << endl;

                                cout << "1. Create Admin Account" << endl;
                                cout << "2. Update Admin Account" << endl;
                                cout << "3. Delete Admin Account" << endl;
                                cout << "4. Admin Dashboard" << endl;

                                cout << "Enter your choice: ";
                                cin >> ManageAdminChoice;
                                cout << endl;

                                switch (ManageAdminChoice) {
                                case 1:
                                    //create admin acc
                                    cout << "Enter account name: ";
                                    cin >> AdminAccName;

                                    cout << "Enter account email: ";
                                    cin >> AdminAccEmail;

                                    cout << "Enter account password: ";
                                    AdminAccPass = getMaskedPass();
                                    cout << endl;

                                    //call func
                                    CreateAdminAcc(con, AdminAccName, AdminAccEmail, AdminAccPass);

                                    break;
                                case 2:
                                    //update admin acc
                                    cout << "Enter account ID: ";
                                    cin >> UpdateAdminAcc;

                                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                                    cout << "Enter account new name: ";
                                    //cin >> AdminNewName;
                                    getline(cin, AdminNewName);

                                    cout << "Enter account new email: ";
                                    //cin >> AdminNewEmail;
                                    getline(cin, AdminNewEmail);
                                    cout << endl;

                                    UpdateAdminAccList(con, UpdateAdminAcc, AdminNewName, AdminNewEmail);


                                    break;
                                case 3:
                                    //delete admin acc
                                    cout << "Enter account ID to delete: ";
                                    cin >> AdminAccDelete;
                                    cout << endl;

                                    DeleteAdminAcc(con, AdminAccDelete);

                                    break;
                                case 4:
                                    //admin dashboard
                                    break;
                                default:
                                    cout << "Invalid choice, Please Try Again!" << endl << endl;
                                    break;
                                }


                            } while (ManageAdminChoice != 4);



                            break;
                        case 3:
                            //manage store

                            do {
                                Refresh();
                                cout << "Bookshop Management System - Admin Dashboard" << endl;
                                cout << "> Manage Inventory System" << endl << endl;

                                cout << "(BOOKSHOP ITEM MENU)" << endl;
                                cout << "+----------+----------------------+----------------+-----------------------------+" << endl;
                                cout << "|  ItemID  |       Item Name      |   Price (RM)   |  Discount (%)  |  Quantity  |" << endl;
                                cout << "+----------+----------------------+----------------+----------------+------------+" << endl << endl;

                                DisplayItemMenu2(con);

                                cout << "1. Create Items" << endl;
                                cout << "2. Refill Items" << endl;
                                cout << "3. Unload Items" << endl;
                                cout << "4. Admin Dashboard" << endl;

                                cout << "Enter your choice: ";
                                cin >> ManageStoreChoice;
                                cout << endl;

                                switch (ManageStoreChoice) {


                                case 1:
                                    //create items
                                    cout << "Enter Item Name: ";
                          
                                    getline(cin >> ws, ItemName);

                                    cout << "Enter Item Price: ";
                                    cin >> ItemPrice;

                                    cout << "Enter Item Discount: ";
                                    cin >> ItemDiscount;

                                    cout << "Enter Stock Quantity: ";
                                    cin >> ItemStock;
                                    cout << endl;

                                    CreateItem(con, ItemName, ItemPrice, ItemDiscount, LoginAdminID, ItemStock);

                                    break;
                                case 2:
                                    //refill items
                                    cout << "Enter Item ID: ";
                                    cin >> RefillItemID;

                                    cout << "Enter Item Quantity: ";
                                    cin >> RefillItemQuantity;
                                    cout << endl;

                                    RefillItem(con, RefillItemID, RefillItemQuantity);

                                    break;
                                case 3:
                                    //unload itens
                                    cout << "Enter Item ID: ";
                                    cin >> UnloadItemID;

                                    cout << "Enter Item Quantity: ";
                                    cin >> UnloadItemQuantity;
                                    cout << endl;

                                    UnloadItem(con, UnloadItemID, UnloadItemQuantity);
                                    break;
                                case 4:
                                    //admin dasboard
                                    break;
                                default:
                                    cout << "Invalid choice, Please Try Again!" << endl;
                                    break;
                                }


                            } while (ManageStoreChoice != 4);






                            break;


                        case 4:
                            //view order status

                            do {

                                cout << "Bookshop Management System - Admin Dashboard" << endl;
                                cout << "> Manage Order Status" << endl << endl;

                                cout << "1. View Order Status" << endl;
                                cout << "2. Update Order Status" << endl;
                                cout << "3. Admin Dashboard" << endl;


                                cout << "Enter your choice: ";
                                cin >> ViewOrderChoice;
                                cout << endl << endl;

                                Refresh();

                                switch (ViewOrderChoice) {
                                case 1:
                                    // view order status
                                    DisplayOrderStatus(con);
                                    break;
                                case 2:
                                    //update order status
                                    UpdateOrderStatus(con);
                                    break;

                                case 3:
                                    //admin dashboard
                                    break;
                                default:
                                    cout << "Invalid choice, Please Try Again!" << endl;
                                    break;
                                }





                            } while (ViewOrderChoice != 3);

                            break;

                        case 5:
                            //generate report




                            do {


                                cout << "Bookshop Management System - Admin Dashboard" << endl;
                                cout << "> Report" << endl << endl;

                                cout << "1. Generate Sales Report" << endl;
                                cout << "2. Admin Dashboard" << endl;




                                cout << "Enter your choice: ";
                                cin >> GenReportChoice;
                                cout << endl << endl;

                                //test
                                Refresh();

                                switch (GenReportChoice) {
                                case 1:

                                    cout << "(SALES REPORT)" << endl;
                                    cout << "-------------------------------------------------------------------------------" << endl;
                                    cout << "Profit Earned: ";
                                    TotalProfit(con);

                                    cout << endl;

                                    //cout << "Leftover Items" << endl;
                                    cout << "+---------+-------------------+-----------------+-----------------------------+" << endl;
                                    cout << "| Item ID |      Item Name    |  Sold Quantity  | Total Profit Earned Per Item|" << endl;
                                    cout << "+---------+-------------------+-----------------+-----------------------------+" << endl;
                                    
                                    
                                    //CalculateLeftoverItems(con);
                                    CalculateItemSales(con);

                                    cout << endl;

                                    cout << "Total Item Sold: ";
                                    CalculateSoldItems(con);
                                    cout << endl << endl;
                                    break;
                                case 2:
                                    //admin dashboard
                                    break;
                                default:
                                    cout << "Invalid choice, Please Try Again!" << endl;
                                    break;
                                }
                            } while (GenReportChoice != 2);



                            break;
                        case 6:
                            //logout
                            Refresh();
                            break;
                        default:
                            cout << "Invalid choice, Please Try Again!" << endl;
                            break;
                        }


                    } while (ChoiceAdminDashboard != 6);
                }










            }
            else {
                cout << "Invalid Role, Please Try Again!" << endl << endl;
            }










            break;

        case 3:
            cout << "Exited!" << endl << endl;
            break;

        default:
            cout << "Invalid choice, Please Try Again!" << endl;
            break;

        }


    } while (ChoiceMainMenu != 3);






    return 0;
}