-- @author 巷北  
-- @time 2026-01-01 22:56:45  

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
