CREATE DATABASE demo;
USE demo;

CREATE TABLE user (
  id INT PRIMARY KEY AUTO_INCREMENT,
  name VARCHAR(20)
);

INSERT INTO user(name) VALUES ('Tom'), ('Jerry');
