-- @author 巷北  
-- @time 2026-01-01 22:31:55  

DELIMITER $$
CREATE PROCEDURE sp_add_payment(
  IN p_billId INT,
  IN p_paidAmount DECIMAL(10,2),
  IN p_method VARCHAR(20),
  IN p_operatorId INT,
  OUT p_paymentId INT
)
BEGIN
  DECLARE v_totalBill DECIMAL(12,2);
  DECLARE v_sumPaid DECIMAL(12,2);
  DECLARE EXIT HANDLER FOR SQLEXCEPTION
  BEGIN
    ROLLBACK;
    SET p_paymentId = NULL;
  END;

  START TRANSACTION;
    SELECT totalAmount INTO v_totalBill FROM Bill WHERE billId = p_billId FOR UPDATE;

    INSERT INTO Payment (billId, paidAmount, paymentMethod, paymentTime, operatorId, remarks)
    VALUES (p_billId, p_paidAmount, p_method, NOW(), p_operatorId, NULL);

    SET p_paymentId = LAST_INSERT_ID();

    SELECT IFNULL(SUM(paidAmount), 0) INTO v_sumPaid FROM Payment WHERE billId = p_billId;

    IF v_sumPaid >= v_totalBill THEN
      UPDATE Bill SET paymentStatus = '已支付', settledTime = NOW() WHERE billId = p_billId;
    ELSEIF v_sumPaid > 0 THEN
      UPDATE Bill SET paymentStatus = '部分支付' WHERE billId = p_billId;
    ELSE
      UPDATE Bill SET paymentStatus = '未支付' WHERE billId = p_billId;
    END IF;

  COMMIT;
END$$
DELIMITER ;

-- CALL sp_add_payment(1, 585.00, '微信', 4, @payid);
-- SELECT @payid;
