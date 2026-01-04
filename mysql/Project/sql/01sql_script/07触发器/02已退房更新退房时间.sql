-- @author 巷北  
-- @time 2026-01-01 22:41:18  

DELIMITER $$
CREATE TRIGGER trg_stay_before_update_set_actual_checkout
BEFORE UPDATE ON Stay
FOR EACH ROW
BEGIN
  IF NEW.status = '已退房' AND OLD.status <> '已退房' THEN
    IF NEW.actualCheckOutTime IS NULL THEN
      SET NEW.actualCheckOutTime = NOW();
    END IF;
  END IF;
END$$
DELIMITER ;
