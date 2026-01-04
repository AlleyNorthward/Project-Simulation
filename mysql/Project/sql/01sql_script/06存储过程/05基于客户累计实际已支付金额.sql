-- @author 巷北  
-- @time 2026-01-01 22:32:44  

DELIMITER $$
CREATE PROCEDURE sp_promote_membership_based_on_spending(
  IN p_customerId INT,
  OUT p_newLevelId INT
)
BEGIN
  DECLARE v_totalPaid DECIMAL(12,2);

  -- 计算客户已支付总额（通过 Payment -> Bill -> Stay -> Customer）
  SELECT IFNULL(SUM(p.paidAmount),0) INTO v_totalPaid
  FROM Payment p
  JOIN Bill b ON p.billId = b.billId
  JOIN Stay s ON b.checkInId = s.checkInId
  WHERE s.customerId = p_customerId;

  -- 找出满足 requiredSpending <= v_totalPaid 的最大等级
  SELECT levelId INTO p_newLevelId
  FROM MembershipLevel
  WHERE requiredSpending <= v_totalPaid
  ORDER BY requiredSpending DESC
  LIMIT 1;

  -- 若找到等级则更新客户
  IF p_newLevelId IS NOT NULL THEN
    UPDATE Customer SET membershipLevelId = p_newLevelId WHERE customerId = p_customerId;
  END IF;
END$$
DELIMITER ;

-- CALL sp_promote_membership_based_on_spending(1, @newlevel);
-- SELECT @newlevel;
