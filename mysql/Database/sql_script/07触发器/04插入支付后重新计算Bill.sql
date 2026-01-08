-- @author 巷北  
-- @time 2026-01-01 22:42:32  

DELIMITER $$
CREATE TRIGGER trg_payment_after_insert_update_bill
AFTER INSERT ON Payment
FOR EACH ROW
BEGIN
  DECLARE v_sumPaid DECIMAL(12,2);
  DECLARE v_total DECIMAL(12,2);

  SELECT IFNULL(SUM(paidAmount),0) INTO v_sumPaid FROM Payment WHERE billId = NEW.billId;
  SELECT totalAmount INTO v_total FROM Bill WHERE billId = NEW.billId;

  IF v_sumPaid >= v_total THEN
    UPDATE Bill SET paymentStatus = '已支付', settledTime = NOW() WHERE billId = NEW.billId;
  ELSEIF v_sumPaid > 0 THEN
    UPDATE Bill SET paymentStatus = '部分支付', settledTime = NULL WHERE billId = NEW.billId;
  ELSE
    UPDATE Bill SET paymentStatus = '未支付', settledTime = NULL WHERE billId = NEW.billId;
  END IF;
END$$
DELIMITER ;
