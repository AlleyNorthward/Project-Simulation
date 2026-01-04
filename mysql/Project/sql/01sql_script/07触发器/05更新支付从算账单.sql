-- @author 巷北  
-- @time 2026-01-01 22:43:06  

DELIMITER $$
CREATE TRIGGER trg_payment_after_update_recalc_bill
AFTER UPDATE ON Payment
FOR EACH ROW
BEGIN
  DECLARE v_sumPaid DECIMAL(12,2);
  DECLARE v_total DECIMAL(12,2);
  DECLARE v_billId INT;

  -- 支持 payment 的 billId 可能被改动，这里以 NEW.billId 为准
  SET v_billId = NEW.billId;

  SELECT IFNULL(SUM(paidAmount),0) INTO v_sumPaid FROM Payment WHERE billId = v_billId;
  SELECT totalAmount INTO v_total FROM Bill WHERE billId = v_billId;

  IF v_sumPaid >= v_total THEN
    UPDATE Bill SET paymentStatus = '已支付', settledTime = NOW() WHERE billId = v_billId;
  ELSEIF v_sumPaid > 0 THEN
    UPDATE Bill SET paymentStatus = '部分支付', settledTime = NULL WHERE billId = v_billId;
  ELSE
    UPDATE Bill SET paymentStatus = '未支付', settledTime = NULL WHERE billId = v_billId;
  END IF;
END$$
DELIMITER ;
