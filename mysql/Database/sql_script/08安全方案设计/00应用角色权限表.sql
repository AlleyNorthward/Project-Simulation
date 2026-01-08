-- @author 巷北  
-- @time 2026-01-01 22:55:15  

SET NAMES utf8mb4;
USE hotel_management;

-- 应用角色表
CREATE TABLE IF NOT EXISTS AppRole (
  roleId INT AUTO_INCREMENT PRIMARY KEY,
  roleName VARCHAR(50) NOT NULL UNIQUE,
  description VARCHAR(255)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- 应用权限表（可细化，例如：RESERVATION.CREATE, BILL.VIEW）
CREATE TABLE IF NOT EXISTS AppPermission (
  permId INT AUTO_INCREMENT PRIMARY KEY,
  permName VARCHAR(100) NOT NULL UNIQUE,
  description VARCHAR(255)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- 角色-权限关联
CREATE TABLE IF NOT EXISTS AppRolePermission (
  roleId INT NOT NULL,
  permId INT NOT NULL,
  PRIMARY KEY(roleId, permId),
  CONSTRAINT fk_arp_role FOREIGN KEY(roleId) REFERENCES AppRole(roleId) ON DELETE CASCADE,
  CONSTRAINT fk_arp_perm FOREIGN KEY(permId) REFERENCES AppPermission(permId) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- 员工-角色关联（把 Employee 表里的员工与应用角色关联）
CREATE TABLE IF NOT EXISTS AppUserRole (
  employeeId INT NOT NULL,
  roleId INT NOT NULL,
  assignedBy INT NULL,
  assignedAt DATETIME DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY(employeeId, roleId),
  CONSTRAINT fk_aur_employee FOREIGN KEY(employeeId) REFERENCES Employee(employeeId) ON DELETE CASCADE,
  CONSTRAINT fk_aur_role FOREIGN KEY(roleId) REFERENCES AppRole(roleId) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;
