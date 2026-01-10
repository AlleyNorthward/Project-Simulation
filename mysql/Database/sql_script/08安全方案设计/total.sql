SET NAMES utf8mb4;
USE hotel_management;

CREATE TABLE IF NOT EXISTS AppRole (
  roleId INT AUTO_INCREMENT PRIMARY KEY,
  roleName VARCHAR(50) NOT NULL UNIQUE,
  description VARCHAR(255)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

CREATE TABLE IF NOT EXISTS AppPermission (
  permId INT AUTO_INCREMENT PRIMARY KEY,
  permName VARCHAR(100) NOT NULL UNIQUE,
  description VARCHAR(255)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

CREATE TABLE IF NOT EXISTS AppRolePermission (
  roleId INT NOT NULL,
  permId INT NOT NULL,
  PRIMARY KEY(roleId, permId),
  CONSTRAINT fk_arp_role FOREIGN KEY(roleId) REFERENCES AppRole(roleId) ON DELETE CASCADE,
  CONSTRAINT fk_arp_perm FOREIGN KEY(permId) REFERENCES AppPermission(permId) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

CREATE TABLE IF NOT EXISTS AppUserRole (
  employeeId INT NOT NULL,
  roleId INT NOT NULL,
  assignedBy INT NULL,
  assignedAt DATETIME DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY(employeeId, roleId),
  CONSTRAINT fk_aur_employee FOREIGN KEY(employeeId) REFERENCES Employee(employeeId) ON DELETE CASCADE,
  CONSTRAINT fk_aur_role FOREIGN KEY(roleId) REFERENCES AppRole(roleId) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;


INSERT IGNORE INTO AppRole(roleName, description) VALUES
('ADMIN', '系统管理员，最高权限'),
('MANAGER', '运营/店长，管理预订/入住/报表'),
('FRONTDESK', '前台，办理预订与入住/退房'),
('MAINTENANCE', '维修人员，管理维修记录'),
('FINANCE', '财务人员，处理账单/收款');

INSERT IGNORE INTO AppPermission(permName, description) VALUES
('RESERVATION.CREATE','创建预订'),
('RESERVATION.CANCEL','取消预订'),
('STAY.CHECKIN','办理入住'),
('STAY.CHECKOUT','办理退房'),
('ROOM.VIEW','查看房间信息'),
('ROOM.EDIT','修改房间状态/信息'),
('MAINTENANCE.MANAGE','管理维修记录'),
('BILL.VIEW','查看账单'),
('BILL.EDIT','修改账单'),
('PAYMENT.MANAGE','收款/退款'),
('CUSTOMER.MANAGE','管理客户资料'),
('REPORT.VIEW','查看运营/财务报表');

INSERT IGNORE INTO AppRolePermission(roleId, permId)
SELECT r.roleId, p.permId
FROM AppRole r CROSS JOIN AppPermission p
WHERE r.roleName = 'ADMIN';

INSERT IGNORE INTO AppRolePermission(roleId, permId)
SELECT r.roleId, p.permId
FROM AppRole r
JOIN AppPermission p ON p.permName IN ('RESERVATION.CREATE','RESERVATION.CANCEL','STAY.CHECKIN','STAY.CHECKOUT','ROOM.VIEW','ROOM.EDIT','BILL.VIEW','BILL.EDIT','REPORT.VIEW')
WHERE r.roleName = 'MANAGER';

INSERT IGNORE INTO AppRolePermission(roleId, permId)
SELECT r.roleId, p.permId
FROM AppRole r
JOIN AppPermission p ON p.permName IN ('RESERVATION.CREATE','RESERVATION.CANCEL','STAY.CHECKIN','STAY.CHECKOUT','ROOM.VIEW','CUSTOMER.MANAGE')
WHERE r.roleName = 'FRONTDESK';

INSERT IGNORE INTO AppRolePermission(roleId, permId)
SELECT r.roleId, p.permId
FROM AppRole r
JOIN AppPermission p ON p.permName IN ('MAINTENANCE.MANAGE','ROOM.VIEW')
WHERE r.roleName = 'MAINTENANCE';

INSERT IGNORE INTO AppRolePermission(roleId, permId)
SELECT r.roleId, p.permId
FROM AppRole r
JOIN AppPermission p ON p.permName IN ('BILL.VIEW','BILL.EDIT','PAYMENT.MANAGE','REPORT.VIEW')
WHERE r.roleName = 'FINANCE';


DELIMITER $$

CREATE PROCEDURE sp_assign_app_role(
  IN p_employeeUsername VARCHAR(50),
  IN p_roleName VARCHAR(50),
  IN p_assignedByUsername VARCHAR(50)
)
BEGIN
  DECLARE v_empId INT;
  DECLARE v_roleId INT;
  DECLARE v_byId INT;

  SELECT employeeId INTO v_empId 
  FROM Employee 
  WHERE username = p_employeeUsername COLLATE utf8mb4_unicode_ci
  LIMIT 1;

  SELECT roleId INTO v_roleId 
  FROM AppRole 
  WHERE roleName = p_roleName COLLATE utf8mb4_unicode_ci
  LIMIT 1;

  SELECT employeeId INTO v_byId 
  FROM Employee 
  WHERE username = p_assignedByUsername COLLATE utf8mb4_unicode_ci
  LIMIT 1;

  IF v_empId IS NULL OR v_roleId IS NULL THEN
    SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'employee or role not found';
  END IF;

  INSERT INTO AppUserRole(employeeId, roleId, assignedBy)
  VALUES (v_empId, v_roleId, v_byId)
  ON DUPLICATE KEY UPDATE assignedAt = CURRENT_TIMESTAMP, assignedBy = v_byId;

END$$

-- 撤销角色
CREATE PROCEDURE sp_revoke_app_role(
  IN p_employeeUsername VARCHAR(50),
  IN p_roleName VARCHAR(50)
)
BEGIN
  DECLARE v_empId INT;
  DECLARE v_roleId INT;

  SELECT employeeId INTO v_empId 
  FROM Employee 
  WHERE username = p_employeeUsername COLLATE utf8mb4_unicode_ci
  LIMIT 1;

  SELECT roleId INTO v_roleId 
  FROM AppRole 
  WHERE roleName = p_roleName COLLATE utf8mb4_unicode_ci
  LIMIT 1;

  IF v_empId IS NULL OR v_roleId IS NULL THEN
    SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'employee or role not found';
  END IF;

  DELETE FROM AppUserRole 
  WHERE employeeId = v_empId AND roleId = v_roleId;

END$$

DELIMITER ;


CALL sp_assign_app_role('wangqianlong', 'ADMIN', 'dev_wang');
CALL sp_assign_app_role('yankaijie', 'ADMIN', 'dev_yan');
CALL sp_assign_app_role('zhangteer', 'ADMIN', 'dev_zhang_');
CALL sp_assign_app_role('teacher', 'ADMIN', 'dev_teacher');

CALL sp_assign_app_role('front_zhang', 'FRONTDESK', 'dev_zhou');
CALL sp_assign_app_role('front_li', 'MANAGER', 'dev_zhou');
CALL sp_assign_app_role('acct_zhao', 'FINANCE', 'dev_zhou');
CALL sp_assign_app_role('maint_chen', 'MAINTENANCE', 'dev_zhou');
CALL sp_assign_app_role('dev_zhou', 'ADMIN', 'dev_zhou');
