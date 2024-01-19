CREATE DATABASE amberage_hydrosync;
USE amberage_hydrosync

CREATE TABLE hydrosync_users (
    userID INT AUTO_INCREMENT PRIMARY KEY NOT NULL,
    username VARCHAR(12) NOT NULL,
    name VARCHAR(50) NOT NULL,
    lastname VARCHAR(50) NOT NULL,
    email VARCHAR(75) NOT NULL,
    phone VARCHAR(20) NOT NULL,
    password VARCHAR(4096) NOT NULL,
    registerDate TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    ip VARCHAR(50)
);

CREATE TABLE hydrosync_devices (
   serialNumber VARCHAR(12) PRIMARY KEY NOT NULL,
   tokenAuth VARCHAR(255) NOT NULL,
   typeDevice ENUM('Transmitter', 'Receiver') NOT NULL,
   ownerUser_userID INT NOT NULL,
   FOREIGN KEY (ownerUser_userID) REFERENCES hydrosync_users (userID) ON UPDATE CASCADE ON DELETE CASCADE
);

CREATE TABLE hydrosync_logs (
    registerID INT AUTO_INCREMENT PRIMARY KEY NOT NULL,
    serialNumber_device VARCHAR(12) NOT NULL,
    waterLevel VARCHAR(12) NOT NULL,
    releState ENUM('ON', 'OFF', '-') NOT NULL,
    systemMessage VARCHAR(100) NOT NULL,
    dateLog TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (serialNumber_device) REFERENCES hydrosync_devices (serialNumber) ON UPDATE CASCADE ON DELETE CASCADE
);