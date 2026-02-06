-- phpMyAdmin SQL Dump
-- version 5.2.0
-- https://www.phpmyadmin.net/
--
-- Host: 127.0.0.1
-- Generation Time: Jan 10, 2026 at 07:56 AM
-- Server version: 10.4.27-MariaDB
-- PHP Version: 8.1.12

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `bookshop_management_db`
--

-- --------------------------------------------------------

--
-- Table structure for table `admin`
--

CREATE TABLE `admin` (
  `ADMIN_ID` int(255) NOT NULL COMMENT 'PRIMARY KEY',
  `Name` varchar(1000) NOT NULL,
  `Email` varchar(1000) NOT NULL,
  `Password` varchar(1000) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dumping data for table `admin`
--

INSERT INTO `admin` (`ADMIN_ID`, `Name`, `Email`, `Password`) VALUES
(1, 'Aric', 'admin.aric@gmail.com', 'admin123');

-- --------------------------------------------------------

--
-- Table structure for table `customer`
--

CREATE TABLE `customer` (
  `CUSTOMER_ID` int(255) NOT NULL COMMENT 'PRIMARY KEY',
  `Name` varchar(1000) NOT NULL,
  `Email` varchar(1000) NOT NULL,
  `Password` varchar(1000) NOT NULL,
  `Address` varchar(1000) NOT NULL,
  `PhoneNo` varchar(1000) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dumping data for table `customer`
--

INSERT INTO `customer` (`CUSTOMER_ID`, `Name`, `Email`, `Password`, `Address`, `PhoneNo`) VALUES
(1, 'Aric', 'aric@gmail.com', '123', 'UK', '018-3952355'),
(3, 'Jane', 'jane@gmail.com', '123', 'Singapore', '011-1018888');

-- --------------------------------------------------------

--
-- Table structure for table `item`
--

CREATE TABLE `item` (
  `ITEM_ID` int(255) NOT NULL COMMENT 'PRIMARY KEY',
  `ItemName` varchar(1000) NOT NULL,
  `ItemPricePerUnit` varchar(1000) NOT NULL,
  `ItemDiscount` varchar(1000) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dumping data for table `item`
--

INSERT INTO `item` (`ITEM_ID`, `ItemName`, `ItemPricePerUnit`, `ItemDiscount`) VALUES
(1, 'ICT Book', '20', '2'),
(2, 'Physics Book', '30', '5'),
(3, 'Art Book', '15', '3'),
(4, 'Business Book', '20', '5');

-- --------------------------------------------------------

--
-- Table structure for table `management`
--

CREATE TABLE `management` (
  `ADMIN_ID` int(255) NOT NULL,
  `CUSTOMER_ID` int(255) NOT NULL,
  `AccStatus` varchar(1000) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dumping data for table `management`
--

INSERT INTO `management` (`ADMIN_ID`, `CUSTOMER_ID`, `AccStatus`) VALUES
(1, 1, 'Active'),
(1, 3, 'Active');

-- --------------------------------------------------------

--
-- Table structure for table `order`
--

CREATE TABLE `order` (
  `ORDER_ID` int(255) NOT NULL COMMENT 'PRIMARY KEY',
  `CUSTOMER_ID` int(255) NOT NULL,
  `ITEM_ID` int(255) NOT NULL,
  `ItemOrderQuantity` int(255) NOT NULL,
  `TotalPricePerOrder` int(255) NOT NULL,
  `ShippingStatus` varchar(1000) NOT NULL,
  `ShippingAddress` varchar(1000) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dumping data for table `order`
--

INSERT INTO `order` (`ORDER_ID`, `CUSTOMER_ID`, `ITEM_ID`, `ItemOrderQuantity`, `TotalPricePerOrder`, `ShippingStatus`, `ShippingAddress`) VALUES
(2, 1, 1, 2, 39, 'Shipped', 'Singapore'),
(5, 1, 3, 1, 14, 'Shipped', 'Singapore'),
(10, 1, 1, 1, 19, 'Shipped', 'USA');

-- --------------------------------------------------------

--
-- Table structure for table `store`
--

CREATE TABLE `store` (
  `ADMIN_ID` int(255) NOT NULL,
  `ITEM_ID` int(255) NOT NULL,
  `StockQuantity` int(255) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dumping data for table `store`
--

INSERT INTO `store` (`ADMIN_ID`, `ITEM_ID`, `StockQuantity`) VALUES
(1, 1, 83),
(1, 2, 95),
(1, 3, 94),
(1, 4, 100);

--
-- Indexes for dumped tables
--

--
-- Indexes for table `admin`
--
ALTER TABLE `admin`
  ADD PRIMARY KEY (`ADMIN_ID`);

--
-- Indexes for table `customer`
--
ALTER TABLE `customer`
  ADD PRIMARY KEY (`CUSTOMER_ID`);

--
-- Indexes for table `item`
--
ALTER TABLE `item`
  ADD PRIMARY KEY (`ITEM_ID`);

--
-- Indexes for table `management`
--
ALTER TABLE `management`
  ADD KEY `Admin FK` (`ADMIN_ID`),
  ADD KEY `Customer FK2` (`CUSTOMER_ID`);

--
-- Indexes for table `order`
--
ALTER TABLE `order`
  ADD PRIMARY KEY (`ORDER_ID`),
  ADD KEY `Customer FK` (`CUSTOMER_ID`),
  ADD KEY `Item FK` (`ITEM_ID`);

--
-- Indexes for table `store`
--
ALTER TABLE `store`
  ADD KEY `Item FK2` (`ITEM_ID`),
  ADD KEY `Admin FK 2` (`ADMIN_ID`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `admin`
--
ALTER TABLE `admin`
  MODIFY `ADMIN_ID` int(255) NOT NULL AUTO_INCREMENT COMMENT 'PRIMARY KEY', AUTO_INCREMENT=4;

--
-- AUTO_INCREMENT for table `customer`
--
ALTER TABLE `customer`
  MODIFY `CUSTOMER_ID` int(255) NOT NULL AUTO_INCREMENT COMMENT 'PRIMARY KEY', AUTO_INCREMENT=5;

--
-- AUTO_INCREMENT for table `item`
--
ALTER TABLE `item`
  MODIFY `ITEM_ID` int(255) NOT NULL AUTO_INCREMENT COMMENT 'PRIMARY KEY', AUTO_INCREMENT=5;

--
-- AUTO_INCREMENT for table `order`
--
ALTER TABLE `order`
  MODIFY `ORDER_ID` int(255) NOT NULL AUTO_INCREMENT COMMENT 'PRIMARY KEY', AUTO_INCREMENT=27;

--
-- Constraints for dumped tables
--

--
-- Constraints for table `management`
--
ALTER TABLE `management`
  ADD CONSTRAINT `Admin FK` FOREIGN KEY (`ADMIN_ID`) REFERENCES `admin` (`ADMIN_ID`),
  ADD CONSTRAINT `Customer FK2` FOREIGN KEY (`CUSTOMER_ID`) REFERENCES `customer` (`CUSTOMER_ID`);

--
-- Constraints for table `order`
--
ALTER TABLE `order`
  ADD CONSTRAINT `Customer FK` FOREIGN KEY (`CUSTOMER_ID`) REFERENCES `customer` (`CUSTOMER_ID`),
  ADD CONSTRAINT `Item FK` FOREIGN KEY (`ITEM_ID`) REFERENCES `item` (`ITEM_ID`);

--
-- Constraints for table `store`
--
ALTER TABLE `store`
  ADD CONSTRAINT `Admin FK 2` FOREIGN KEY (`ADMIN_ID`) REFERENCES `admin` (`ADMIN_ID`),
  ADD CONSTRAINT `Item FK2` FOREIGN KEY (`ITEM_ID`) REFERENCES `item` (`ITEM_ID`);
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
